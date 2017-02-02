/**
 * \file pri_queue.c
 * \brief Fichier d'implémentation de la file de priorité pour l'algo Held-Karp.
 * \author David Phan, Raphaël Afanyan
 * \version
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include "pri_queue.h"

/** \def priq_purge(q)
 * \brief réinitialise la file
 */

#define priq_purge(q) (q)->n = 1


/** \def priq_size(q)
 * \brief retourne le nombre d'éléments actuels dans la file
 */

#define priq_size(q) ((q)->n - 1)



/** \fn pri_queue priq_new(int size)
 * \brief crée et initialise une file de priorité (le premier élément dans le tableau n'est pas utilisé pour simplifier les indices)
 * \param size, taille de la file de priorité
 * \return file de priorité
 */

pri_queue priq_new(int size)
{
    if (size < 4) size = 4;

    pri_queue q = malloc(sizeof(pri_queue_t));
    q->buf = malloc(sizeof(q_elem_t) * size);
    q->alloc = size;
    q->n = 1;

    return q;
}


/** \fn priq_free(pri_queue q)
 * \brief Supprime une file de priorité et libère sa mémoire
 * \param q, la file de priorité choisie
 */

void priq_free(pri_queue q)
{
    free(q->buf);
    free(q);
}


/** \fn void priq_push(pri_queue q, void *data, double pri)
 * \brief ajoute un élément à une file de priorité avec une priorité associée
 * \param q, la file de priorité traitée
 * \param data, ce que l'on ajoute à la pile
 * \param pri, la priorité associée à l'élément data
 */

void priq_push(pri_queue q, void *data, double pri)
{
    q_elem_t *b;
    int n, m;

    if (q->n >= q->alloc)
    {
        q->alloc *= 2;
        b = q->buf = realloc(q->buf, sizeof(q_elem_t) * q->alloc);
    }
    else
        b = q->buf;

    n = q->n++;
    /* append at end, then up heap */
    while ((m = n / 2) && pri < b[m].pri)
    {
        b[n] = b[m];
        n = m;
    }
    b[n].data = data;
    b[n].pri = pri;
}


/** \fn void * priq_pop(pri_queue q, double *pri)
 * \brief Retire l'élément du haut
 * \param  q, la file de priorité
 * \param pri, indice de priorité pouvant influencer l'élément retiré (peut être NULL)
 * \return 0 si la file est vide, un q_elem_t sinon
 */

void * priq_pop(pri_queue q, double *pri)
{
    void *out;
    if (q->n == 1) return 0;

    q_elem_t *b = q->buf;

    out = b[1].data;
    if (pri) *pri = b[1].pri;

    /* pull last item to top, then down heap. */
    --q->n;

    int n = 1, m;
    while ((m = n * 2) < q->n)
    {
        if (m + 1 < q->n && b[m].pri > b[m + 1].pri) m++;

        if (b[q->n].pri <= b[m].pri) break;
        b[n] = b[m];
        n = m;
    }

    b[n] = b[q->n];
    if (q->n < q->alloc / 2 && q->n >= 16)
        q->buf = realloc(q->buf, (q->alloc /= 2) * sizeof(b[0]));

    return out;
}

/** \fn void* priq_top(pri_queue q, double *pri)
 * \brief Retourne l'élément du haut de la file
 * \param q, la file de priorité
 * \param pri, l'indice de priorité
 * \return 0 si la file est vide, le q_elem_t situé en haut sinon
 */

/* get the top element without removing it from queue */
void* priq_top(pri_queue q, double *pri)
{
    if (q->n == 1) return 0;
    if (pri) *pri = q->buf[1].pri;
    return q->buf[1].data;
}


/** \fn void priq_combine(pri_queue q, pri_queue q2)
 * \brief Concatène 2 files de priorité
 * \param q, la file de priorité 1
 * \param q2, la file de priorité 2
 */

void priq_combine(pri_queue q, pri_queue q2)
{
    int i;
    q_elem_t *e = q2->buf + 1;

    for (i = q2->n - 1; i >= 1; i--, e++)
        priq_push(q, e->data, e->pri);
    priq_purge(q2);
}
