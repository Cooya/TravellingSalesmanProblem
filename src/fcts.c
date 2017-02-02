/**
 * \file fcts.c
 * \brief Fonctions utilitaires aux autres fonctions
 * \author David Phan, Jason Pindat
 * \version 0.28
 * \date 12/03/2014
 *
 * Fonctions utilitaires pour les affichages dans le terminal (mode Verbose etc) , ainsi que des fonctions d'encapsulations de mémoires
 * et des fonctions de compteurs du Clock
 *
 */


#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "fcts.h"
#include "string.h"

#if defined (_WIN32) || defined (_WIN64)
#define CLOCKDIV 1
#elif defined (__linux)
#define CLOCKDIV 1000
#endif


int _verbose;
bool _werror;

clock_t _clock;

/** \fn void initVerboseMode()
 *
 * Initialise le _verbose à 0
 *
 */

void initVerboseMode()
{
    _verbose=0;
}

/** \fn void setVerboseMode(int v)
 *
 * \param v Le niveau de verbose
 *
 */

void setVerboseMode(int v)
{
    if(v>_verbose)
        _verbose=v;
}

/** \fn int getVerboseMode()
 *
 * \return Le niveau de verbose
 *
 */

int getVerboseMode()
{
    return _verbose;
}
/** \fn void setWerrorMode(bool we)
 *
 * \param we booleen
 * définit _werror
 */

void setWerrorMode(bool we)
{
    _werror=we;
}


/** \fn void randInit()
 *  \brief Initialise le srand pour la fonction randNum
 *
 *
 */

void randInit()
{
    srand(time(NULL));
}
/** \fn int randNum(int a, int b)
 * \brief renvoie un numéro aléatoire borné entre a et b
 *
 * \param a entier borne minimum
 * \param b entier borne maximale
 * \return un entier aléatoire
 *
 */

int randNum(int a, int b)
{
    return rand()%(b-a+1)+a;
}

/** \fn void throwErr(Str module, Str msg, Str addMsg)
 *
 * \brief Affiche une erreur dans tous les modes verboses
 * \param module Chaine de caracteres qui désigne le Nom du module où l'on a une erreur (par ex si nous sommes dans le Main)
 * \param msg Chaine de caracteres qui désigne le message d'erreur
 * \param addMsg Chaine de caracteres qui désigne un message additionnel
 *
 */

void throwErr(Str module, Str msg, Str addMsg)
{
    printf("/!\\ [%s] %s", module, msg);

    if(addMsg!=NULL)
        printf(" \"%s\"", addMsg);
    printf("\n");

    exit(1);
}
/** \fn void throwWarn(Str module, Str msg, Str addMsg)
 *  \brief Affiche un message d'avertissement lorsque verbose >=1
 *
 * \param module Chaine de caracteres qui désigne le Nom du module où l'on a une erreur (par ex si nous sommes dans le Main)
 * \param msg Chaine de caracteres qui désigne le message d'erreur
 * \param addMsg Chaine de caracteres qui désigne un message additionnel
 *
 */

void throwWarn(Str module, Str msg, Str addMsg)
{
    if(_werror)
        throwErr(module, msg, addMsg);
    else
    {
        if(_verbose>=1)
        {
            printf("(!) [%s] %s", module, msg);

            if(addMsg!=NULL)
                printf(" \"%s\"", addMsg);
            printf("\n");
        }
    }
}
/** \fn void throwMsg(Str module, Str msg, Str addMsg)
 *  \brief Affiche un message lorsque verbose >=2 c'est à dire qu'il est en mode verbose (./VDC -v )
 *
 * \param module Chaine de caracteres qui désigne le Nom du module où l'on a une erreur (par ex si nous sommes dans le Main)
 * \param msg Chaine de caracteres qui désigne le message d'erreur
 * \param addMsg Chaine de caracteres qui désigne un message additionnel
 *
 */
void throwMsg(Str module, Str msg)
{
    if(_verbose>=2)
    {
        printf(" -> [%s] %s\n", module, msg);
    }
}

/** \fn long factorielle(int x)
 *
 * \param x nombre d'iteration de la factorielle
 *
 */

long factorielle(int x)
{
    long resultat=1;
    for(; x>1; --x)
        resultat*=x;
    return resultat;
}


/** \fn void clockBegin(void)
 *
 * \brief met _clock à la valeur initiale
 *
 */

void clockBegin(void)
{
    _clock=clock();
}
/** \fn double clockStop()
 *
 * \return le temps d'execution de l'horloge
 *
 */

double clockStop()
{
    clock_t end=clock();
    return ((double)end-(double)_clock)/CLOCKDIV;
}
/** \fn int* arrIndexesCreate(int nb)
 *
 * \param nb Nombre d'entiers
 * \return tableau d'entiers de taille nb
 *
 *  Crée un tableau d'entiers de taille nb
 *
 */

int* arrIndexesCreate(int nb)
{
    int *arr=malloc(sizeof(int)*(nb));
    return arr;
}
/** \fn int* arrIndexesRealloc(int nb)
 *
 * \param nb Nombre d'entiers
 * \param ptr Pointeur du tableau d'entiers
 * \return tableau d'entiers de taille nb
 *
 *  Redimnesionne un tableau d'entiers en taille nb
 *
 */
int *arrIndexesRealloc(int *ptr, int nb)
{
    return (int*)realloc(ptr, sizeof(int)*nb);
}
/** \fn void freeArrIndexes(int *arr)
 *
 * \param arr Reference du Tableau d'entiers
 *
 *  Libère un tableau d'entiers
 *
 */
void freeArrIndexes(int * arr)
{
    free(arr);
}
/** \fn City *arrCitiesCreate(int nb)
 *
 * \param nb Nombre de City que le tableau aura
 * \return Tableau de City qui aura nb City
 * Crée un tableau de City de taille nb
 */

City* arrCitiesCreate(int nb)
{
    City *citiesArr=malloc(sizeof(City)*(nb));
    return citiesArr;
}
/** \fn void freeArrCities(City *arr)
 *
 * \param arr réference du tableau de City
 * Libère le tableau de City
 */

void freeArrCities(City* arr)
{
    free(arr);
}
/** \fn Str  getTime()
 *
 * Renvoie la date
 */
Str getTime()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Str txt = malloc(1000);
    sprintf(txt, "%d-%d-%d-%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return txt;
}
