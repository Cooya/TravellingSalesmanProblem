/**
 * \file vertex.h
 * \brief Fichier d'implémentation de la structure vertex.
 * \author David Phan, Raphaël Afanyan
 * \version
 * \date 2014
 */

#ifndef VERTEX_H
#define VERTEX_H

/**
 * \struct shared_ptr
 * \brief Pointeur partagé du tableau d'exclusions des edges
 */
struct shared_ptr
{
    int count; //nombre de fois qu'a été partagé le pointeur
    char * ptr;
};
/**
 * \struct vertex_t
 * \brief Structure vertex qui admet un pointeur d'exclusion, un double*adjusted, un double lowerbound, un int* degree, un int* parent.
 */
typedef struct vertex
{
    struct shared_ptr* excluded;	// pour récupérer les valeurs des chemins exclus
    double* adjusted;
    double lowerBound;
    int* degree;
    int* parent;
} vertex_t;
/**
* \fn vertex_free(vertex_t* vertex, int n)
* \brief Fonction de création et d'initialisation de l'abre.
* \param size Taille de l'arbre, c'est-à-dire le nombre de sommets de l'arbre.
*/
void vertex_free(vertex_t* vertex, int n);
/**
 * \fn vertex_t* vertex_new(int n)
 * \brief Fonction de création d'un vertex.
 * \param n Taille de la map.
 * \return Pointeur sur structure vertex.
 */
vertex_t * vertex_new(int n);

/**
* \fn vertex_copy_vertex_t* to, vertex_t* from, int n)
* \brief Fonction de copie des données d'un vertex dans un autre.
* \param size Taille de l'arbre, c'est-à-dire le nombre de sommets de l'arbre.
*/
void vertex_copy(vertex_t*, vertex_t*, int);

int vertex_compare(const vertex_t * a, const vertex_t * b);
#endif

