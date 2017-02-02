/**
 * \file branch_and_bound_hk.c
 * \brief Fichier implémentant l'algorithme Branch and Bound with Held Karp relaxation
 * \author David Phan, Raphael Afanyan
 * \version 2014
 * \date 2014
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h> //DBL_MIN, DBL_MAX
#include "algos.h"
#include "../fcts.h"
#include "pri_queue.h"
#include "vertex.h"
#include "branch_and_bound_hk.h"



static int n = 0;  // Nombre de villes
static double** adjustedDist = NULL; // matrix of adjusted costs
Map map = NULL;
static vertex_t *bestVertex = NULL; // Initialisation du meilleur vertex

City *arrCity = NULL; // Initialisation du chemin le plus court



/** \brief ajoute une arête entre deux vertex
 *
 * \param vertex, un pointeur de vertex
 * \param i, indice du premier vertex
 * \param j, indice du deuxième vertex
 *
 * Incrémente le cout minimum par le coup ajusté entre les vertex i et j
 * Incrémente le degré de vertex i et de vertex j
 *
 */

static void addEdge(vertex_t *vertex, int i, int j)
{
    vertex->lowerBound += adjustedDist[i][j];
    vertex->degree[i]++;
    vertex->degree[j]++;
};


/** \brief Remplit le tableau de degree du vertex passé en paramètre par la valeur val
 *
 * \param int * - le tableau de degree
 * \param int val - la valeur que prendra le pointeur de vertex
 * \param int size - la taille du tableau (n)
 *
 *
 */

void fillDegreeArr(int * a, const int val, const int size)
{
    int i;
    for(i=0; i < size; i++)
    {
        a[i] = val;
    }
}


/** \brief Crée un 1-tree
 *
 * Trouve les deux plus proches voisins du premier vertex 0
 * Pour cela il crée l'arbre couvrant minimal de {1...n-1} et relie
 * les 2 vertex les plus proches de 0.
 *
 */

static void doOneTree(vertex_t * vertex)
{
    // calcule les couts ajustés
    vertex->lowerBound = 0;
    fillDegreeArr(vertex->degree, 0, n); // remplit les vertex avec un degré = 0

    int i,j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            adjustedDist[i][j] = (vertex->excluded[i].ptr[j] == 1)? DBL_MAX : cityGetDist(mapGetCity(map, i),j) + vertex->adjusted[i] + vertex->adjusted[j];
    }
    int firstNeighbor;
    int secondNeighbor;
    // recherche des deux voisins les plus proches de 0
    if (adjustedDist[0][2] < adjustedDist[0][1])
    {
        firstNeighbor = 2;
        secondNeighbor = 1;
    }
    else
    {
        firstNeighbor = 1;
        secondNeighbor = 2;
    }
    for (j = 3; j < n; j++)
    {
        if (adjustedDist[0][j] < adjustedDist[0][secondNeighbor])
        {
            if (adjustedDist[0][j] < adjustedDist[0][firstNeighbor])
            {
                secondNeighbor = firstNeighbor;
                firstNeighbor = j;
            }
            else
            {
                secondNeighbor = j;
            }
        }
    }
    addEdge(vertex, 0, firstNeighbor); // on connecte le premier vertex (0) avec son voisin le plus proche
    fillDegreeArr(vertex->parent, firstNeighbor, n); // son voisin le plus proche devient son père
    vertex->parent[firstNeighbor] = 0;
    // Calcul de l'arbre au poids minimal des vertex 1 à (n-1)
    double* minDist = (double*) malloc(sizeof(double)*n);
    memcpy(minDist, adjustedDist[firstNeighbor], sizeof(double)*n); // initialisation du tableau minDist par les Dists ajustés du premier Voisin

    int k;
    for (k = 2; k < n; k++)
    {
        for (i = 1; i < n; i++) // on cherche à connecter le premier voisin
        {
            if (vertex->degree[i] == 0) break;
        }
        for (j = i + 1; j < n; j++) // on cherche un vertex qui n'est pas connecté
        {
            if ((vertex->degree[j] == 0) && (minDist[j] < minDist[i])) // et dont la distance est plus petite
                i = j;
        }
        addEdge(vertex, vertex->parent[i], i); // il devient son parent
        for (j = 1; j < n; j++) // on cherche à connecter le deuxième voisin : il faut qu'il n'ait pas de connexions (degré nul) et une distance ajustée inférieure à la distance requise
        {
            if (vertex->degree[j] == 0 && adjustedDist[i][j] < minDist[j])
            {
                minDist[j] = adjustedDist[i][j];
                vertex->parent[j] = i;
            }
        }
    }
    free(minDist);
    addEdge(vertex, 0, secondNeighbor); // on le connecte
    vertex->parent[0] = secondNeighbor; // le père du premier vertex est initialisé
    vertex->lowerBound = vertex->lowerBound;
};


/** \brief Applique la relaxation de Held-Karp à un Vertex
 *
 * \param pointeur de vertex_t
 *
 */

static void doHeldKarp(vertex_t *vertex)
{

    vertex->lowerBound = DBL_MIN; //lowerBound pour le meilleur 1-tree
    //relaxation de Lagrange
    double lambda = 0.1f; //multiplicateur de Lagrange
    //calcul du 1-tree optimal
    while (lambda > 1e-06f)
    {
        double previousLowerBound = vertex->lowerBound;
        doOneTree(vertex);
        if (!(vertex->lowerBound < bestVertex->lowerBound)) return;
        if (!(vertex->lowerBound < previousLowerBound)) lambda *= 0.9f; //maj lambda
        int i, denom = 0;
        // optimisation du subgradient pour le lowerbound
        for (i = 1; i < n; i++)
        {
            int d = vertex->degree[i] - 2; //force le 1-tree a devenir un tour
            denom += d * d;
        }
        if (denom == 0) return;
        double t = lambda * vertex->lowerBound / denom;
        for (i = 1; i < n; i++) vertex->adjusted[i] += t * (vertex->degree[i] - 2);
    }
};


