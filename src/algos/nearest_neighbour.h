/**
 * \file nearest_neighbour.h
 * \brief Fichier d'en-tête de l'algorithme Nearest Neighbour.
 * \author Jason Pindat, Nicolas Marcy
 * \version
 * \date
 */

#ifndef NEAREST_NEIGHBOUR_H_INCLUDED
#define NEAREST_NEIGHBOUR_H_INCLUDED

#include "../city.h"
#include "../map.h"
/**
 * \fn City* nearestNeighbour(Map m, City c)
 * \brief Fonction qui exécute l'algorithme Nearest Neighbour.
 * \param Map m : Map à laquelle est appliqué l'algorithme.
 * \param City c : Ville de départ et d'arrivée du chemin.
 * \return Un chemin sous la forme d'un tableau de City.
 */

City* nearestNeighbour(Map m, City c);

/**
 * \fn City* nearestNeighbour2(Map m, City cityBegin)
 * \brief Fonction qui exécute une seconde version de l'algorithme Nearest Neighbour.
 * \param Map m : Map à laquelle est appliquée l'algorithme.
 * \param City cityBegin : Ville de départ et d'arrivée du chemin.
 * \return Un chemin sous la forme d'un tableau de City.
 */
City* nearestNeighbour2(Map m, City cityBegin);


#endif // NEAREST_NEIGHBOUR_H_INCLUDED
