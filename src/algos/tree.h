/**
 * \file tree.h
 * \brief Fichier d'en-tête de la structure Tree utilisée pour l'algo MST.
 * \author Nicolas Marcy
 * \version
 * \date 2014
 */

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef struct Top* Top;
typedef struct Tree* Tree;

/**
* \fn Tree createTree(int size)
* \brief Fonction de création et d'initialisation de l'abre.
* \param size Taille de l'arbre, c'est-à-dire le nombre de sommets de l'arbre.
* \return Nouvelle instance d'un Tree.
*/
Tree createTree(int);
/**
* \fn destroyTree(Tree T)
* \brief Fonction pour libérer la mémoire allouée par l'arbre et ses sommets.
* \param T Arbre à détruire.
* \return void
*/

void destroyTree(Tree);

/**
* \fn void addTopToTree(Tree T, int iBegin, int iEnd)
* \brief Fonction d'ajout d'un sommet à l'arbre.
* \param Tree T : Arbre auquel on ajout le sommet.
* \param int iBegin : Indice du sommet père auquel on ajoute un fils.
* \param int iEnd : Indice du sommet fils que l'on ajoute à l'arbre.
* \return void
*/

void addTopToTree(Tree,int,int);

/**
* \fn void upToRoot(Tree T)
* \brief Fonction qui remonte le pointeur de la structure Tree à la racine de l'arbre.
* \param Tree T : Arbre concerné.
* \return void
*/

void upToRoot(Tree);

/**
* \fn void fillIndexesArrayFromTree(Tree T, int** iPath)
* \brief Fonction récursive qui remplit un tableau d'indices à partir de l'arbre passé en paramètre.
* \param Tree T Arbre à parcourir.
* \param int** iPath Pointeur sur tableau d'indices à remplir. (On utilise un pointeur sur tableau pour avoir une adresse qui ne change jamais à chaque appel récursif.)
* \return void
*/

void fillIndexesArrayFromTree(Tree, int**);

#endif
