/**
 * \file branch_and_bound_hk.h
 * \brief Fichier d'en-tête de l'algorithme Branch and Bound with Held Karp relaxation
 * \author David Phan, Raphael Afanyan
 * \version 2014
 * \date 2014
 */

#ifndef BRANCH_AND_BOUND_HK_H_INCLUDED
#define BRANCH_AND_BOUND_HK_H_INCLUDED

#include "../city.h"
#include "../map.h"

/**
 * \fn City *branchAndBoundHK(Map m, City c)
 * \brief Renvoie le chemin optimal
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */
City* branchAndBoundHK(Map m, City c);

#endif // BRANCH_AND_BOUND_H_INCLUDED
