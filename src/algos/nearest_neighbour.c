/**
 * \file nearest_neighbour.c
 * \brief Fichier implémentant l'algorithme Nearest Neighbour.
 * \author Jason Pindat, Nicolas Marcy
 * \version
 * \date
 */

#include <stdlib.h>
#include <stdio.h>

#include "../city.h"
#include "../point.h"
#include "../map.h"
#include "../fcts.h"
#include "algos.h"
#include "nearest_neighbour.h"

/**
 * \fn City* nearestNeighbour(Map m, City c)
 * \brief Fonction qui exécute l'algorithme Nearest Neighbour.
 * \param Map m : Map à laquelle est appliqué l'algorithme.
 * \param City c : Ville de départ et d'arrivée du chemin.
 * \return Un chemin sous la forme d'un tableau de City.
 */

City* nearestNeighbour(Map m, City c) // complexité en temps : n² - n
{
    int nbCities=mapGetSize(m);
    City *path=arrCitiesCreate(nbCities+1);
    int visited[nbCities]; // tableau des villes exclues

    int tmpIndex;
    City currCity; // ville actuelle
    double edge;

    path[0]=c;
    visited[0]=cityGetIndex(c); // indice de la ville de départ
    currCity=c;

    for(int i=1; i<nbCities; i++)
    {
        int indexTest=0;
        while(cityEquals(currCity, mapGetCity(m, indexTest)) || isInside(visited, i, indexTest)) // choix d'une ville qui servira pour le calcul des distances
            indexTest++;

        if(indexTest>=nbCities)
            return path;

        edge=cityGetDist(currCity, indexTest);
        tmpIndex=indexTest;

        for(int j=0; j<nbCities; j++)
        {
            if(!cityEquals(currCity, mapGetCity(m, j)) && !isInside(visited, i, j))
                if(cityGetDist(currCity, j)<edge)
                {
                    edge=cityGetDist(currCity, j);
                    tmpIndex=j;
                }
        }
        currCity=mapGetCity(m, tmpIndex);
        visited[i]=tmpIndex;
        path[i]=currCity;
    }
    path[nbCities]=c;
    return path;
}

/**
 * \fn City* nearestNeighbour2(Map m, City cityBegin)
 * \brief Fonction qui exécute une seconde version de l'algorithme Nearest Neighbour.
 * \param Map m : Map à laquelle est appliquée l'algorithme.
 * \param City cityBegin : Ville de départ et d'arrivée du chemin.
 * \return Un chemin sous la forme d'un tableau de City.
 */

City* nearestNeighbour2(Map m, City cityBegin) // seconde version du NN
{
    int nbCities=mapGetSize(m);
    City* path=arrCitiesCreate(nbCities+1);
    double minDist;
    int currentIndex;
    int iArray[nbCities]; // tableau des indices inclues des villes placées dans le chemin
    int iCount=0; // nombre de villes placées dans le chemin
    path[0]=cityBegin;
    iArray[0]=cityGetIndex(cityBegin);
    while(iCount<nbCities) // tant qu'on a pas ajouté toutes les villes dans le tableau des indices inclues
    {
        currentIndex=0;
        while(isInside(iArray,iCount,currentIndex)) // on cherche le premier indice qui n'est pas dans le tableau des indices inclues
            currentIndex++;
        minDist=cityGetDist(mapGetCity(m,currentIndex),iArray[iCount]); // quand on l'a, on prend la distance entre lui et le dernier indice ajouté, ce sera notre base pour calculer la distance minimum
        for(int i=0; i<nbCities; ++i)
            if(iArray[iCount]!=i && !isInside(iArray,iCount,i) && cityGetDist(mapGetCity(m,i),iArray[iCount])<minDist) // si la ville n'a pas déjà été inclue et que sa distance avec la dernière ajoutée est inférieure à la distance stockée
            {
                minDist=cityGetDist(mapGetCity(m,i),iArray[iCount]);
                currentIndex=i;
            }
        path[iCount+1]=mapGetCity(m,currentIndex); // ajout dans le path
        iArray[++iCount]=currentIndex; // et ajout dans le tableau des indices inclue
    }
    path[nbCities]=cityBegin;
    return path;
}
