/**
 * \file tsp.c
 * \brief Fichier d'implémentation du lecteur de fichiers TSP.
 * \author Jason Pindat
 * \version
 * \date 2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "point.h"
#include "city.h"
#include "map.h"
#include "tsp.h"
#include "fcts.h"

/**
 * \fn void throwTspErr(Str msg, int lcount, Str addMsg)
 * \brief Fonction qui lance une erreur.
 * \param Str msg : Message d'erreur.
 * \param int lcount : Ligne où se situe l'erreur.
 * \param Str addMsg : Message additionnel.
 * \return void
 */

void throwTspErr(Str msg, int lcount, Str addMsg)
{
    if(lcount>0)
    {
        Str msg2=malloc((20+strLength(msg))*sizeof(char));

        sprintf(msg2, "At line %d : %s", lcount, msg);

        throwErr("TSP Reader", msg2, addMsg);

        free(msg2);
    }
    else
        throwErr("TSP Reader", msg, addMsg);
}

/**
 * \fn void throwTspWarn(Str msg, int lcount, Str addMsg)
 * \brief Fonction qui lance un avertissement.
 * \param Str msg : Message d'erreur.
 * \param int lcount : Ligne où se situe l'erreur.
 * \param Str addMsg : Message additionnel.
 * \return void
 */

void throwTspWarn(Str msg, int lcount, Str addMsg)
{
    if(lcount>0)
    {
        Str msg2=malloc((20+strLength(msg))*sizeof(char));

        sprintf(msg2, "At line %d : %s", lcount, msg);

        throwWarn("TSP Reader", msg2, addMsg);

        free(msg2);
    }
    else
        throwWarn("TSP Reader", msg, addMsg);
}

/**
 * \fn void freeBfrs(Str str, double **ews, Point *dds, int nbCities)
 * \brief Fonction qui libère les buffers alloués lors de la lecture d'un fichier corrompu par tspLoad.
 * \param Str str : Chaîne actuellement décodée.
 * \param double **ews : Matrice de distances.
 * \param Point *dds : Tableau des coordonnées des villes.
 * \param int nbCities : Nombre de villes.
 * \return void
 */

void freeBfrs(Str str, double **ews, Point *dds, int nbCities)
{
    if(str)
        free(str);

    if(ews)
    {
        for(int i=0; i<nbCities; i++)
            free(ews[i]);

        free(ews);
    }

    if(dds)
        free(dds);
}

/**
 * \fn Map tspLoad(Str filename)
 * \brief Fonction qui lit un fichier TSP et qui retourne une Map.
 * \param Str filename : Chaîne de caractères contenant le chemin du fichier à lire.
 * \return La Map à laquelle on appliquera les algorithmes.
 */

