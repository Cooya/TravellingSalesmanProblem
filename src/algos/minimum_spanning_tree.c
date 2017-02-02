/**
 * \file minimum_spanning_tree.c
 * \brief Fichier implémentant l'algorithme Minimum Spanning Tree
 * \author Nicolas Marcy
 * \version
 * \date
 */

#include "../city.h"
#include "../point.h"
#include "../map.h"
#include "../fcts.h"
#include "tree.h"
#include "algos.h"
#include "minimum_spanning_tree.h"

/**
 * \fn City* minimumSpanningTree(Map m, City cityBegin)
 * \brief Fonction qui exécute l'algorithme Minimum Spanning Tree.
 * \param Map m : Map à laquelle est appliqué l'algorithme.
 * \param City cityBegin : Ville de départ et d'arrivée du chemin.
 * \return Un chemin sous la forme d'un tableau de City.
 */

City* minimumSpanningTree(Map m, City cityBegin)
{
    int nbCities=mapGetSize(m);
    City* path=arrCitiesCreate(nbCities+1);
    City current;
    double minDist;
    int iBegin; // premier sommet de l'arête
    int iEnd; // second sommet de l'arête
    int iArray[nbCities]; // tableau des villes placées dans l'arbre
    iArray[0]=cityGetIndex(cityBegin);
    int iCount=1; // nombre de villes placées dans l'arbre

    Tree T=createTree(nbCities);
    addTopToTree(T,-1,cityGetIndex(cityBegin)); // ajout de l'indice de la ville de départ dans le tableau

    while(iCount!=nbCities) // tant que tous les indices n'ont pas été inclus
    {
        iBegin=0;
        iEnd=0;
        while(!isInside(iArray,iCount,iBegin)) // on cherche un début d'arête qui est dans l'arbre
            iBegin++;
        while(isInside(iArray,iCount,iEnd)) // puis on cherche une fin d'arête qui n'est pas dans l'arbre
            iEnd++;
        minDist=cityGetDist(mapGetCity(m,iBegin),iEnd); // et on prend la distance entre ces 2 sommets, ce sera notre base de calcul de distance minimum
        for(int i=0; i<iCount; i++) // on parcourt les sommets inclus
        {
            current=mapGetCity(m,iArray[i]); // on cherche la plus petite distance dans la liste des distances du sommet courant
            for(int j=0; j<nbCities; j++)
                if(iArray[i]!=j && !isInside(iArray,iCount,j) && cityGetDist(current,j)<minDist) // si ce second indice n'est pas déjà inclus et que la distance entre l'indice courant et celui-ci est plus petite que la distance courante
                {
                    minDist=cityGetDist(current,j); // on met à jour la distance minimum et les deux sommets d'arête
                    iBegin=iArray[i];
                    iEnd=j;
                }
        }
        addTopToTree(T,iBegin,iEnd);
        iArray[iCount++]=iEnd;
    }

    upToRoot(T); // on remonte en haut de l'arbre pour traiter tous les sommets
    int treePath[nbCities];
    int* tmpPath=treePath; // utilisation d'un pointeur car treePath est un tableau statique
    fillIndexesArrayFromTree(T,&tmpPath); // on parcourt l'arbre en remplissant le tableau du path
    upToRoot(T); // on remonte encore pour tout libérer
    destroyTree(T);
    for(int i=0; i<nbCities; ++i)
        path[i]=mapGetCity(m,treePath[i]);
    path[nbCities]=cityBegin;
    return path;
}

