/**
 * \file bruteforce.c
 * \brief Fichier implémentant l'algorithme bruteforce en plusieurs versions.
 * \author David Phan
 * \version 2014
 * \date 2014
 */

#include <stdlib.h>

#include "../city.h"
#include "../point.h"
#include "../map.h"
#include "../fcts.h"
#include "bruteforce.h"
#include "algos.h"

#include <pthread.h> // pour le bruteforce multithread


/**
*	\struct mt_params
*	\brief structure d'envoi à un thread, cela contient la map, un tableau d'indices, un tableau de minimum, une longueur minimum
*
*/
struct mt_params // parametres à envoyer pour le multithreading
{
    Map m;
    int *indexesArr;
    int *minArr;
    double minLength;
};

/**
 * \fn static voic permut_bf(Map m, int *t, int iBegin, int size, double* minLength, int** minArr)
 * \brief fais la permutation
 * \param Map m : la map
 * \param int* t : tableau d'indices a permuter
 * \param int iBegin : l'indice de debut de permutation
 * \param int size : taille du tableau
 * \param double *minLength : pointeur de minLength
 * \param int** minArr : pointeur vers le tableau d'indice du min
 * \return
 */

static void permut_bf(Map m, int* t, int iBegin, int size , double* minLength, int** minArr)
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

            int tmp=t[i];
            t[i]=t[iBegin];
            t[iBegin]=tmp;
            //Permutation
            permut_bf(m, t,iBegin+1,size, minLength, minArr);
            t[iBegin]=t[i];
            t[i]=tmp;
        }
    }
}

/**
 * \fn City* bruteForce_recursif(Map m, City c)
 * \brief Renvoie le tableau de cities du tour optimal
 * \param Map m : l'instance
 * \param City c : la ville de depart
 * \return City * tableau de city
 */

City* bruteForce_recursif(Map m, City c)
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
    permut_bf(m,t,1,nbCities,&minLength,&minArr);

    City* arrCity=arrCitiesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        arrCity[i]=mapGetCity(m,minArr[i]);

    free(t);
    free(minArr);

    return arrCity;
}

/**
 * \fn City *bruteForce(Map m, City c)
 * \brief renvoie le chemin optimal
 * \param Map m : carte
 * \param City c : la ville de depart
 * \return tableau de City
 */

City *bruteForce(Map m, City c)
{
    int indiceStartCity=cityGetIndex(c);
    int nbCities=mapGetSize(m);
    int *path=arrIndexesCreate(nbCities+1);
    int *permut=arrIndexesCreate(nbCities-1);
    for(int i=0; i<nbCities+1; i++)
        path[i]=i; // init du tableau path de taille nbCities+1 [0,...,nbCities] pour pouvoir mettre sCity à la fin
    int tmpInd=path[0];
    path[0]=path[indiceStartCity]; // Mise du startCity en premier
    path[indiceStartCity]=tmpInd; // Echange du 0 avec le startCity
    path[nbCities]=indiceStartCity; // Mise du startCity en dernier (pour le retour)

    for(int i=0; i<nbCities-1; i++) // init du tableau permut de taille nbCities-1, qui prendra la partie à permuter de path path=[sCity,1,2,3,sCity], alors permut=[1,2,3]
        permut[i]=path[i+1];

    int *p=arrIndexesCreate(nbCities);
    p[0]=0;
    int *minArr=arrIndexesCreate(nbCities+1);
    for(int i=1; i<nbCities; i++) // le minArr est un tableau de taille nbCities+1, qui contient le contient les indices du parcours minimum, initialisé avec le parcours initial par ex avec sCity=0 (sCity, 1,2,3, sCity) + initialisation p à 0
    {
        minArr[i]=permut[i-1];
        p[i]=0;
    }
    minArr[0]=indiceStartCity;
    minArr[nbCities]=indiceStartCity;

    double minLength=calcPathLengthFromIndexesArr(m,minArr); // min Length initialisé avec le path de base

    // Permutations non ordonnées iteratives
    int i=1;
    int j,tmp;
    double length;
    while(i<nbCities-1)
    {
        if(p[i]<i)
        {
            if(i%2==1) // i impair
                j=p[i]; // j = p[i]
            else
                j=0;
            tmp=permut[j];
            permut[j]=permut[i]; // permutation
            permut[i]=tmp;

            length=0;
            for(int i=1; i<nbCities; i++)
                path[i]=permut[i-1]; // on met la permutation dans le path

            length=calcPathLengthFromIndexesArr(m, path); // Calcul de la longueur du chemin généré

            if(length<minLength)  // si la longueur du path est plus petite que celle en mémoire
            {
                minLength=length;
                for(int i=0; i<nbCities+1; i++) // modifier le minArr en mettant les valeurs du path
                    minArr[i]=path[i];
            }
            p[i]++;
            i=1;
        }
        else
        {
            p[i]=0;
            i++;
        }
    }
    City *arrCity=arrCitiesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        arrCity[i]=mapGetCity(m,minArr[i]);

    freeArrIndexes(path);
    freeArrIndexes(permut);
    freeArrIndexes(p);
    freeArrIndexes(minArr);

    return arrCity;
}

