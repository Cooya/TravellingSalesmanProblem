/**
 * \file minimum_spanning_tree.h
 * \brief Fichier d'en-tête de l'algorithme Minimum Spanning Tree
 * \author Nicolas Marcy
 * \version
 * \date
 */
#ifndef MINIMUM_SPANNING_TREE_H_INCLUDED
#define MINIMUM_SPANNING_TREE_H_INCLUDED

#include "../city.h"
#include "../map.h"
/**
 * \fn City* minimumSpanningTree(Map m, City cityBegin)
 * \brief Fonction qui exécute l'algorithme Minimum Spanning Tree.
 * \param Map m : Map à laquelle est appliqué l'algorithme.
 * \param City cityBegin : Ville de départ et d'arrivée du chemin.
 * \return Un chemin sous la forme d'un tableau de City.
 */
City* minimumSpanningTree(Map m, City cityBegin);


#endif // MINIMUM_SPANNING_TREE_H_INCLUDED