Map tspLoad(Str filename)
{
    char line[102400];
    int index, index2, indext1, indext2;
    int lcount=1;

    bool dimok=false;
    bool ewsok=false;
    bool ddsok=false;
    bool eofok=false;

    int nbCities=-1;
    int cIndex=-1;

    Str str=NULL, str2=NULL;

    FILE *file = fopen(filename, "r");

    Point pNULL;
    pNULL.x=0;
    pNULL.y=0;

    double **ews=NULL;
    Point *dds=NULL;

    if(!file)
    {
        throwTspWarn("File could'nt be loaded from", 0, filename);
        //free(dds);
        return NULL;
    }

    throwMsg("TSP Reader", filename);

    while(fgets(line, sizeof(line), file) != NULL)
    {

        index=0;

        while(!isEOL(line[index]) && isSpaceChar(line[index])) // Recherche debut mot
            index++;

        index2=index;

        while(!isEOL(line[index2]) && !isSepChar(line[index2])) // Recherche fin mot
            index2++;

        str=subString(line, index, index2);

        /*if(strCmp(str, "NAME"))
        {
            //printf("NAME l%d\n", lcount);
        }
        else if(strCmp(str, "TYPE"))
        {
            //printf("NAME l%d\n", lcount);
        }
        else if(strCmp(str, "COMMENT"))
        {
            //printf("NAME l%d\n", lcount);
        }*/
        if(strCmp(str, "DIMENSION"))
        {
            dimok=true;
            indext1=index2;

            while(!isEOL(line[indext1]) && isSpaceChar(line[indext1]))
                indext1++;

            if(line[indext1]!=':')
            {
                throwTspWarn("Expected ':'", lcount, NULL);
                freeBfrs(str, ews, dds, nbCities);
                return NULL;
            }

            indext1++;

            while(!isEOL(line[indext1]) && isSpaceChar(line[indext1]))
                indext1++;

            indext2=indext1;

            if(!isNumber(line[indext2]))
            {
                throwTspWarn("Expected number", lcount, NULL);
                freeBfrs(str, ews, dds, nbCities);
                return NULL;
            }

            while(!isEOL(line[indext2]) && isNumber(line[indext2]))
                indext2++;

            str2=subString(line, indext1, indext2);

            nbCities=atoi(str2);

            if(nbCities<0)
            {
                throwTspWarn("Amount of cities isn't acceptable", lcount, NULL);
                freeBfrs(str, ews, dds, nbCities);
                free(str2);
                return NULL;
            }

            free(str2);

            ews=malloc(nbCities*sizeof(double*));

            for(int i=0; i<nbCities; i++)
                ews[i]=malloc(nbCities*sizeof(double));

            dds=malloc(nbCities*sizeof(Point));
        }
        else if(strCmp(str, "EDGE_WEIGHT_SECTION"))
        {
            ewsok=true;
            if(nbCities<0)
            {
                throwTspWarn("Expected DIMENSION parameter before", lcount, NULL);
                freeBfrs(str, ews, dds, nbCities);
                return NULL;
            }

            lcount++;

            for(int i=0; i<nbCities; i++)
            {
                if(fgets(line, sizeof(line), file)==NULL)
                {
                    throwTspWarn("Data missing in EDGE_WEIGHT_SECTION", lcount, NULL);
                    freeBfrs(str, ews, dds, nbCities);
                    return NULL;
                }

                index=0;

                for(int j=0; j<nbCities; j++)
                {
                    while(!isEOL(line[index]) && isSpaceChar(line[index]))
                        index++;

                    if(isEOL(line[index]))
                    {
                        throwTspWarn("Data missing in EDGE_WEIGHT_SECTION", lcount, NULL);
                        freeBfrs(str, ews, dds, nbCities);
                        return NULL;
                    }

                    index2=index;

                    if(!isNumber(line[index2]))
                    {
                        throwTspWarn("Expected number", lcount, NULL);
                        freeBfrs(str, ews, dds, nbCities);
                        return NULL;
                    }

                    while(!isEOL(line[index2]) && isNumber(line[index2]))
                        index2++;

                    if(!isSpaceChar(line[index2]))
                    {
                        throwTspWarn("Expected ' '", lcount, NULL);
                        freeBfrs(str, ews, dds, nbCities);
                        return NULL;
                    }

                    line[index2]='\0';

                    ews[i][j]=Atof(line+index);

                    index=index2+1;
                }

                lcount++;
            }
            lcount--;
        }
        else if(strCmp(str, "DISPLAY_DATA_SECTION"))
        {
            ddsok=true;
            if(nbCities<0)
            {
                throwTspWarn("Expected DIMENSION parameter before", lcount, NULL);
                freeBfrs(str, ews, dds, nbCities);
                return NULL;
            }

            lcount++;

            for(int i=0; i<nbCities; i++)
            {
                if(fgets(line, sizeof(line), file)==NULL)
                {
                    throwTspWarn("Data missing in DISPLAY_DATA_SECTION", lcount, NULL);
                    freeBfrs(str, ews, dds, nbCities);
                    return NULL;
                }

                index=0;

                for(int j=0; j<3; j++)
                {
                    while(!isEOL(line[index]) && isSpaceChar(line[index]))
                        index++;

                    if(isEOL(line[index]))
                    {
                        throwTspWarn("Data missing in DISPLAY_DATA_SECTION", lcount, NULL);
                        freeBfrs(str, ews, dds, nbCities);
                        return NULL;
                    }

                    index2=index;

                    if(!isNumber(line[index2]))
                    {
                        throwTspWarn("Expected number", lcount, NULL);
                        freeBfrs(str, ews, dds, nbCities);
                        return NULL;
                    }

                    while(!isEOL(line[index2]) && isNumber(line[index2]))
                        index2++;

                    if(!isSpaceChar(line[index2]))
                    {
                        throwTspErr("Expected ' '", lcount, NULL);
                        freeBfrs(str, ews, dds, nbCities);
                        return NULL;
                    }

                    line[index2]='\0';

                    if(j==0)
                    {
                        cIndex=atoi(line+index);
                        cIndex--;

                        if(cIndex<0 || cIndex>=nbCities)
                        {
                            throwTspWarn("Invalid index for city", lcount, NULL);
                            freeBfrs(str, ews, dds, nbCities);
                            return NULL;
                        }

                        index=index2+1;
                    }
                    else if(j==1)
                    {
                        dds[cIndex].x=Atof(line+index);

                        index=index2+1;
                    }
                    else
                    {
                        dds[cIndex].y=Atof(line+index);
                    }
                }
                lcount++;
            }
            lcount--;
        }
        else if(strCmp(str, "EOF"))
        {
            eofok=true;
            free(str);
            str=NULL;
            break;
        }
        else if(strCmp(str, "")) {}
        else
        {
            strSetLength(line, strLength(line)-1);
            throwTspWarn("Unknown instruction", lcount, line);
        }

        free(str);
        str=NULL;

        lcount++;
    }

    fclose(file);

    Map m;

    if(!dimok)
    {
        throwTspWarn("Expected instruction DIMENSION", lcount, NULL);
        return NULL;
    }
    if(!ewsok)
    {
        if(ddsok)
        {
            throwTspWarn("Expected instruction EDGE_WEIGHT_SECTION", lcount, NULL);
            m=mapCreateFromPoints(dds, nbCities, filename);
        }
        else
        {
            throwTspWarn("Expected instruction EDGE_WEIGHT_SECTION", lcount, NULL);
            freeBfrs(str, ews, dds, nbCities);
            return NULL;
        }
    }
    else
    {
        m=mapCreate();
        mapSetName(m, filename);

        if(ddsok)
        {
            for(int i=0; i<nbCities; i++)
                mapAddCity(m, cityCreate(true, dds[i], ews[i], nbCities));
        }
        else
        {
            throwMsg("TSP Reader", "No city positions available");

            for(int i=0; i<nbCities; i++)
                mapAddCity(m, cityCreate(false, pNULL, ews[i], nbCities));
        }
    }
    if(!eofok)
        throwTspWarn("Expected instruction EOF", lcount, NULL);

    for(int i=0; i<nbCities; i++)
        free(ews[i]);

    free(ews);
    free(dds);

    return m;
}

