/**
 * \file pri_queue.h
 * \brief Fichier d'en-tête de la file de priorité pour l'algo Held-Karp.
 * \author David Phan, Raphaël Afanyan
 * \version
 * \date 2014
 */

#ifndef PRIQ_H
#define PRIQ_H

/** \struct q_elem_t
 * \brief représente un élément de la file de priorité.
 */

typedef struct
{
    void * data; /**< pointeur générique vers l'élément */
    double pri; /**< indice de priorité */
} q_elem_t;

/** \struct pri_queue_t, *pri_queue
 * \brief La file de priorité
 *  Une file de priorité est une file où chaque élément a une "priorité"
 * associée, ainsi un élément avec une haute priorité est servi avant
 * un élémentavec une plus faible priorité, si elle est égale le premier
 * rentré sortira en premier
 */

typedef struct
{
    q_elem_t *buf; /**< Tableau de q_elem_t (éléments de la file) */
    int n; /**< indice représentant la position dans la file */
    int alloc; /**< taille max de la file de priorité */
} pri_queue_t, *pri_queue;

/** \fn pri_queue priq_new(int size)
 * \brief crée et initialise une file de priorité (le premier élément dans le tableau n'est pas utilisé pour simplifier les indices)
 * \param size, taille de la file de priorité
 * \return file de priorité
 */

pri_queue priq_new(int size);

/** \fn priq_free(pri_queue q)
 * \brief Supprime une file de priorité et libère sa mémoire
 * \param q, la file de priorité choisie
 */

void priq_free(pri_queue q);

/** \fn void priq_push(pri_queue q, void *data, double pri)
 * \brief ajoute un élément à une file de priorité avec une priorité associée
 * \param q, la file de priorité traitée
 * \param data, ce que l'on ajoute à la pile
 * \param pri, la priorité associée à l'élément data
 */

void priq_push(pri_queue q, void *data, double pri);

/** \fn void * priq_pop(pri_queue q, double *pri)
 * \brief Retire l'élément du haut
 * \param  q, la file de priorité
 * \param pri, indice de priorité pouvant influencer l'élément retiré (peut être NULL)
 * \return 0 si la file est vide, un q_elem_t sinon
 */

void * priq_pop(pri_queue q, double *pri);

/** \fn void* priq_top(pri_queue q, double *pri)
 * \brief Retourne l'élément du haut de la file
 * \param q, la file de priorité
 * \param pri, l'indice de priorité
 * \return 0 si la file est vide, le q_elem_t situé en haut sinon
 */

void* priq_top(pri_queue q, double *pri);

/** \fn void priq_combine(pri_queue q, pri_queue q2)
 * \brief Concatène 2 files de priorité
 * \param q, la file de priorité 1
 * \param q2, la file de priorité 2
 */

void priq_combine(pri_queue q, pri_queue q2);

#endif
