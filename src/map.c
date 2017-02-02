/**
 * \file map.c
 * \brief Structure Map et ses fonctions
 * \author David Phan, Jason Pindat
 * \version 0.28
 * \date 12/03/2014
 * Structure Map et ses fonctions d'instanciation, de suppression et d'affichage (mode terminal)
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "map.h"
#include "city.h"
#include "fcts.h"
#include "algos/algos.h"

#define CITIESINIT 10

/** \struct _Map
 *  \brief Structure représentant une Map contenant les villes.
 */

struct _Map
{
    bool isPos; /*!< Booléen qui vaut true lorsque la map est generée par des City déterminées par des points (et non une matrice de distance seulement). */
    City* cities; /*!< Tableau de villes extensible. */
    int nbCities; /*!< Nombre de villes dans le tableau. */
    int nbCitiesMax; /*!< Nombre de villes maximum. */
    Str name; /*!< Nom du fichier TSP associé à la Map. */
    City **paths; /*!< Tableau à deux dimensions contenant les chemins de obtenus par chaque algorithme. */
    double *duration; /*!< Tableau stockant les temps d'exécution de chaque algorithme. */
    int startCity; /*!< L'index de la ville de départ de la Map. */
};


/** \fn Map mapCreate()
 *  \brief mapCreate crée une instance de Map (avec allocation mémoire)
 *  \return objet de type Map
 */

Map mapCreate()
{
    throwMsg("Map", "Creating map...");

    Map mapTMP=malloc(sizeof(struct _Map));

    mapTMP->isPos=true;
    mapTMP->nbCities=0;
    mapTMP->nbCitiesMax=CITIESINIT;
    mapTMP->cities=malloc(sizeof(City)*mapTMP->nbCitiesMax);
    mapTMP->name=NULL;
    mapTMP->paths=malloc(NB_ALGOS*sizeof(City*));
    mapTMP->startCity=0;

    for(int i=0; i<NB_ALGOS; i++)
        mapTMP->paths[i]=NULL;

    mapTMP->duration=malloc(NB_ALGOS*sizeof(double));

    return mapTMP;
}


/** \fn Str mapGetName(Map m)
 * \brief Retourne le nom de la map passée en paramètre
 * \param m Map que l'on traite
 * \return de type String
 */

Str mapGetName(Map m)
{
    return m->name;
}


/** \fn void mapSetName(Map, Str n)
 * \brief Modifie le nom d'une map
 * \param m Map que l'on souhaite modifier
 * \param n nom choisi de type String
 */

void mapSetName(Map m, Str n)
{
    if(m->name)
        free(m->name);
    m->name=strCopy(n);
}

/** \fn City *mapGetPath(Map m, int i)
 * \brief Retourne une City stockée dans le tableau paths d'une Map
 * \param m Map contenant le tableau de City
 * \param i entier passé en index de paths désignant le chemin choisi
 * \return objet de type City
 */

City *mapGetPath(Map m, int i)
{
    return m->paths[i];
}

/** \fn void mapSetPath(Map m, int i, City *path)
 * \brief Modifie une des chemins d'une Map
 * \param m Map contenant le chemin
 * \param i entier passé en index de paths désignant le chemin choisi
 * \param path tableau de City représentant le nouveau chemin
 */

void mapSetPath(Map m, int i, City *path)
{
    m->paths[i]=path;
}


/** \fn double mapGetDuration(Map m, int i)
 * \brief Retourne le temps d'exécution de l'algorithme choisi grâce à l'entier i
 * \param m Map stockant le tableau duration
 * \param i Entier passé en index du tableau duration désignant l'algorithme
 * \return nombre de type double (précision)
 */

double mapGetDuration(Map m, int i)
{
    return m->duration[i];
}


/** \fn double mapSetDuration(Map m, int i, double val)
 * \brief Modifie le tableau duration
 * \param m Map stockant le tableau duration
 * \param i Entier passé en index du tableau duration désignant l'algorithme
 * \param val temps d'exécution de l'algorithme choisi
 */

void mapSetDuration(Map m, int i, double val)
{
    m->duration[i]=val;
}


