/**
 * \file city.c
 * \brief Structure City et ses fonctions d'instanciation, de suppression et d'affichage
 * \author David Phan, Jason Pindat
 * \version 0.48
 * \date 24/03/2014
 */

#include <stdlib.h>
#include <stdio.h>

#include "city.h"
#include "fcts.h"

/** \struct City
 *  \brief Structure City qui est défini par des distances (et sa position si isPos).
 */

struct _City
{
    bool isPos; /*!< Booléen qui vaut true si la ville possède des coordonnées cartésiennes. */
    Point pos; /*!< Position de la ville sur la map. */
    double *dists; /*!< Tableau de distances avec les autres. */
    int distsSize; /*!< Nombre de distances (c'est-à-dire nombre de villes - 1). */
    int index; /*!< Index de la ville (compris entre 0 et le nombre de villes - 1) */
};

/** \fn City cityCreate(bool isPos, Point pos, double *distsBuffer, int distsSize)
 *  \brief Crée l'objet de type City par rapport à la matrice de Distance
 * \param isPos est à TRUE si City est défini par un Point
 * \param pos Point qui donne la position de la City
 * \param distsBuffer Tableau de distance
 * \param distsSize nombre de Distances du tableau distsBuffer
 * \return Objet de type City
 */

City cityCreate(bool isPos, Point pos, double *distsBuffer, int distsSize)
{
    throwMsg("City", "Creating city...");

    City c = malloc(sizeof(struct _City));

    c->isPos=isPos;
    c->pos=pos;
    c->dists=malloc(distsSize*sizeof(double));

    for(int i=0; i<distsSize; i++)
        c->dists[i]=distsBuffer[i];

    c->distsSize=distsSize;
    c->index=-1;

    return c;
}

/** \fn void cityDelete(City c)
 *  \brief Libère l'objet de type City
 * \param c Objet de type City
 */

void cityDelete(City c)
{
    throwMsg("City", "Deleting city...");
    free(c->dists);
    free(c);
}

/** \fn double cityGetDist(City c, int i)
 *  \brief Renvoie la distance d'une City par rapport à une autre (par son indice)
 * \param c Objet de type City
 * \param i Indice de la deuxième City
 * \return Distance entre la City c et la City d'indice i
 */

double cityGetDist(City c, int i)
{
    if(i<0 || i>=c->distsSize)
        throwErr("City", "Distance index out of range (cityGetDist)", NULL);
    return c->dists[i];
}

/** \fn int cityGetDistsSize(City c)
 *  \brief Retourne le nombre de distance qu'a l'objet City
 * \param c Objet de type City
 * \return Nombre de distance
 */

int cityGetDistsSize(City c)  // retourne le nombre de distances
{
    return c->distsSize;
}

/** \fn Point cityGetPos(City c)
 *  \brief Retourne l'objet Point si défini par un Point de l'objet City c
 * \param c Objet de type City
 * \return Objet de type Point
 */

Point cityGetPos(City c)
{
    if(!c->isPos)
        throwErr("City", "The city has no position", NULL);
    return c->pos;
}

/** \fn bool cityGetIsPos(City c)
 *  \brief Retourne si l'objet de type City est défini par un objet de type Point
 * \param c Objet de type City
 * \return TRUE si elle est définie par un Point, FALSE si elle n'est définie que par un tableau de Distance
 */

bool cityGetIsPos(City c)
{
    return c->isPos;
}

/** \fn int cityGetIndex(City c)
 *  \brief Retourne l'indice de la ville
 * \param c Objet de type City
 * \return L'indice auquel est cette ville dans sa Map
 */

int cityGetIndex(City c) {
    return c->index;
}

/** \fn int citySetIndex(City c, int val)
 *  \brief Met à jour l'indice de la ville dans la Map
 * \param c Objet de type City
 * \param val entier : indice de la ville
 */

void citySetIndex(City c, int val) {
    c->index=val;
}

/** \fn bool cityEquals(City c1, City c2)
 *  \brief Retourne si deux objets City sont égales
 * \param c1 Objet de type City
 * \param c2 Objet de type City
 * \return TRUE si les tableaux de Distances de c1 et c2 sont les mêmes, FALSE sinon
 */

bool cityEquals(City c1, City c2)
{
    return c1==c2;
    //return c1->dists==c2->dists;
}

/** \fn void cityDataDump(City c)
 *  \brief Affiche sur le terminal les données de l'objet City ( Position s'il y a , Tableau de distances relative à ce point)
 * \param c Objet de type City
 */

void cityDataDump(City c)  // fonction pour afficher les coordonnées de la ville ainsi que les distances avec les autres villes
{
    if(c->isPos)
        printf("X=%f Y=%f \n",pointGetX(cityGetPos(c)), pointGetY(cityGetPos(c))); // coordonnées
    else
        printf("No position\n");

    printf("Dists[] : ");
    for(int i=0; i<cityGetDistsSize(c); i++)
    {
        printf(" %f",cityGetDist(c,i)); // distances
    }
    printf("\n");
}
