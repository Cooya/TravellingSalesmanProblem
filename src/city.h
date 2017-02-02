/**
 * \file city.h
 * \brief Structure City
 * \author David Phan, Jason Pindat
 * \version 0.19
 * \date 24/03/2014
 * Structure City et ses fonctions d'instanciation, de suppression et d'affichage
 */

#ifndef CITY_H
#define CITY_H

#include <stdbool.h>
#include "point.h"

typedef struct _City *City;

/** \fn City cityCreate(bool isPos, Point pos, double *distsBuffer, int distsSize)
 *  \brief Crée l'objet de type City par rapport à la matrice de Distance
 * \param isPos est à TRUE si City est défini par un Point
 * \param pos Point qui donne la position de la City
 * \param distsBuffer Tableau de distance
 * \param distsSize nombre de Distances du tableau distsBuffer
 * \return Objet de type City
 */

City cityCreate(bool, Point pos, double *distsBuffer, int distsSize);

/** \fn void cityDelete(City c)
 *  \brief Libère l'objet de type City
 * \param c Objet de type City
 */

void cityDelete(City c);

/** \fn double cityGetDist(City c, int i)
 *  \brief Renvoie la distance d'une City par rapport à une autre (par son indice)
 * \param c Objet de type City
 * \param i Indice de la deuxième City
 * \return Distance entre la City c et la City d'indice i
 */

double cityGetDist(City, int);

/** \fn int cityGetDistsSize(City c)
 *  \brief Retourne le nombre de distance qu'a l'objet City
 * \param c Objet de type City
 * \return Nombre de distance
 */

int cityGetDistsSize(City c);

/** \fn Point cityGetPos(City c)
 *  \brief Retourne l'objet Point si défini par un Point de l'objet City c
 * \param c Objet de type City
 * \return Objet de type Point
 */

Point cityGetPos(City c);

/** \fn bool cityGetIsPos(City c)
 *  \brief Retourne si l'objet de type City est défini par un objet de type Point
 * \param c Objet de type City
 * \return TRUE si elle est définie par un Point, FALSE si elle n'est définie que par un tableau de Distance
 */

bool cityGetIsPos(City c);

/** \fn int cityGetIndex(City c)
 *  \brief Retourne l'indice de la ville
 * \param c Objet de type City
 * \return L'indice auquel est cette ville dans sa Map
 */

int cityGetIndex(City c);

/** \fn int citySetIndex(City c, int val)
 *  \brief Met à jour l'indice de la ville dans la Map
 * \param c Objet de type City
 * \param val entier : indice de la ville
 */

void citySetIndex(City c, int val);

/** \fn bool cityEquals(City c1, City c2)
 *  \brief Retourne si deux objets City sont égales
 * \param c1 Objet de type City
 * \param c2 Objet de type City
 * \return TRUE si les tableaux de Distances de c1 et c2 sont les mêmes, FALSE sinon
 */

bool cityEquals(City, City);

/** \fn void cityDataDump(City c)
 *  \brief Affiche sur le terminal les données de l'objet City ( Position s'il y a , Tableau de distances relative à ce point)
 * \param c Objet de type City
 */

void cityDataDump(City);

#endif

