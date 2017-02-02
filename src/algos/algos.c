/**
 * \file algos.c
 * \brief Fichier implémentant la gestion des algos
 * \author Jason Pindat
 * \version 2014
 * \date 2014
 */

#include <stdlib.h>
#include <stdio.h>

#include "../city.h"
#include "../point.h"
#include "../map.h"
#include "../fcts.h"
#include "algos.h"
#include "../gui/gui.h"
#include "nearest_neighbour.h"
#include "bruteforce.h"
#include "branch_and_bound.h"
#include "minimum_spanning_tree.h"
#include "branch_and_bound_hk.h"
#include "../tsp.h"

/** \struct algos
 *
 * \brief Utilisée pour faciliter l'implémentation de ce module, elle permet de gérer tous les algorithmes en stockant
 *  les données necessaires à la réalisation et l'affichage des tests.
 *
 */

struct _algos
{
    City*(*fcts[NB_ALGOS])(Map, City);
    Str names[NB_ALGOS];
} algos;

/** \fn void initAlgos()
 *
 * \brief "remplit" les tableaux fcts et names de la structure algos
 *
 */

void initAlgos()
{
    algos.fcts[0]=&nearestNeighbour;
    algos.fcts[1]=&minimumSpanningTree;
    algos.fcts[2]=&bruteForce;
    algos.fcts[3]=&branchAndBound;
    algos.fcts[4]=&bruteForce_recursif;
    algos.fcts[5]=&bruteForce_mt;
    algos.fcts[6]=&branchAndBoundNNMST;
    algos.fcts[7]=&branchAndBoundHK;
    algos.names[0]="Nearest Neighbour";
    algos.names[1]="Minimum Spanning Tree";
    algos.names[2]="Iterative Brute Force";
    algos.names[3]="Branch and Bound";
    algos.names[4]="Recursive Bruteforce";
    algos.names[5]="Multi-threaded BruteForce";
    algos.names[6]="Branch and Bound with NN-MST relaxation";
    algos.names[7]="Branch and Bound with Held Karp relaxation";
}

/** \fn void printTest(int nbCities, int nbTests, bool* algosSelected)
 *
 * \brief A pour rôle d'afficher le test des algorithmes choisis (temps d'exécution,
                                                                    longueur du chemin)
 *
 * \param nbCities, le nombre de villes calculé à partir de la taille de la map sur laquelle
 * se porte l'algorithme
 * \param nbTests
 * \param algosSelected, tableau de booléens où les indices valent "true" si l'algorithme associé
 * doit être testé, "false" sinon
 *
 */

void printTest(int nbCities, int nbTests, bool* algosSelected)
{
    Map m;
    City* path;
    double sums[NB_ALGOS*2];
    for(int i=0; i<NB_ALGOS*2; ++i)
        sums[i]=0;

    for(int i=0; i<nbTests; ++i)
    {
        m=mapCreateRandom(nbCities);
        for(int i=0,j=0; i<NB_ALGOS; ++i, j+=2)
            if(algosSelected[i])
            {
                clockBegin();
                path=algos.fcts[i](m,mapGetCity(m,0));
                sums[j+1]+=clockStop();
                sums[j]+=calcPathLength(m, path);
                freeArrCities(path);
            }
        mapDeleteRec(m);
    }


    printf("Results for %d tests with %d cities : \n",nbTests,nbCities);
    for(int i=0,j=0; i<NB_ALGOS; ++i, j+=2)
        if(algosSelected[i])
            printf("%s : %.2f (%.3f ms)\n",algos.names[i],sums[j]/nbTests,sums[j+1]/nbTests);
    printf("\n");
}

/** \fn void printAlgoResult(Map m, City *path)
 *
 * \brief Affiche le chemin solution de l'algorithme
 * \param m Map choisie pour la résolution de l'algorithme
 * \param path, tableau solution de l'algorithme contenant les indices des villes ordonnées.
 *
 */

void printAlgoResult(Map m, City *path)
{
    for(int i=0; i<mapGetSize(m)+1; i++)
    {
        if(i!=0)
            printf("-> ");
        printf("%d ", cityGetIndex(path[i])+1); // nos indices commencent à 0, il faut donc incrémenter par 1
    }
    printf("\n");
    printf("Total Length: %f\n", calcPathLength(m, path));
}

