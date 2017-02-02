/**
 * \file gui/gui.h
 * \brief Fichier d'en-tête de l'interface graphique.
 * \author Jason Pindat
 * \version 0.64
 * \date 26/03/2014
 */

#ifndef GUI_H
#define GUI_H

#include "../map.h"
#include "../point.h"
#include "../string.h"

/**
 * \fn void guiInit(int* argc, char ***argv)
 * \brief Fonction d'initialisation des tampons pour l'affichage graphique
 * \param int *argc et char ***argv adresses respectives des paramètres de la fonction main
 * \return void
 */

void guiInit(int*, char***);

/**
 * \fn void guiFreeMap()
 * \brief Libère le tampon de stockage des coordonnées de villes
 * \param void
 * \return void
 */

void guiFreeMap();

/**
 * \fn void guiFreePaths()
 * \brief Libère le tampon de stockage des coordonnées de chemins
 * \param void
 * \return void
 */

void guiFreePaths();

/**
 * \fn void guiFreeLabels()
 * \brief Libère le tampon de stockage des labels
 * \param void
 * \return void
 */

void guiFreeLabels();

/**
 * \fn void guiFree()
 * \brief Libère tous les tampons de stockage
 * \param void
 * \return void
 */

void guiFree();

/**
 * \fn void guiStop()
 * \brief Libère tous les tampons de stockage et l'interface graphique
 * \param void
 * \return void
 */

void guiStop();

/**
 * \fn void guiAddPoint(Point pos, Str name)
 * \brief Ajoute un point dans le tampon des coordonnées
 * \param Point pos : Coordonnées de la ville
 * \param Str name : Nom de la ville
 * \return void
 */

void guiAddPoint(Point, Str);

/**
 * \fn void guiAddLine(Point pos1, Point pos2, double l)
 * \brief Ajoute une flèche dans le tampon des chemins
 * \param Point pos1 : Coordonnées de départ de la flèche
 * \param Point pos2 : Coordonnées d'arrivée de la flèche
 * \param double l   : longueur de la flèche
 * \return void
 */

void guiAddLine(Point, Point, double);

/**
 * \fn void guiAddLabel(Str str)
 * \brief Ajoute un texte dans le tampon des labels
 * \param char *str : Texte du label
 * \return void
 */

void guiAddLabel(Str);

/**
 * \fn void guiAddMap(Map m)
 * \brief Ajoute toutes les coordonnées des villes d'une Map dans le tampon de coordonnées
 * \param Map m : Map à ajouter
 * \return void
 */

void guiAddMap(Map);

/**
 * \fn void guiAddPath(Map m, City *path, double exTime)
 * \brief Ajoute toutes les flèches d'un chemin ainsi que les labels de la distance totale et du temps d'éxecution
 * \param Map m : Map contenant les villes du parcours
 * \param City *path : liste ordonnées de villes décrivant le chemin
 * \param double exTime : Temps d'execution
 * \return void
 */

void guiAddPath(Map, City*, double);

/**
 * \fn void guiDraw()
 * \brief Dessine tout ce qui se trouve dans les tampons
 * \param void
 * \return void
 */

void guiDraw();

/**
 * \fn void guiShow()
 * \brief Affiche la fenêtre et dessine ce qui se trouve dans les tampons
 * \param void
 * \return void
 */

void guiShow();

/**
 * \fn void guiShow()
 * \brief Affiche le logo du groupe avant de lancer la GUI
 * \param char* image_name : Nom de l'image à charger.
 * \param int time : Durée pendant laquelle l'image est affichée.
 * \param int width : Largeur de l'image.
 * \param int height : Longueur de l'image.
 * \return void
 */

void showSplashScreen(char*,int,int,int);

#endif

