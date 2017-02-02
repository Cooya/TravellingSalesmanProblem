/**
 * \file fcts.h
 * \brief Fonctions utilitaires aux autres fonctions
 * \author David Phan, Jason Pindat
 * \version 0.28
 * \date 12/03/2014
 *
 * Fonctions utilitaires pour les affichages dans le terminal (mode Verbose etc) , ainsi que des fonctions d'encapsulations de mémoires
 * et des fonctions de compteurs du Clock
 *
 */

#ifndef FCTS_H
#define FCTS_H

#include <stdbool.h>
#include "string.h"
#include "city.h"

/** \fn void setVerboseMode(int v)
 *
 * \param v Le niveau de verbose
 *
 */
void setVerboseMode(int);
/** \fn void initVerboseMode()
 *
 * Initialise le _verbose à 0
 *
 */
void initVerboseMode();
/** \fn int getVerboseMode()
 *
 * \return Le niveau de verbose
 *
 */
int getVerboseMode();
/** \fn void setWerrorMode(bool we)
 *
 * \param we booleen
 * définit _werror
 */
void setWerrorMode(bool);
/** \fn void randInit()
 *  \brief Initialise le srand pour la fonction randNum
 *
 *
 */
void randInit();
/** \fn int randNum(int a, int b)
 * \brief renvoie un numéro aléatoire borné entre a et b
 *
 * \param a entier borne minimum
 * \param b entier borne maximale
 * \return un entier aléatoire
 *
 */
int randNum(int, int);
/** \fn void throwErr(Str module, Str msg, Str addMsg)
 *
 * \brief Affiche une erreur dans tous les modes verboses
 * \param module Chaine de caracteres qui désigne le Nom du module où l'on a une erreur (par ex si nous sommes dans le Main)
 * \param msg Chaine de caracteres qui désigne le message d'erreur
 * \param addMsg Chaine de caracteres qui désigne un message additionnel
 *
 */
void throwErr(Str, Str, Str);
/** \fn void throwWarn(Str module, Str msg, Str addMsg)
 *  \brief Affiche un message d'avertissement lorsque verbose >=1
 *
 * \param module Chaine de caracteres qui désigne le Nom du module où l'on a une erreur (par ex si nous sommes dans le Main)
 * \param msg Chaine de caracteres qui désigne le message d'erreur
 * \param addMsg Chaine de caracteres qui désigne un message additionnel
 *
 */
void throwWarn(Str, Str, Str);
/** \fn void throwMsg(Str module, Str msg, Str addMsg)
 *  \brief Affiche un message lorsque verbose >=2 c'est à dire qu'il est en mode verbose (./VDC -v )
 *
 * \param module Chaine de caracteres qui désigne le Nom du module où l'on a une erreur (par ex si nous sommes dans le Main)
 * \param msg Chaine de caracteres qui désigne le message d'erreur
 * \param addMsg Chaine de caracteres qui désigne un message additionnel
 *
 */
void throwMsg(Str, Str);
/** \fn void clockBegin(void)
 *
 * \brief met _clock à la valeur initiale
 *
 */
void clockBegin();
/** \fn double clockStop()
 *
 * \return le temps d'execution de l'horloge
 *
 */
double clockStop();
/** \fn int* arrIndexesCreate(int nb)
 *
 * \param nb Nombre d'entiers
 * \return tableau d'entiers de taille nb
 *
 *  Crée un tableau d'entiers de taille nb
 *
 */
int* arrIndexesCreate(int nb);
/** \fn void freeArrIndexes(int *arr)
 *
 * \param arr Reference du Tableau d'entiers
 *
 *  Libère un tableau d'entiers
 *
 */
void freeArrIndexes(int * arr);
/** \fn int* arrIndexesRealloc(int nb)
 *
 * \param nb Nombre d'entiers
 * \param ptr Pointeur du tableau d'entiers
 * \return tableau d'entiers de taille nb
 *
 *  Redimnesionne un tableau d'entiers en taille nb
 *
 */
int *arrIndexesRealloc(int *ptr, int nb);
/** \fn int* arrCitiesCreate(int nb)
 *
 * \param nb Nombre d'entiers
 * \param ptr Pointeur du tableau d'entiers
 * \return tableau d'entiers de taille nb
 *
 *  Créer un tableau de City en taille nb
 *
 */
City* arrCitiesCreate(int nb);
/** \fn void freeArrCities(City *arr)
 *
 * \param arr Reference du Tableau de City
 *
 *  Libère un tableau de City
 *
 */
void freeArrCities(City* arr);
/** \fn Str  getTime()
 *
 * Renvoie la date
 */
Str getTime();

#endif