/** \fn executeAlgos(Map m, bool *algosSelected, int startC, bool graphics, bool tour)
 *
 * \brief
 * \param m Map choisie pour la résolution de l'algorithme
 * \param algosSelected, tableau de booléens où les indices valent "true" si l'algorithme associé
 * doit être testé, "false" sinon.
 * \param startC, ville de départ pour le calcul de l'algorithme
 * \param graphics, booléen : "true" si le mode graphique est lancé, "false" sinon.
 * \param tour, booléen
 *
 */

void executeAlgos(Map m, bool *algosSelected, int startC, bool graphics, bool tour)
{
    if(mapGetSize(m)<2) {
        printf("2 Cities is the minimum.");
        return;
    }

    if(startC!=mapGetStartCity(m))
        for(int i=0; i<NB_ALGOS; i++)
        {
            if(mapGetPath(m, i))
            {
                free(mapGetPath(m, i));
                mapSetPath(m, i, NULL);
            }
        }

    mapSetStartCity(m, startC);

    for(int i=0; i<NB_ALGOS; i++)
    {
        if(algosSelected[i])
        {
            if(!mapGetPath(m, i))
            {
                clockBegin();
                mapSetPath(m, i, algos.fcts[i](m, mapGetCity(m, startC)));
                mapSetDuration(m, i, clockStop());
            }

            printf("%s Result: (%.3f ms)\n", algos.names[i], mapGetDuration(m, i));
            printAlgoResult(m, mapGetPath(m, i));

            if(graphics)
                guiAddPath(m, mapGetPath(m, i), mapGetDuration(m, i));
        }
    }

    bool atleast1=false;

    for(int i=0; i<NB_ALGOS; i++)
    	if(mapGetPath(m, i)) {
    	    atleast1=true;
    	    break;
    	}

    if(!atleast1)
    	tour=false;

    if(tour)
    {
        bool initI=false;
        int minI=-1;
        double lengths[NB_ALGOS];

        for(int i=0; i<NB_ALGOS; i++)
        {
            if(mapGetPath(m, i))
            {
                lengths[i]=calcPathLength(m, mapGetPath(m, i));

                if(!initI)
                {
                    minI=i;
                    initI=true;
                }
                else if(lengths[i]<lengths[minI])
                    minI=i;
            }
        }

        Str name=mapGetName(m);
        int le=strLength(name);

        Str newname=malloc((le+6)*sizeof(char));

        for(int i=0; i<le; i++) newname[i]=name[i];

        newname[le++]='.';
        newname[le++]='t';
        newname[le++]='o';
        newname[le++]='u';
        newname[le++]='r';
        newname[le]='\0';

        tspOut(m, mapGetPath(m, minI), newname, algos.names[minI]);

        free(newname);
    }
}

/** \fn double calcPathLength(Map m, City *path)
 *
 * \brief calcule la longueur du chemin de la solution à partir d'un tableau de City
 * \param m Map choisie pour la résolution de l'algorithme
 * \param path, tableau de City ordonnées
 * \return distance de type double
 *
 */

double calcPathLength(Map m, City *path)
{
    double s=0;

    for(int i=0; i<mapGetSize(m); i++)
        s+=cityGetDist(path[i], cityGetIndex(path[i+1]));

    return s;
}


/** \fn double calcPathLengthFromIndexesArr(Map m, int *indArr)
 *
 * \brief calcule la longueur du chemin de la solution à partir d'un tableau d'entiers
 * \param m, Map choisie pour la résolution de l'algorithme
 * \param indexArr, tableau d'int
 *
 * \return distance de type double
 *
 */

double calcPathLengthFromIndexesArr(Map m, int *indArr)   // path doit contenir sCity final
{
    double s=0;

    for(int i=0; i<mapGetSize(m); i++)
        s+=cityGetDist(mapGetCity(m, indArr[i]),indArr[i+1]);

    return s;
}

/** \fn bool isInside(int *res_indice, int lim, int index)
 *
 * \brief indique si le tableau passé en paramètre contient une valeur choisie
 * \param res_indice, tableau d'entiers sur lequel fait la recherche
 * \param lim, taille de res_indice
 * \param index, entier sur lequel on fait la recherche
 *
 * \return Retourne true si index est dans le tableau, false sinon.
 *
 */

bool isInside(int *res_indice, int lim, int index) // retourne si un indice est dans le tableau donné en paramètre
{
    for(int k=0; k<lim; k++)
        if(res_indice[k]==index)
            return true;
    return false;
}