/** \brief Exclu les chemins i et j (non bénéfique) d'un vertex
 *
 *
 */

static vertex_t* exclude(vertex_t *vertex, int i, int j)
{
    vertex_t* child = vertex_new(n);

    int k=0;
    for(k=0; k < n; k++)
    {
        if((k == i) || (k == j))
        {
            continue;
        }
        child->excluded[k].ptr = vertex->excluded[k].ptr;
        ++vertex->excluded[k].count; //incrémente le pointeur partagé des exclus
    }
    memcpy(child->excluded[i].ptr, vertex->excluded[i].ptr, sizeof(char)*n);
    memcpy(child->excluded[j].ptr, vertex->excluded[j].ptr, sizeof(char)*n);
    child->excluded[i].count = 0;
    child->excluded[j].count = 0;

    child->excluded[i].ptr[j] = true;
    child->excluded[j].ptr[i] = true;

    doHeldKarp(child);
    return child;
}

/** \brief Libère une partie de la mémoire utilisée et allouée par l'algorithme
 *
 *
 */

static void bbrhk_free()
{

    int i;
    for(i=0; i < n; i++)
    {
        free(adjustedDist[i]);
    }
    free(adjustedDist);

    if(bestVertex)
    {
        vertex_free(bestVertex, n);
    }
    // on ne free pas la Map car elle a été passé par référence et pas par copie
}


/** \brief Alloue une partie de la mémoire utilisée pour l'algorithme
 *
 * \param Map m - pour pouvoir  avoir la taille de l'instance
 *
 *
 */

static void bbrhk_alloc(Map m)
{
    map=m;
    n=mapGetSize(m);
    arrCity=arrCitiesCreate(n+1);

    adjustedDist = (double**) malloc(sizeof(double*)*n);
    int i;
    for(i=0; i < n; i++)
    {
        adjustedDist[i] = (double*) malloc(sizeof(double)*n);
        int j;
        for(j=0; j < n; j++)
        {
            adjustedDist[i][j] = 0.0f;
        }
    }
}

City *branchAndBoundHK(Map m, City startCity)
{
    if(mapGetSize(m)==2)
    {
        City *path=arrCitiesCreate(3);

        path[0]=mapGetCity(m,mapGetStartCity(m));

        if(mapGetStartCity(m)==0)
            path[1]=mapGetCity(m,1);
        else
            path[1]=mapGetCity(m,0);

        path[2]=path[0];

        return path;
    }

    bbrhk_alloc(m);

    bestVertex = vertex_new(n);
    bestVertex->lowerBound = DBL_MAX;

    vertex_t* currentVertex = vertex_new(n);

    doHeldKarp(currentVertex); // on calcule HeldKarp

    pri_queue pq = priq_new(11);

    do // while (currentVertex != NULL) && (currentVertex->lowerBound < bestVertex->lowerBound)
    {
        do //         while (currentVertex->lowerBound < bestVertex->lowerBound);
        {
            int i = -1,j;
            for (j = 0; j < n; j++)
            {

                if((currentVertex->degree[j] > 2) && // si degrée >2
                        ((i < 0) || currentVertex->degree[j] < currentVertex->degree[i]) ) // et qu'on trouve un degré inférieur
                    i = j;
            }
            if (i < 0) // si i <0, donc on a notre chemin à comparer
            {
                if (currentVertex->lowerBound < bestVertex->lowerBound) // comparaison avec le meilleur
                {
                    vertex_copy(bestVertex, currentVertex, n); // on le remplace
                    break;
                }

            }

            pri_queue children = priq_new(11);
            vertex_t * vertex = exclude(currentVertex, i, currentVertex->parent[i]);
            priq_push(children, vertex, vertex->lowerBound);

            for (j = 0; j < n; j++)
            {
                if (currentVertex->parent[j] == i)
                {
                    vertex = exclude(currentVertex, i, j);
                    priq_push(children, vertex, vertex->lowerBound);
                }
            }
            vertex_free(currentVertex, n);

            currentVertex = priq_pop(children, NULL);
            priq_combine(pq, children);
            priq_free(children);

        }
        while (currentVertex->lowerBound < bestVertex->lowerBound);


        currentVertex = priq_pop(pq, NULL);
    }
    while( (currentVertex != NULL) &&
            (currentVertex->lowerBound < bestVertex->lowerBound) );

    int j = 0;
    int k = 0;
    do
    {
        int i = bestVertex->parent[j];
        arrCity[k++]=mapGetCity(m,i);
        j = i;
    }
    while (j != 0);
    arrCity[k]=arrCity[0];

    vertex_t * vertex;
    while((vertex = priq_pop(pq, 0)) != NULL)
    {
        vertex_free(vertex,n);
    }
    priq_free(pq);

    while(cityGetIndex(arrCity[0])!=cityGetIndex(startCity)) // positionnage de  start City
    {
        City CityTmp=arrCity[0];
        for(int i=0; i<n-1; i++)
            arrCity[i]=arrCity[i+1];
        arrCity[n-1]=CityTmp;
        arrCity[n]=arrCity[0];
    }

    //vertex_free(currentVertex, n);
    //vertex_free(bestVertex, n);

    bbrhk_free();

    return arrCity;
}