/** \fn double mapGetStartCity(Map m)
 * \brief Retourne la ville de départ de la Map
 * \param m Map contenant la ville de depart
 * \return retourne la ville de départ
 */

int mapGetStartCity(Map m)
{
    return m->startCity;
}


/** \fn double mapSetStartCity(Map m, int val)
 * \brief Modifie la ville de départ
 * \param m Map contenant la ville de départ
 * \param val ville de départ choisie
 */

void mapSetStartCity(Map m, int val)
{
    m->startCity=val;
}

/** \fn void mapDelete(Map m)
 *  \brief Supprime et libère la mémoire utilisée par la Map passée en paramètre
 * \param m Map qu'il faut libérer
 */

void mapDelete(Map m)
{
    throwMsg("Map", "Deleting Map...");

    for(int i=0; i<NB_ALGOS; i++)
        if(m->paths[i])
            free(m->paths[i]);

    free(m->paths);

    free(m->duration);

    free(m->cities);

    if(m->name)
        free(m->name);

    free(m);
}

/** \fn mapDeleteRec(Map m)
 *  \brief supprime les instances de type City de l'objet Map passé en paramètre
 * \param m Map qui contient les objets de type City
 */

void mapDeleteRec(Map m)   // Frees recursively
{
    for(int i=0; i<mapGetSize(m); i++)
        cityDelete(mapGetCity(m, i));

    mapDelete(m);
}
/** \fn void mapAddCity(Map m, City c)
 *  \brief Ajoute un objet de type City (par référence) à un objet de type Map
 * \param m Objet de type Map
 * \param c Objet de type City
 */

void mapAddCity(Map m, City c)
{
    throwMsg("Map", "Adding city...");

    if(m->nbCities>=m->nbCitiesMax)
    {
        m->nbCitiesMax*=2;
        m->cities=realloc(m->cities, sizeof(City)*m->nbCitiesMax);
    }

    if(!cityGetIsPos(c))
        m->isPos=false;

    citySetIndex(c, m->nbCities);

    m->cities[m->nbCities++]=c;
}


/** \fn City mapGetCity(Map m, int indice)
 *  \brief Retourne l'objet City par son indice indice dans la Map m
 * \param m Map qui contient la ville que l'on cherche
 * \param indice Entier qui définit le numéro de l'objet City dans l'objet Map
 * \return City objet de type City
 */

City mapGetCity(Map m, int indice)
{
    if(indice<0 || indice>=m->nbCities)
        throwErr("Map", "City index out of range (mapGetCity)", NULL);

    return m->cities[indice];
}
/** \fn void mapSetCity(Map m, City c, int indice)
 *  \brief Met l'objet City c dans l'objet Map avec l'indice indice
 * \param m Objet de type Map
 * \param c Objet de type City qui sera ajouté dans l'objet m par référence
 * \param indice Indice que prendra l'objet City dans l'objet Map
 */

void mapSetCity(Map m, City c, int indice)
{
    if(indice<0 || indice>=m->nbCities)
        throwErr("Map", "Index out of range (mapSetCity)", NULL);

    m->cities[indice]=c;
}


/** \fn int mapGetSize(Map m)
 *  \brief Retourne le nombre de City que contient l'objet Map m
 * \param m L'objet de type Map
 * \return le nombre d'objet City
 */

int mapGetSize(Map m)
{
    return m->nbCities;
}

/** \fn bool mapGetIsPos(Map m)
 *  \brief retourne si la Map m est déterminée par des points ou seulement par une matrice de distance
 * \param m l'objet de type Map
 * \return TRUE lorsque Map est déterminée par des points, FALSE lorsque Map est déterminée par
 */

bool mapGetIsPos(Map m)
{
    return m->isPos;
}

/** \fn void mapDataDump(Map m)
 *  \brief affiche les données d'une Map (pour le mode verbose)
 *
 * \param m Objet de type Map
 *
 */

void mapDataDump(Map m)
{
    int nbVilles=mapGetSize(m);
    for(int i=0; i<nbVilles; i++)
    {
        printf("City %d \n",i);
        cityDataDump(mapGetCity(m,i));
    }
}

