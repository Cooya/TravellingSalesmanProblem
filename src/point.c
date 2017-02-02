/**
 * \file point.c
 * \brief Structure Point et ses fonctions
 * \author David Phan, Jason Pindat
 * \version 0.48
 * \date 24/03/2014
 *
 * Structure City et ses fonctions d'instanciation, de suppression et d'affichage
 *
 */

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "point.h"

bool lengthType;

/** \fn void setLengthType(bool val)
 *  \brief Met le type de longueur pour le programme
 *  \param val Bool, val==1 => longueur de manhattan, val==0 => longueur euclidienne
 *  si on veut des longueurs Manhattan val est à 1, si on veut des longueurs Euclidienne, val est à 0
 */

void setLengthType(bool val)   // fonction pour sélectionner le type de distance (euclidienne ou Manhattan)
{
    lengthType=val;
}

/** \fn double lengthEuc(Point p1, Point p2)
 *  \brief renvoie la distance Euclidienne de deux points
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Double qui est la distance euclidienne entre deux points
 */

double lengthEuc(Point p1, Point p2)   // distance euclidienne
{
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}
/** \fn double lengthMan(Point p1, Point p2)
 *  \brief renvoie la distance Manhattan de deux points
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Double qui est la distance de manhattan entre deux points
 */
double lengthMan(Point p1, Point p2)   // distance de Manhattan
{
    return abs((int)(p2.x-p1.x))+abs((int)(p2.y-p1.y));
}

/** \fn double length(Point p1, Point p2)
 *  \brief renvoie la distance de deux points selon le bool lengthType
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Double qui est la distance entre deux points
 */
double length(Point p1, Point p2)
{
    if (lengthType)
        return lengthMan(p1, p2);
    else
        return lengthEuc(p1,p2);
}

/** \fn double pointGetX(Point p)
 *  \brief retourne l'abscisse du Point p
 * \param p Objet de type Point
 * \return L'abscisse du Point
 */

double pointGetX(Point p)
{
    return p.x;
}

/** \fn double pointGetY(Point p)
 *  \brief retourne l'ordonnée du Point p
 * \param p Objet de type Point
 * \return L'ordonnée du Point
 */

double pointGetY(Point p)
{
    return p.y;
}

/** \fn bool pointEquals(Point p1, Point p2)
 *  \brief retourne l'égalité de deux points
 * \param p1 Objet de type Point
 * \param p2 Objet de type Point
 * \return Egalité des deux points
 */

bool pointEquals(Point p1, Point p2)
{
    return p1.x==p2.x && p1.y==p2.y;
}

/** \fn Point point(double x, double y)
 *  \brief Crée un point avec les coordonnées passées en paramètre.
 *  \param double x : Abcisse du point.
 *  \param double y : Ordonnée du point.
 *  \return Retourne un point.
 */

Point point(double x, double y)
{
    Point p;
    p.x=x;
    p.y=y;
    return p;
}