/**
 * \fn void tspWrite(Map m, Str filename)
 * \brief Fonction qui écrit un fichier TSP à partir d'une Map.
 * \param Map m : Map à écrire dans le fichier.
 * \param Str filename : Chaîne de caractères contenant le chemin du fichier à écrire.
 * \return void
 */

void tspWrite(Map m, Str filename)
{
    FILE *file = fopen(filename, "w");

    int mapSize=mapGetSize(m);

    if(!file)
        throwErr("TSP Writer", "File could'nt be written", filename);

    fprintf(file, "NAME: %s\n", filename);
    fprintf(file, "TYPE: TSP\n");
    fprintf(file, "COMMENT: Generated from VDC Random TSP Renderer\n");
    fprintf(file, "DIMENSION: %d\n", mapSize);

    fprintf(file, "EDGE_WEIGHT_TYPE: EXPLICIT\n");
    fprintf(file, "EDGE_WEIGHT_FORMAT: FULL_MATRIX \n");
    fprintf(file, "DISPLAY_DATA_TYPE: TWOD_DISPLAY\n");

    fprintf(file, "EDGE_WEIGHT_SECTION\n");

    for(int i=0; i<mapSize; i++)
    {
        for(int j=0; j<mapSize; j++)
        {
            fprintf(file, "%f ", cityGetDist(mapGetCity(m, i), j));
        }
        fprintf(file, "\n");
    }

    fprintf(file, "DISPLAY_DATA_SECTION\n");

    Point p;

    for(int i=0; i<mapSize; i++)
    {
        p = cityGetPos(mapGetCity(m, i));
        fprintf(file, "%d %f %f\n", i+1, p.x, p.y);
    }

    fprintf(file, "EOF\n");

    fclose(file);
}

/**
 * \fn void tspOut(Map m, City *path, Str filename, Str comment)
 * \brief Fonction qui écrit un fichier TOUR à partir d'une Map et d'un chemin.
 * \param Map m : Map à écrire dans le fichier.
 * \param City *path : Chemin à écrire dans le fichier.
 * \param Str filename : Chaîne de caractères contenant le chemin du fichier à écrire.
 * \param Str comment : Commentaire.
 * \return void
 */

void tspOut(Map m, City *path, Str filename, Str comment)
{
    FILE *file = fopen(filename, "w");

    int mapSize=mapGetSize(m);

    if(!file)
        throwErr("TSP Writer", "File could'nt be written", filename);

    fprintf(file, "NAME: %s\n", filename);
    fprintf(file, "TYPE: TOUR\n");
    fprintf(file, "COMMENT: Generated from TSP Solver Tour Renderer (%s algorithm)\n", comment);
    fprintf(file, "DIMENSION: %d\n", mapSize);

    fprintf(file, "TOUR_SECTION\n");

    for(int i=0; i<mapSize; i++)
    {
        fprintf(file, "%d\n", cityGetIndex(path[i])+1);
    }

    fprintf(file, "-1\n");
    fprintf(file, "EOF\n");

    fclose(file);
}
