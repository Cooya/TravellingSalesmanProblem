/**
 * \file tree.c
 * \brief Fichier d'implémentation de la structure Tree utilisée pour l'algo MST.
 * \author Nicolas Marcy
 * \version
 * \date 2014
 */

#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

/**
* \def NB_SONS_MAX_DEFAULT
* \brief Nombre de fils maximum fixé pour les sommets de l'arbre, utilisé pour la création du tableau des fils qui peut être étendu si besoin.
*/

#define NB_SONS_MAX_DEFAULT 3

/**
* \struct Tree tree.h
* \brief Structure représentant un arbre mathématique. Tree est composé d'un pointeur sur un sommet courant et d'un tableau stockant tous les sommets de l'arbre.
*/

struct Tree
{
    Top currentTop; /*!< Sommet pointé. */
    Top* arrayTops; /*!< Tableau de pointeurs de sommets stockant tous les sommets de l'abre. */
    int indexArray; /*!< Nombre de sommets dans le tableau (c'est-à-dire ajoutés à l'arbre). */
    int nbTops; /*!< Nombre de sommets total. */
};

/**
* \struct Top tree.h
* \brief Structure représentant un sommet d'arbre mathématique. Les sommets sont reliés par "chaînage", c'est-à-dire un pointeur vers un père et un tableau de pointeurs vers des fils.
*/

struct Top
{
    int indice; /*!< Indice du sommet (de 0 à nbTops-1). */
    int nbSons; /*!< Nombre de fils du sommet. */
    int nbSonsMax; /*!< Nombre de fils maximum du sommet, défini au début par NB_SONS_MAX_DEFAULT. */
    Top* sons; /*!< Tableau de pointeurs vers les sommets fils. */
    Top father; /*!< Pointeur vers le sommet père */
};

/**
* \fn Tree createTree(int size)
* \brief Fonction de création et d'initialisation de l'abre.
* \param size Taille de l'arbre, c'est-à-dire le nombre de sommets de l'arbre.
* \return Nouvelle instance d'un Tree.
*/

Tree createTree(int size)
{
    Tree T=malloc(sizeof(struct Tree));
    T->arrayTops=malloc(sizeof(Top)*size);
    T->indexArray=0;
    T->currentTop=NULL;
    T->nbTops=0;
    return T;
}

/**
* \fn Top createTop(int indice)
* \brief Fonction de création et d'initialisation d'un sommet de l'arbre.
* \param int indice : Indice du sommet.
* \return Nouvelle instance d'un Top.
*/

static Top createTop(int indice)
{
    Top t=malloc(sizeof(struct Top));
    t->indice=indice;
    t->nbSons=0;
    t->nbSonsMax=NB_SONS_MAX_DEFAULT;
    t->sons=malloc(sizeof(Top)*NB_SONS_MAX_DEFAULT);
    return t;
}

/**
* \fn void addTopToTree(Tree T, int iBegin, int iEnd)
* \brief Fonction d'ajout d'un sommet à l'arbre.
* \param Tree T : Arbre auquel on ajout le sommet.
* \param int iBegin : Indice du sommet père auquel on ajoute un fils.
* \param int iEnd : Indice du sommet fils que l'on ajoute à l'arbre.
* \return void
*/

void addTopToTree(Tree T, int iBegin, int iEnd)
{
    Top t=createTop(iEnd);
    if (T->currentTop==NULL) // si c'est le premier sommet qu'on ajoute
        t->father=NULL;
    else
    {
        for(int i=0; i<T->indexArray; i++) // on cherche le sommet de départ de l'arête dans le tableau des sommets
            if(T->arrayTops[i]->indice==iBegin)
                T->currentTop=T->arrayTops[i]; // on pointe vers lui pour lui ajouter un fils
        t->father=T->currentTop;
        if (T->currentTop->nbSons>=T->currentTop->nbSonsMax) // si tableau des fils plein
        {
            T->currentTop->nbSonsMax*=2;
            T->currentTop->sons=realloc(T->currentTop->sons,sizeof(Top)*T->currentTop->nbSonsMax); // augmentation de la taille du tableau des fils
        }
        T->currentTop->sons[T->currentTop->nbSons++]=t;
    }
    T->arrayTops[T->indexArray++]=t; // on ajoute ce sommet au tableau des sommets
    T->currentTop=t; // l'abre pointe désormais sur ce nouveau sommet
}

/**
* \fn void upToRoot(Tree T)
* \brief Fonction qui remonte le pointeur de la structure Tree à la racine de l'arbre.
* \param Tree T : Arbre concerné.
* \return void
*/

void upToRoot(Tree T)
{
    while(T->currentTop->father!=NULL)
        T->currentTop=T->currentTop->father;
}

/**
* \fn static void destroyTops(Top t)
* \brief Fonction utilisée par la fonction destroyTree pour libérer récursivement la mémoire allouée pour les sommets de l'arbre.
* \param Top t : Racine de l'arbre ou du sous-arbre à détruire. (Pour libérer tous les sommets d'un arbre, la racine de l'arbre doit être passée en paramètre à l'appel de cette fonction.)
* \return void
*/

static void destroyTops(Top t)
{
    for(int i=0; i<t->nbSons; i++)
        destroyTops(t->sons[i]);
    free(t->sons);
    free(t);
}

/**
* \fn destroyTree(Tree T)
* \brief Fonction pour libérer la mémoire allouée par l'arbre et ses sommets.
* \param T Arbre à détruire.
* \return void
*/

void destroyTree(Tree T)
{
    upToRoot(T);
    destroyTops(T->currentTop);
    free(T->arrayTops);
    free(T);
}

/**
* \fn void fillIndexesArrayFromTree(Tree T, int** iPath)
* \brief Fonction récursive qui remplit un tableau d'indices à partir de l'arbre passé en paramètre.
* \param Tree T Arbre à parcourir.
* \param int** iPath Pointeur sur tableau d'indices à remplir. (On utilise un pointeur sur tableau pour avoir une adresse qui ne change jamais à chaque appel récursif.)
* \return void
*/

void fillIndexesArrayFromTree(Tree T, int** iPath)
{
    **iPath=T->currentTop->indice;
    (*iPath)++;
    Top currentTop=T->currentTop; // on sauvegarde le sommet courant pour ne pas le perdre car il change après
    for(int i=0; i<currentTop->nbSons; ++i)
    {
        T->currentTop=currentTop->sons[i];
        fillIndexesArrayFromTree(T,iPath); // on descend vers les fils jusqu'aux feuilles de l'arbre
    }
}
