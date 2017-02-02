/**
 * \file point.h
 * \brief Structure Coords
 * \author David Phan, Jason Pindat
 * \version 0.27
 * \date 24/03/2014
 *
 * Structure Coords
 *
 */

#ifndef POINT_H
#define POINT_H

#include <stdbool.h>

#define MANHATTAN 1
#define EUCLIDIAN 0


/** \struct Coords
 * \brief Structure Coords (coordonnées) d'un Point par son abscisse x et son ordonnée y
 */

typedef struct Coords
{
    double x; /*!< Abscisse. */
    double y; /*!< Ordonnée. */
} Coords;

typedef Coords Point;

/** \fn void setLengthType(bool val)
 *  \brief Met le type de longueur pour le programme
 *  \param val Bool, val==1 => longueur de manhattan, val==0 => longueur euclidienne
 *  si on veut des longueurs Manhattan val est à 1, si on veut des longueurs Euclidienne, val est à 0
 */

void setLengthType(bool) ;

/** \fn double lengthEuc(Point p1, Point p2)
 *  \brief renvoie la distance Euclidienne de deux points
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Double qui est la distance euclidienne entre deux points
 */

double lengthEuc(Point, Point);

/** \fn double lengthMan(Point p1, Point p2)
 *  \brief renvoie la distance Manhattan de deux points
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Double qui est la distance de manhattan entre deux points
 */

double lengthMan(Point, Point);

/** \fn double length(Point p1, Point p2)
 *  \brief renvoie la distance de deux points selon le bool lengthType
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Double qui est la distance entre deux points
 */

double length(Point, Point);

/** \fn double pointGetX(Point p)
 *  \brief retourne l'abscisse du Point p
 * \param p Objet de type Point
 * \return L'abscisse du Point
 */

double pointGetX(Point);

/** \fn double pointGetY(Point p)
 *  \brief retourne l'ordonnée du Point p
 * \param p Objet de type Point
 * \return L'ordonnée du Point
 */

double pointGetY(Point);

/** \fn bool pointEquals(Point p1, Point p2)
 *  \brief retourne l'égalité de deux points
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Egalité des deux points
 */

bool pointEquals(Point, Point);

/** \fn Point point(double x, double y)
 *  \brief Crée un point avec les coordonnées passées en paramètre.
 *  \param double x : Abcisse du point.
 *  \param double y : Ordonnée du point.
 *  \return Retourne un point.
 */

Point point(double x, double y);

#endif
