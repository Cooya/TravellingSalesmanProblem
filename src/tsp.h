/**
 * \file tsp.h
 * \brief Fichier d'en-tête du lecteur de fichiers TSP.
 * \author Jason Pindat
 * \version
 * \date 2014
 */

#ifndef TSP_H
#define TSP_H

#include "map.h"
#include "string.h"

/**
 * \fn void throwTspErr(Str, int, Str)
 * \brief Fonction qui lance une erreur.
 * \param Str : Message d'erreur.
 * \param int : Ligne où se situe l'erreur.
 * \param Str : Message additionnel.
 * \return void
 */

void throwTspErr(Str, int, Str);

/**
 * \fn void throwTspWarn(Str, int, Str)
 * \brief Fonction qui lance un avertissement.
 * \param Str : Message d'erreur.
 * \param int : Ligne où se situe l'erreur.
 * \param Str : Message additionnel.
 * \return void
 */

void throwTspWarn(Str, int, Str);

 /**
 * \fn void freeBfrs(Str, double**, Point*, int)
 * \brief Fonction qui libère les buffers alloués lors de la lecture d'un fichier corrompu par tspLoad.
 * \param Str : Chaîne actuellement décodée.
 * \param double** : Matrice de distances.
 * \param Point* : Tableau des coordonnées des villes.
 * \param int : Nombre de villes.
 * \return void
 */
 
void freeBfrs(Str, double** , Point*, int);

/**
 * \fn Map tspLoad(Str)
 * \brief Fonction qui lit un fichier TSP et qui retourne une Map.
 * \param Str : Chaîne de caractères contenant le chemin du fichier à lire.
 * \return La Map à laquelle on appliquera les algorithmes.
 */

Map tspLoad(Str);

/**
 * \fn void tspWrite(Map, Str)
 * \brief Fonction qui écrit un fichier TSP à partir d'une Map.
 * \param Map : Map à écrire dans le fichier.
 * \param Str : Chaîne de caractères contenant le chemin du fichier à écrire.
 * \return void
 */

void tspWrite(Map, Str);

/**
 * \fn void tspOut(Map, City*, Str, Str)
 * \brief Fonction qui écrit un fichier TOUR à partir d'une Map et d'un chemin.
 * \param Map : Map à écrire dans le fichier.
 * \param City*: Chemin à écrire dans le fichier.
 * \param Str : Chaîne de caractères contenant le chemin du fichier à écrire.
 * \param Str : Commentaire.
 * \return void
 */

void tspOut(Map, City*, Str, Str);

#endif
