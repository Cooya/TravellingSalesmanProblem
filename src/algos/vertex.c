/**
 * \file vertex.c
 * \brief Fichier d'implémentation de la structure vertex.
 * \author David Phan, Raphaël Afanyan
 * \version
 * \date 2014
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h> //DBL_MIN, DBL_MAX
#include "vertex.h"

/**
 * \fn vertex_t* vertex_new(int n)
 * \brief Fonction de création d'un vertex.
 * \param n Taille de la map.
 * \return Pointeur sur structure vertex.
 */

vertex_t * vertex_new(int n)
{
    vertex_t* vertex = (vertex_t*) malloc(sizeof(vertex_t));
    vertex->excluded = (struct shared_ptr*) malloc(sizeof(struct shared_ptr)*n);
    if(vertex->excluded == NULL)
    {
        perror("malloc");
        exit(1);
    }
    int i,j;
    for(i=0; i < n; i++)
    {
        vertex->excluded[i].count = 0;
        vertex->excluded[i].ptr = (char*) malloc(sizeof(char)*n);
        for(j=0; j < n; j++)
            vertex->excluded[i].ptr[j] = 0;
    }

    vertex->adjusted = (double*) malloc(sizeof(double)*n);
    if(vertex->adjusted == NULL)
    {
        perror("malloc");
        exit(1);
    }

    vertex->degree = (int*) malloc(sizeof(int)*n);
    if(vertex->degree == NULL)
    {
        perror("malloc");
        exit(1);
    }

    vertex->parent = (int*) malloc(sizeof(int)*n);
    if(vertex->parent == NULL)
    {
        perror("malloc");
        exit(1);
    }

    vertex->lowerBound = DBL_MIN;
    for(i=0; i < n; i++)
    {
        vertex->adjusted[i] = 0.0f;
        vertex->degree[i] =
            vertex->parent[i] = 0;
    }

    return vertex;
}

/**
* \fn vertex_free(vertex_t* vertex, int n)
* \brief Fonction de création et d'initialisation de l'abre.
* \param size Taille de l'arbre, c'est-à-dire le nombre de sommets de l'arbre.
*/

void vertex_free(vertex_t* vertex, int n)
{
/*        int i;
        //probleme de free ici
        for(i=0; i < n; i++){
        	if(--vertex->excluded[i].count <= 0)
                if(vertex->excluded[i].ptr) {
                    free(vertex->excluded[i].ptr);
                    vertex->excluded[i].ptr=NULL;
                }
        }
*/
    free(vertex->excluded);
    free(vertex->parent);
    free(vertex->degree);
    free(vertex->adjusted);
    free(vertex);
    vertex = NULL;
}

/**
* \fn vertex_copy_vertex_t* to, vertex_t* from, int n)
* \brief Fonction de copie des données d'un vertex dans un autre.
* \param size Taille de l'arbre, c'est-à-dire le nombre de sommets de l'arbre.
*/

void vertex_copy(vertex_t * to, vertex_t* from, int n)
{
    to->lowerBound = from->lowerBound;
    int i;
    for(i=0; i < n; i++)
    {
        memcpy(to->excluded[i].ptr, from->excluded[i].ptr, sizeof(char)*n);
        to->excluded[i].count = 0;
    }
    memcpy(to->adjusted,	   from->adjusted,	 sizeof(double)*n);
    memcpy(to->degree, from->degree, sizeof(int)*n);
    memcpy(to->parent, from->parent, sizeof(int)*n);
};