/** \fn Map mapCreateFromPoints(Point* villes, int nbVilles)
 *  \brief Crée un objet de type Map avec un tableau de Point, et le nombre de Point
 * \param villes Objet de type Point* contenant les références de nbVilles Point
 * \param nbVilles nombre d'objet de type Point passés en paramètres
 * \param name nom du fichier tsp
 * \return Objet de type Map avec nbVilles objets City
 */

Map mapCreateFromPoints(Point* villes, int nbVilles, Str name)
{
    Map m=mapCreate();

    m->name=strCopy(name);

    throwMsg("Map", "Calculating lengths...");

    double dists[nbVilles];

    for(int i=0; i<nbVilles; i++)
    {
        for (int j=0; j<nbVilles; j++)
            dists[j]=length(villes[i],villes[j]);

        City ville=cityCreate(true, villes[i],dists,nbVilles);
        mapAddCity(m,ville);
    }

    return m;
}


/** \fn Map mapCreateRandom(int nbVilles)
 *  \brief Crée un objet de type Map avec nbVilles objets City avec des points générés aléatoirements
 * \param nbVilles nombre de villes que l'objet Map aura
 * \return un objet de type Map avec nbVilles City
 */

Map mapCreateRandom(int nbVilles)
{
    Point villes[nbVilles];

    for(int i=0; i<nbVilles; i++)
    {
        villes[i].x=(double)randNum(0, 1000);
        villes[i].y=(double)randNum(0, 1000);
    }

    Str txt=malloc(1000);

    Str txt1="Random_Map_";
    Str txt2=getTime();

    int j=0;

    for(int i=0; i<strLength(txt1); i++)
        txt[j++]=txt1[i];

    for(int i=0; i<strLength(txt2); i++)
        txt[j++]=txt2[i];

    txt[j]='\0';

    Map m = mapCreateFromPoints(villes, nbVilles, txt);

    free(txt);
    free(txt2);

    return m;
}

/** \fn void mapDraw(Map m)
 *  \brief Dessine sur le terminal l'objet de type Map
 * \param m l'objet de type Map
 */

void mapDraw(Map m)
{
    if(!mapGetIsPos(m))
        throwWarn("Map", "Some cities in the map have no positions (mapDraw)", NULL);
    else
    {
        int sizey=30;
        int sizex=(int)(1.25*sizey);
        int mapSize=mapGetSize(m);

        Point min, max, tmp;

        Point pts[mapSize];

        min=cityGetPos(mapGetCity(m, 0));
        max=min;

        for(int i=1; i<mapSize; i++)
        {
            tmp=cityGetPos(mapGetCity(m, i));
            if(tmp.x<min.x)
                min.x=tmp.x;
            if(tmp.y<min.y)
                min.y=tmp.y;
            if(tmp.x>max.x)
                max.x=tmp.x;
            if(tmp.y>max.y)
                max.y=tmp.y;
        }

        double ecartx=max.x-min.x;
        double ecarty=max.y-min.y;

        if(ecartx==0)
            ecartx=1;
        if(ecarty==0)
            ecarty=1;

        double coefx=sizex/ecartx;
        double coefy=sizey/ecarty;

        for(int i=0; i<mapSize; i++)
        {
            pts[i]=cityGetPos(mapGetCity(m, i));
            pts[i].x-=min.x;
            pts[i].y-=min.y;
            pts[i].x*=coefx;
            pts[i].y*=coefy;
            //printf("%f %f\n", pts[i].x, pts[i].y);
        }

        printf(" ");
        for(int i=0; i<(sizex+1)*2; i++)
            printf("-");
        printf("\n");

        bool ok=false;
        for(int i=0; i<sizey+1; i++)
        {
            printf("|");
            for(int j=0; j<sizex+1; j++)
            {
                ok=false;
                for(int k=0; k<mapSize; k++)
                    if((int)pts[k].x==j && (int)pts[k].y==i)
                    {
                        printf("%2d", k+1);
                        ok=true;
                    }
                if(!ok)
                    printf("  ");
            }
            printf("|\n");
        }

        printf(" ");
        for(int i=0; i<(sizex+1)*2; i++)
            printf("-");
        printf("\n");
    }
}
