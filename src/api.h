/**
* \file api.h
* \brief Programme de sortie de map et résultats au format JSON. Les algorithmes acceptés sont NN, MST et BBRHK.
* \author David Phan
* \version
* \date Avril 2014
*/

#ifndef API_H
#define API_H

#include "city.h"
#include "map.h"

/** \fn void executeApis(Map, bool *, int)
 * \brief Execute les algorithmes demandés (NN, MST, BBRHK acceptés) l'ecriture des JSON selon les algos demandés ainsi que la Map
 * \param Map : La map à traiter
 * \param bool* : un tableau d'algorithmes de taille NB_ALGOS, quand le bool = TRUE; alors il va exécuter printJSON
 * \param int : la City de départ
 */

void executeApis(Map, bool*, int);

/** \fn void printJSON(Map, City *, int)
 *  \brief Execute l'ecriture des JSON avec les paths calculés dans executeApis, avec les données de la Map et l'ID d'algorithme (pour écrire le nom de l'algorithme dans le fichier)
 * \param Map : La map à traiter
 * \param City* : Le chemin calculé par le(s) algorithme(s)
 * \param int : L'id de l'algorithme utilisé
 * \return Ecrit un fichier {nomfichier}_{algo}.json
 */

void printJSON(Map, City*, int);

#endif
