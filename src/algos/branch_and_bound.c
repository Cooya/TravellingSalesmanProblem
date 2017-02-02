/**
 * \file branch_and_bound.c
 * \brief Fichier implémentant l'algorithme Branch and Bound
 * \author David Phan
 * \version 2014
 * \date 2014
 */

#include <stdlib.h>
#include "../city.h"
#include "../point.h"
#include "../map.h"
#include "../fcts.h"
#include "algos.h"
#include "branch_and_bound.h"
#include "nearest_neighbour.h"
#include "minimum_spanning_tree.h"

/**
 * \fn static voic permut_bb(Map m, int *t, int iBegin, int size, double* minLength, int** minArr)
 * \brief fais la permutation
 * \param Map m : la map
 * \param int* t : tableau d'indices a permuter
 * \param int iBegin : l'indice de debut de permutation
 * \param int size : taille du tableau
 * \param double *minLength : pointeur de minLength
 * \param int** minArr : pointeur vers le tableau d'indice du min
 * \return void
 */
static void permut_bb(Map m, int* t, int iBegin, int size , double* minLength, int** minArr)
{
    double length=0;
    if(iBegin==size-1) // Nous sommes arrivés à une feuille (permutation complete)
    {
        length=calcPathLengthFromIndexesArr(m,t); // calcul de la permutation
        if(length<*minLength)  // Comparaison
        {
            *minLength=length;
            for(int i=0; i<size; i++)
                (*minArr)[i]=t[i]; // Mise en memoire si < minLength
        }
    }
    else
    {
        for(int i=iBegin; i<size; i++)
        {
            length=0;
            //Calculer distance jusqu'a iBegin, comparer vec minlength
            for(int i=0; i<iBegin; i++)
                length+=cityGetDist(mapGetCity(m, t[i]),t[i+1]);

            if(length>*minLength) // pas la peine d'aller plus loin dans la branche --  bound
                break;

            int tmp=t[i];
            t[i]=t[iBegin];
            t[iBegin]=tmp;
            //Permutation
            permut_bb(m, t,iBegin+1,size, minLength, minArr);
            t[iBegin]=t[i];
            t[i]=tmp;
        }
    }
}


/**
 * \fn City *branchAndBound(Map m, City c)
 * \brief Renvoie le chemin optimal
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */
City* branchAndBound(Map m, City c)
{

    int nbCities=mapGetSize(m);
    int sCity=cityGetIndex(c);
    int *t=arrIndexesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        t[i]=i;
    int t_tmp=t[sCity];
    t[sCity]=t[0];
    t[0]=t_tmp;
    t[nbCities]=sCity;

    double minLength=calcPathLengthFromIndexesArr(m,t);
    int* minArr=arrIndexesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        minArr[i]=t[i];
    permut_bb(m,t,1,nbCities,&minLength,&minArr);


    City* arrCity=arrCitiesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        arrCity[i]=mapGetCity(m,minArr[i]);

    free(t);
    free(minArr);

    return arrCity;
}
/**
 * \fn City *branchAndBoundNNMST(Map m, City c)
 * \brief Renvoie le chemin optimal
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */
City* branchAndBoundNNMST(Map m, City c)
{

    int nbCities=mapGetSize(m);
    int sCity=cityGetIndex(c);
    int *t=arrIndexesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++) t[i]=i;
    int t_tmp=t[sCity];
    t[sCity]=t[0];
    t[0]=t_tmp;
    t[nbCities]=sCity;

    int *minArr=arrIndexesCreate(nbCities+1);

    ///City* minArrNN=arrCitiesCreate(nbCities+1);
    City* minArrNN=nearestNeighbour(m, c);
    double minLength=calcPathLength(m,minArrNN);

    //City *minArrMST=arrCitiesCreate(nbCities+1);
    City* minArrMST=minimumSpanningTree(m, c);
    if(calcPathLength(m,minArrMST)<minLength)
    {
        freeArrCities(minArrNN);
        for(int i=0; i<nbCities+1; i++) minArr[i]=cityGetIndex(minArrMST[i]);
        minLength=calcPathLength(m, minArrMST);
        freeArrCities(minArrMST);
    }
    else
    {
        freeArrCities(minArrMST);
        for(int i=0; i<nbCities+1; i++) minArr[i]=cityGetIndex(minArrNN[i]);
        minLength=calcPathLength(m, minArrNN);
        freeArrCities(minArrNN);
    }

    permut_bb(m,t,1,nbCities,&minLength,&minArr);

    City* arrCity=arrCitiesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++) arrCity[i]=mapGetCity(m,minArr[i]);

    free(t);
    free(minArr);

    return arrCity;
}


