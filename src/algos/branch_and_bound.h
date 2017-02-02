/**
 * \file branch_and_bound.h
 * \brief Fichier d'en-tête de l'algorithme Branch and Bound
 * \author David Phan
 * \version 2014
 * \date 2014
 */

#ifndef BRANCH_AND_BOUND_H_INCLUDED
#define BRANCH_AND_BOUND_H_INCLUDED

#include "../city.h"
#include "../map.h"

/**
 * \fn City *branchAndBound(Map m, City c)
 * \brief Renvoie le chemin optimal
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */
City* branchAndBound(Map m, City c);

/**
 * \fn City *branchAndBoundNNMST(Map m, City c)
 * \brief Renvoie le chemin optimal avec relaxation de NearestNeighbour MST
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */
City* branchAndBoundNNMST(Map, City);

#endif // BRANCH_AND_BOUND_H_INCLUDED
