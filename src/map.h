/**
 * \file map.h
 * \brief Fichier d'en-tête de la structure Map.
 * \author David Phan, Jason Pindat
 * \version 0.28
 * \date 12/03/2014
 */

#ifndef MAP_H
#define MAP_H

#include "city.h"
#include "string.h"

typedef struct _Map * Map;

/** \fn Map mapCreate()
 *  \brief mapCreate crée une instance de Map (avec allocation mémoire)
 *  \return objet de type Map
 */

Map mapCreate();

/** \fn Str mapGetName(Map m)
 * \brief Retourne le nom de la map passée en paramètre
 * \param m Map que l'on traite
 * \return de type String
 */

Str mapGetName(Map);

/** \fn void mapSetName(Map, Str n)
 * \brief Modifie le nom d'une map
 * \param m Map que l'on souhaite modifier
 * \param n nom choisi de type String
 */

void mapSetName(Map, Str);

/** \fn City *mapGetPath(Map m, int i)
 * \brief Retourne une City stockée dans le tableau paths d'une Map
 * \param m Map contenant le tableau de City
 * \param i entier passé en index de paths désignant le chemin choisi
 * \return objet de type City
 */

City *mapGetPath(Map m, int i);

/** \fn void mapSetPath(Map m, int i, City *path)
 * \brief Modifie une des chemins d'une Map
 * \param m Map contenant le chemin
 * \param i entier passé en index de paths désignant le chemin choisi
 * \param path tableau de City représentant le nouveau chemin
 */

void mapSetPath(Map m, int i, City *path);

/** \fn double mapGetDuration(Map m, int i)
 * \brief Retourne le temps d'exécution de l'algorithme choisi grâce à l'entier i
 * \param m Map stockant le tableau duration
 * \param i Entier passé en index du tableau duration désignant l'algorithme
 * \return nombre de type double (précision)
 */

double mapGetDuration(Map, int);

/** \fn double mapSetDuration(Map m, int i, double val)
 * \brief Modifie le tableau duration
 * \param m Map stockant le tableau duration
 * \param i Entier passé en index du tableau duration désignant l'algorithme
 * \param val temps d'exécution de l'algorithme choisi
 */

void mapSetDuration(Map, int, double);

/** \fn double mapGetStartCity(Map m)
 * \brief Retourne la ville de départ de la Map
 * \param m Map contenant la ville de depart
 * \return retourne la ville de départ
 */

int mapGetStartCity(Map m);

/** \fn double mapSetStartCity(Map m, int val)
 * \brief Modifie la ville de départ
 * \param m Map contenant la ville de départ
 * \param val ville de départ choisie
 */

void mapSetStartCity(Map m, int val);

/** \fn void mapDelete(Map m)
 *  \brief Supprime et libère la mémoire utilisée par la Map passée en paramètre
 * \param m Map qu'il faut libérer
 */

void mapDelete(Map);

/** \fn mapDeleteRec(Map m)
 *  \brief supprime les instances de type City de l'objet Map passé en paramètre
 * \param m Map qui contient les objets de type City
 */

void mapDeleteRec(Map);

/** \fn void mapAddCity(Map m, City c)
 *  \brief Ajoute un objet de type City (par référence) à un objet de type Map
 * \param m Objet de type Map
 * \param c Objet de type City
 */

void mapAddCity(Map, City);

/** \fn City mapGetCity(Map m, int indice)
 *  \brief Retourne l'objet City par son indice indice dans la Map m
 * \param m Map qui contient la ville que l'on cherche
 * \param indice Entier qui définit le numéro de l'objet City dans l'objet Map
 * \return City objet de type City
 */

City mapGetCity(Map,int);

/** \fn void mapSetCity(Map m, City c, int indice)
 *  \brief Met l'objet City c dans l'objet Map avec l'indice indice
 * \param m Objet de type Map
 * \param c Objet de type City qui sera ajouté dans l'objet m par référence
 * \param indice Indice que prendra l'objet City dans l'objet Map
 */

void mapSetCity(Map,City, int);

/** \fn int mapGetSize(Map m)
 *  \brief Retourne le nombre de City que contient l'objet Map m
 * \param m L'objet de type Map
 * \return le nombre d'objet City
 */

int mapGetSize(Map);

 /** \fn bool mapGetIsPos(Map m)
 *  \brief retourne si la Map m est déterminée par des points ou seulement par une matrice de distance
 * \param m l'objet de type Map
 * \return TRUE lorsque Map est déterminée par des points, FALSE lorsque Map est déterminée par
 */

bool mapGetIsPos(Map m);

/** \fn void mapDataDump(Map m)
 *  \brief affiche les données d'une Map (pour le mode verbose)
 * \param m Objet de type Map
 */

void mapDataDump(Map);

/** \fn Map mapCreateFromPoints(Point* villes, int nbVilles)
 *  \brief Crée un objet de type Map avec un tableau de Point, et le nombre de Point
 * \param villes Objet de type Point* contenant les références de nbVilles Point
 * \param nbVilles nombre d'objet de type Point passés en paramètres
 * \param name nom du fichier tsp
 * \return Objet de type Map avec nbVilles objets City
 */

Map mapCreateFromPoints(Point*, int, Str);

/** \fn Map mapCreateRandom(int nbVilles)
 *  \brief Crée un objet de type Map avec nbVilles objets City avec des points générés aléatoirements
 * \param nbVilles nombre de villes que l'objet Map aura
 * \return un objet de type Map avec nbVilles City
 */

Map mapCreateRandom(int);

/** \fn void mapDraw(Map m)
 *  \brief Dessine sur le terminal l'objet de type Map
 * \param m l'objet de type Map
 */

void mapDraw(Map);

#endif
