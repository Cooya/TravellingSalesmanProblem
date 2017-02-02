/**
 * \file api.c
 * \brief Programme de sortie de Map et résultats au format JSON. Les algorithmes acceptés sont NN, MST et BBRHK.
 * \author David
 * \version
 * \date Avril 2014
 */

#include <stdlib.h>
#include <stdio.h>

#include "city.h"
#include "point.h"
#include "map.h"
#include "fcts.h"
#include "algos/algos.h"
#include "algos/nearest_neighbour.h"
#include "algos/bruteforce.h"
#include "algos/branch_and_bound.h"
#include "algos/minimum_spanning_tree.h"
#include "algos/branch_and_bound_hk.h"
#include "api.h"

/** \fn void executeApis(Map m, bool * algos, int sC)
 *  \brief Exécute les algorithmes demandés (NN, MST, BBRHK acceptés) l'ecriture des JSON selon les algos demandés ainsi que la Map
 * \param Map m : La map à traiter
 * \param bool * algos : un tableau d'algorithmes de taille NB_ALGOS, quand le bool = TRUE; alors il va exécuter printJSON
 * \param int sC : la City de départ
 * \return void
 */

void executeApis(Map m, bool *algos, int startC)
{
    if(algos[0])
    {
        City* path=nearestNeighbour(m, mapGetCity(m, startC));
        printJSON(m,path,0);
        freeArrCities(path);
    }
    if(algos[1])
    {
        City* path=minimumSpanningTree(m, mapGetCity(m, startC));
        printJSON(m,path,1);
        freeArrCities(path);
    }
    for(int i=2; i>7; i++)
    {
        if(algos[i])
            printf("Vous ne pouvez pas générer un fichier _opt.json avec l'algorithme choisi %d \n",i);
    }
    if(algos[7])
    {
        City* path=branchAndBoundHK(m, mapGetCity(m, startC));
        printJSON(m,path,7);
        freeArrCities(path);
    }

}

/** \fn void printJSON(Map m, City * path, int algoid)
 *  \brief Exécute l'ecriture des JSON avec les chemins calculés dans executeApis, avec les données de la Map et l'ID d'algorithme (pour écrire le nom de l'algorithme dans le fichier)
 * \param Map m : La map à traiter
 * \param City * path : le chemin calculé par le(s) algorithme(s)
 * \param int algoid : l'id de l'algorithme utilisé
 * \return écrit un fichier {nomfichier}_{algo}.json
 */

void printJSON(Map m, City *path, int algoid)
{

    char *suffix;
    char *algoname;
    switch (algoid)
    {
    case 0:
        algoname="Nearest Neighbour";
        suffix="_nn.json";
        break;
    case 1:
        algoname="Minimum Spanning Tree";
        suffix="_mst.json";
        break;
    case 7:
        algoname="Branch and Bound with Held Karp Relaxation (optimal tour)";
        suffix="_opt.json";
        break;
    default:
        suffix="err";
        break;
    }
    char outputname[strlen(mapGetName(m))+strlen(suffix)+1];

    strcpy(outputname, mapGetName(m));
    strcat(outputname,suffix);

    if(!(algoid>=2 && algoid <=6))
    {

        FILE *fp=fopen(outputname, "w"); // fichier en mode ecriture (ecrasage si existant)
        fprintf(fp, "{\n\t\"tsp\":{\n");
        fprintf(fp, "\t\t\"name\": \"%s\",\n",mapGetName(m));
        fprintf(fp, "\t\t\"nb\": \"%d\",\n",mapGetSize(m));
        fprintf(fp, "\t\t\"isPos\": \"%s\",\n", mapGetIsPos(m) ? "true" : "false");
        if(mapGetIsPos(m))
        {
            fprintf(fp, "\t\t\"POS\":{\n");
            for(int i=0; i<mapGetSize(m); i++)
            {
                fprintf(fp, "\"%d\" : { \"x\" : \"%lf\", \"y\" : \"%lf\" } ",i,pointGetX(cityGetPos(mapGetCity(m,i))) , pointGetY(cityGetPos(mapGetCity(m,i))) );
                if(i!=mapGetSize(m)-1)
                    fprintf(fp, ",");
                fprintf(fp, "\n");
            }
        }
        fprintf(fp, "\n},");
        fprintf(fp, "\t\t\"2DMAT\":{\n");
        for(int i=0; i<mapGetSize(m); i++)

        {
            fprintf(fp, "\t\t\t\"%d\":{\n\t\t\t\t",i);
            for(int j=0; j<mapGetSize(m); j++)
            {
                fprintf(fp, "\"%d\" : \"%lf\" ",j,cityGetDist(mapGetCity(m,i),j));
                if(j!=mapGetSize(m)-1)
                    fprintf(fp, ",");
            }
            fprintf(fp, "\t\t\t}");
            if(i!=mapGetSize(m)-1)
                fprintf(fp, ",");
            fprintf(fp, "\n");

        }
        fprintf(fp, "\t\t},\n");
        fprintf(fp, "\t\"result\": {\n");
        fprintf(fp, "\t\t\"algorithm\" : \"%s\",",algoname);
        fprintf(fp, "\t\t\"length\" : \"%lf\" , \n",calcPathLength(m,path));
        fprintf(fp, "\t\t\"path_len\" : \"%d\" , \n",mapGetSize(m)+1);
        fprintf(fp, "\t\t\"path\" : {\n");
        fprintf(fp, "\t\t\t");
        for(int i=0; i<mapGetSize(m)+1; i++)
        {
            fprintf(fp, "\"%d\" : \"%d\"",i, cityGetIndex(path[i])+1);
            if(i!=mapGetSize(m))
                fprintf(fp, ", ");
        }
        fprintf(fp, "\n\t\t},\n");
        fprintf(fp, "\t\t\"dists_path\" : {\n");
        fprintf(fp, "\t\t\t");
        for(int i=0; i<mapGetSize(m)+1; i++)
        {
            if(i==0)
                fprintf(fp, "\"%d\" : \"0.000000\"",i);
            else
                fprintf(fp, "\"%d\" : \"%lf\"",i, cityGetDist(path[i-1],cityGetIndex(path[i]))); //mapGetIndiceCity(m,path[i])+1);
            if(i!=mapGetSize(m))
                fprintf(fp, ", ");
        }
        fprintf(fp, "\n\t\t}\n");
        fprintf(fp, "\t}\n");
        fprintf(fp, "}}\n");

        if(fclose(fp)==0)
            printf("Successfully wrote JSON File %s\n",outputname);
        else
            printf("Error while writing JSON File %s\n", outputname);
    }
    else
    {
        printf("Algorithme choisi %d non disponible dans l'API",algoid);
    }

}
