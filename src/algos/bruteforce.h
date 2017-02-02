/**
 * \file bruteforce.h
 * \brief Fichier implémentant l'algorithme bruteforce en plusieurs versions.
 * \author David Phan
 * \version 2014
 * \date 2014
 */

#ifndef BRUTEFORCE_H_INCLUDED
#define BRUTEFORCE_H_INCLUDED


#include "../city.h"
#include "../map.h"
/**
 * \fn City *bruteForce(Map m, City c)
 * \brief renvoie le chemin optimal
 * \param Map m : carte
 * \param City c : la ville de depart
 * \return tableau de City
 */
City* bruteForce(Map m, City c);
/**
 * \fn City* bruteForce_recursif(Map m, City c)
 * \brief Renvoie le tableau de cities du tour optimal
 * \param Map m : l'instance
 * \param City c : la ville de depart
 * \return City * tableau de city
 */
City *bruteForce_recursif(Map m, City c);
/**
 * \fn City *bruteForce_mt(Map m, City c)
 * \brief Renvoie le chemin optimal
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */
City *bruteForce_mt(Map m, City c);


#endif // BRUTEFORCE_H_INCLUDED
