/**
 * \file algos.h
 * \brief Fichier d'en-tête de l'exécution des algorithmes
 * \author Jason Pindat
 * \version 2014
 * \date 2014
 */

#ifndef ALGOS_H
#define ALGOS_H

#include "../city.h"
#include "../map.h"

#define NB_ALGOS 8

/** \fn void initAlgos()
 *
 * \brief "remplit" les tableaux fcts et names de la structure algos
 *
 */
void initAlgos();
/** \fn void printTest(int nbCities, int nbTests, bool* algosSelected)
 *
 * \brief A pour rôle d'afficher le test des algorithmes choisis (temps d'exécution,
                                                                    longueur du chemin)
 *
 * \param nbCities, le nombre de villes calculé à partir de la taille de la map sur laquelle
 * se porte l'algorithme
 * \param nbTests
 * \param algosSelected, tableau de booléens où les indices valent "true" si l'algorithme associé
 * doit être testé, "false" sinon
 *
 */
void printTest(int, int, bool*);
/** \fn void printAlgoResult(Map m, City *path)
 *
 * \brief Affiche le chemin solution de l'algorithme
 * \param m Map choisie pour la résolution de l'algorithme
 * \param path, tableau solution de l'algorithme contenant les indices des villes ordonnées.
 *
 */
void printAlgoResult(Map m, City *path);
/** \fn executeAlgos(Map m, bool *algosSelected, int startC, bool graphics, bool tour)
 *
 * \brief
 * \param m Map choisie pour la résolution de l'algorithme
 * \param algosSelected, tableau de booléens où les indices valent "true" si l'algorithme associé
 * doit être testé, "false" sinon.
 * \param startC, ville de départ pour le calcul de l'algorithme
 * \param graphics, booléen : "true" si le mode graphique est lancé, "false" sinon.
 * \param tour, booléen
 *
 */
void executeAlgos(Map m, bool *algos, int startC, bool graphics, bool tour);
/** \fn double calcPathLength(Map m, City *path)
 *
 * \brief calcule la longueur du chemin de la solution à partir d'un tableau de City
 * \param m Map choisie pour la résolution de l'algorithme
 * \param path, tableau de City ordonnées
 * \return distance de type double
 *
 */
double calcPathLength(Map, City *);
/** \fn double calcPathLengthFromIndexesArr(Map m, int *indArr)
 *
 * \brief calcule la longueur du chemin de la solution à partir d'un tableau d'entiers
 * \param m, Map choisie pour la résolution de l'algorithme
 * \param indexArr, tableau d'int
 *
 * \return distance de type double
 *
 */
double calcPathLengthFromIndexesArr(Map, int *);
/** \fn bool isInside(int *res_indice, int lim, int index)
 *
 * \brief indique si le tableau passé en paramètre contient une valeur choisie
 * \param res_indice, tableau d'entiers sur lequel fait la recherche
 * \param lim, taille de res_indice
 * \param index, entier sur lequel on fait la recherche
 *
 * \return Retourne true si index est dans le tableau, false sinon.
 *
 */
bool isInside(int *res_indice, int lim, int index);

#endif