/**
 * \fn void *bruteforce_thread(void *params)
 * \brief C'est une fonction appellée par pthread pour la version multi threadee
 * \param pointeur vers la structure params_mt
 */

void *bruteforce_thread(void *params) // fonction du thread
{
    struct mt_params *param=(struct mt_params *)params;
    Map m=(Map)param->m;
    int nbCities=mapGetSize(m);
    int *permut=arrIndexesCreate(nbCities-2);
    for(int i=0; i<nbCities-2; i++) // init du tableau permut de taille nbCities-1, qui prendra la partie à permuter de path path=[sCity,1,2,3,sCity], alors permut=[1,2,3]
        permut[i]=param->indexesArr[i+2];

    int *p=arrIndexesCreate(nbCities);
    for(int i=0; i<nbCities; i++)
        p[i]=0;


    param->minLength=calcPathLengthFromIndexesArr(m,param->indexesArr);
    for(int i=0; i<nbCities+1; i++)
        param->minArr[i]=param->indexesArr[i];

    // Permutations non ordonnées iteratives
    int i=1;
    int j,tmp;
    double length;
    while(i<nbCities-2)
    {
        if(p[i]<i)
        {
            if(i%2==1) // i impair
                j=p[i]; // j = p[i]
            else
                j=0;
            tmp=permut[j];
            permut[j]=permut[i]; // permutation
            permut[i]=tmp;

            length=0;
            for(int i=2; i<nbCities; i++)
                param->indexesArr[i]=permut[i-2]; // on met la permutation dans le path

            length=calcPathLengthFromIndexesArr(m, param->indexesArr); // Calcul de la longueur du chemin généré

            if(length<param->minLength)  // si la longueur du path est plus petite que celle en mémoire
            {
                param->minLength=length;
                for(int i=0; i<nbCities+1; i++) // modifier le minArr en mettant les valeurs du path
                    param->minArr[i]=param->indexesArr[i];
            }
            p[i]++;
            i=1;
        }
        else
        {
            p[i]=0;
            i++;
        }
    }
    freeArrIndexes(permut);
    freeArrIndexes(p);
    return NULL;
}

/**
 * \fn City *bruteForce_mt(Map m, City c)
 * \brief Renvoie le chemin optimal
 * \param Map m la carte
 * \param City c : la ville de depart
 * \return un tableau de City
 */

City *bruteForce_mt(Map m, City c) // fonction qui lance les threads
{
    int indiceStartCity=cityGetIndex(c);
    int nbCities=mapGetSize(m);
    pthread_t thread[nbCities-1];

    if(nbCities==1)
    {
        City *arrCity=arrCitiesCreate(2);
        arrCity[0]=mapGetCity(m,0);
        arrCity[1]=arrCity[0];
        return arrCity;
    }


    int *path=arrIndexesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        path[i]=i; // init du tableau path de taille nbCities+1 [0,...,nbCities] pour pouvoir mettre sCity à la fin
    int tmpInd=path[0];
    path[0]=path[indiceStartCity]; // Mise du startCity en premier
    path[indiceStartCity]=tmpInd; // Echange du 0 avec le startCity
    path[nbCities]=path[0]; // Mise du startCity en dernier (pour le retour)

    struct mt_params *params=malloc(sizeof(struct mt_params)*(nbCities-1)); // encapsuler l'allocation
    for(int i=0; i<nbCities-1; i++)
    {
        params[i].m=m;
        params[i].indexesArr=arrIndexesCreate(nbCities+1); //calloc(nbCities+1,sizeof(int));
        params[i].minArr=arrIndexesCreate(nbCities+1); //calloc(nbCities+1,sizeof(int));
        for(int j=0; j<nbCities+1; j++)
        {
            params[i].indexesArr[j]=path[j];
            params[i].minArr[j]=0;
        }
        params[i].minLength=0;
    }
    for(int i=1; i<nbCities-1; i++)
    {
        int tmp=params[i].indexesArr[1];
        params[i].indexesArr[1]=params[i].indexesArr[i+1];
        params[i].indexesArr[i+1]=tmp;
    }

    for(int i=0; i<nbCities-1; i++)
        pthread_create(&thread[i], NULL, bruteforce_thread, &params[i]);

    for(int i=0; i<nbCities-1; i++)
        pthread_join(thread[i],NULL);

    int iMinArr=0;
    for(int i=0; i<nbCities-2; i++)
    {
        if(params[i+1].minLength<params[iMinArr].minLength)
            iMinArr=i+1;
    }

    City *arrCity=arrCitiesCreate(nbCities+1);
    for(int i=0; i<nbCities+1; i++)
        arrCity[i]=mapGetCity(m,params[iMinArr].minArr[i]);

    free(path);
    for(int i=0; i<nbCities-1; i++)
    {
        free(params[i].indexesArr);
        free(params[i].minArr);
    }
    free(params);


    return arrCity;
}


