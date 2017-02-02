 /**
 * \file main.c
 * \brief Fichier principal du programme, il contient l'usage et la gestion des différentes options.
 * \author Jason Pindat, Nicolas Marcy, David Phan
 * \version
 * \date
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tsp.h"
#include "string.h"
#include "city.h"
#include "point.h"
#include "map.h"
#include "fcts.h"
#include "gui/gui.h"
#include "algos/algos.h"
#include "api.h"

/**
* \fn void welcome()
* \brief Fonction qui affiche le nom du programme dans le terminal.
* \return void
*/

void welcome()
{
    printf("\tVoyageur de Commerce\n");
    printf("\t====================\n\n");
}

/**
* \fn void usage1(Str name)
* \brief Fonction factorisée de l'usage minimal.
* \param Str name : Nom du programme.
* \return void
*/

void usage1(Str name)
{
    printf("Usage : \n\t%s [options] <file1> [startCity1] [<file2> [startCity2] ...]\n\n", name);
    printf("Ou file(n) est un fichier TSP et startCity(n) la ville de depart (>=1) pour les algorithmes, si startCity n'est pas mentionne les algorithmes partent de la ville 1.\n");
}

/**
* \fn void usage(Str name)
* \brief Fonction qui affiche l'usage minimal.
* \param Str name : Nom du programme.
* \return void
*/

void usage(Str name)
{
    usage1(name);
    printf("\nUtiliser %s -h pour plus d'informations.\n", name);
}

/**
* \fn void phelp(Str name)
* \brief Fonction qui affiche l'usage complet.
* \param Str name : Nom du programme.
* \return void
*/

void phelp(Str name)
{
    usage1(name);
    printf("\n\tOptions d'algorithmes:\n");
    printf("-all : Execute tous les algorithmes\n");
    printf("-bb : Execute l'algorithme exact avec branch and bound\n");
    printf("-bbr : Execute l'algorithme exact avec branch and bound et relaxation NN+MST\n");
    printf("-bbrhk : Execute l'algorithme exact avec branch and bound et la relaxation de Held Karp\n");
    printf("-bf : Execute l'algorithme exact avec recherche exhaustive (Iteratif)\n");
    printf("-bfrec : Execute l'algorithme exact avec recherche exhaustive (Recursive - plus lente)\n");
    printf("-bfmt : Execute l'algorithme exact avec recherche exhaustive Multithreadee \n");
    printf("-mst : Execute l'algorithme minimum spanning tree\n");
    printf("-nn : Execute l'algorithme du plus proche voisin\n");

    printf("\n\tOptions d'affichage:\n");
    printf("-g : Mode graphique, seules les options -v, -w, -we comptent\n");
    printf("-gt : Mode graphique textuel\n");
    printf("-t : Affiche les batteries de test, soit la moyenne des distances et des temps d'execution pour un nombre donne de tests.\n A utiliser comme suit : -t <nbTests> <nbMaxCities> <algo1> [<algo2> [<algo3> [...]]] \n");
    printf("-v : Affiche les messages d'etat et les exceptions\n");
    printf("-w : Affiche les exceptions qui n'impactent pas le deroulement du programme\n");
    printf("-we : Traite les exceptions comme des erreurs\n");

    printf("\n\tOptions de traitement:\n");
    printf("-le : Definir le mode de calcul de distances en euclidiennes (defaut)\n");
    printf("-lm : Definir le mode de calcul de distances en manhattan\n");

    printf("-api : Retourne un fichier au format JSON avec les resultats d'un algorithme\n");
    printf("-o : Genere le fichier TSP correspondant au calcul aleatoire (-r)\n");
    printf("-r : Ajoute une carte de villes aleatoires. Utliser -r <nbCities> [startCity] ou nbCities est le nombre de villes a creer et startCity est la ville de depart\n");
    printf("-to : Genere le fichier TSP TOUR correspondant au meilleur resultat parmi les algorithmes executes\n");
}

/**
* \fn void main(int argc, Str *argv)
* \brief Fonction principal du programme qui gère les options et lance les modules associés.
* \param int argc : Nombre d'arguments du programme.
* \param Str *argv : Tableau de chaînes de caractères contenant les arguments du programme.
* \return void
*/

int main(int argc, Str *argv)
{
    randInit();

    initVerboseMode();

    initAlgos();

    setWerrorMode(false);
    setLengthType(MANHATTAN);

    int filesCpt=0;
    int filesSize=10;
    int *files=arrIndexesCreate(filesSize);
    int *start=arrIndexesCreate(filesSize);

    for(int i=0; i<filesSize; i++)
        start[i]=0;

    bool wStart=false;

    bool algos[NB_ALGOS];

    for(int i=0; i<NB_ALGOS; i++)
        algos[i]=false;

    bool graphics=false;
    bool api=false;
    bool graphicsText=false;
    bool randomCalc=false;
    int randNb=-1;
    bool randomStartCity=false;
    int randStartCity=0;

    bool test=false;

    bool output=false;
    Str outName;

    bool help=false;

    bool tour=false;

    welcome();

    if(argc>1)
    {
        for(int i=1; i<argc; i++)
        {
            if(wStart)   //si j'attend un numéro de ville de départ (apres un nom de fichier)
            {
                bool cont=true;
                wStart=false;

                for(int j=0; j<strLength(argv[i]); j++)   //si j'ai pas de nombre alors j'analyse l'arg normalement et met la ville à 0
                {
                    if(!isNumber(argv[i][j]))
                    {
                        randomStartCity=false;
                        cont=false;
                        break;
                    }
                }

                if(cont)   //sinon saute au prochain arg
                {
                    if(randomStartCity)
                        randStartCity=atoi(argv[i])-1;
                    else
                        start[filesCpt-1]=atoi(argv[i])-1;

                    randomStartCity=false;
                    continue;
                }
            }
            if(strCmp(argv[i], "-v"))
                setVerboseMode(2);
            else if(strCmp(argv[i], "-w"))
                setVerboseMode(1);
            else if(strCmp(argv[i], "-we"))
                setWerrorMode(true);
            else if(strCmp(argv[i], "-nn"))
                algos[0]=true;
            else if(strCmp(argv[i], "-mst"))
                algos[1]=true;
            else if(strCmp(argv[i], "-bf"))
                algos[2]=true;
            else if(strCmp(argv[i], "-bb"))
                algos[3]=true;
            else if(strCmp(argv[i], "-bfrec"))
                algos[4]=true;
            else if(strCmp(argv[i], "-bfmt"))
                algos[5]=true;
            else if(strCmp(argv[i], "-bbr"))
                algos[6]=true;
            else if(strCmp(argv[i], "-bbrhk"))
                algos[7]=true;
            else if(strCmp(argv[i], "-all"))
                for(int i=0; i<NB_ALGOS; i++)
                    algos[i]=true;
            else if(strCmp(argv[i], "-g"))
                graphics=true;
            else if(strCmp(argv[i], "-lm"))
                setLengthType(MANHATTAN);
            else if(strCmp(argv[i], "-le"))
                setLengthType(EUCLIDIAN);
            else if(strCmp(argv[i], "-gt"))
                graphicsText=true;
            else if(strCmp(argv[i], "-api"))
                api=true;
            else if(strCmp(argv[i], "-t"))
            {
                if(i!=1 || argc<5)
                    throwErr("Main", "Expecting -t and at least 3 arguments", NULL);
                i++;
                for(int j=0; j<strLength(argv[i]); ++j) // vérification du premier nombre
                    if(!isNumber(argv[i][j]))
                        throwErr("Main", "Expecting -t <number> <number> ...", NULL);
                i++;
                for(int j=0; j<strLength(argv[i]); ++j) // vérification du second nombre
                    if(!isNumber(argv[i][j]))
                        throwErr("Main", "Expecting -t <number> <number> ...", NULL);
                test=true;
            }
            else if(strCmp(argv[i], "-r"))
            {
                randomCalc=true;

                i++;

                if(i>=argc)
                    throwErr("Main", "Expecting a city amount for random calculation", NULL);


                for(int j=0; argv[i][j]!='\0'; j++)
                    if(!isNumber(argv[i][j]))
                        throwErr("Main", "Expecting a city amount for random calculation", NULL);

                randNb=atoi(argv[i]);
                if(randNb<=0)
                    throwErr("Main", "Expecting a city amount >=1 for random calculation", NULL);

                randomStartCity=true;
                wStart=true;
            }
            else if(strCmp(argv[i], "-o"))
            {
                output=true;

                i++;

                if(i>=argc)
                    throwErr("Main", "Expecting a name for file output", NULL);

                outName=argv[i];
            }
            else if(strCmp(argv[i], "-h"))
                help=true;
            else if(strCmp(argv[i], "-to"))
                tour=true;
            else
            {
                if(filesCpt>=filesSize)
                {
                    filesSize*=2;
                    files=arrIndexesRealloc(files, filesSize);
                    start=arrIndexesRealloc(start, filesSize);

                    for(int i=filesCpt; i<filesSize; i++)
                        start[i]=0;
                }

                wStart=true;
                files[filesCpt++]=i;
            }
        }
    }

    /*************/

    if(help)
    {
        phelp(argv[0]);
        exit(0);
    }

    if(graphics)
        guiInit(&argc, &argv);


    /*if(graphics) {
        throwMsg("Main", "Entering graphic mode");
        //fct mode graphic
    }
    else {*/

    if(filesCpt==0 && !randomCalc && !test)
    {
        if(!graphics)
            usage(argv[0]);
        else
            guiShow();
    }
    if(test)
    {
        printf("====== / GLOBAL TEST \\ =======\n\n");
        for(int i=2; i<atoi(argv[3])+1; ++i){
		printTest(i,atoi(argv[2]),algos);
	}

    }
    if(randomCalc)
    {
        Map m = mapCreateRandom(randNb);

        if(!output)
        {
            if(getVerboseMode()>=2)
                mapDataDump(m);

            printf("=== Random Map ===\n");

            if(randStartCity<0 || randStartCity>=mapGetSize(m))
            {
                char erroMsg[100];
                sprintf(erroMsg,"The start city isn't in the random map : input %d , value should be between 1 and %d",randStartCity+1,mapGetSize(m));
                throwErr("Main", erroMsg, NULL);
            }

            if(!api)
            {
                executeAlgos(m, algos, randStartCity, graphics, tour);
                if(graphicsText)
                    mapDraw(m);

                if(graphics)
                {
                    guiAddMap(m);
                    guiShow();
                }

            }
            else //api
            {
                executeApis(m,algos,randStartCity);
            }

        }
        else
        {
            tspWrite(m, outName);
        }

        mapDeleteRec(m);
    }

    for(int i=0; i<filesCpt; i++)
    {
        Map m = tspLoad(argv[files[i]]);
        if(m!=NULL)
        {
            if(getVerboseMode()>=2) // showTSP)
                mapDataDump(m);

            printf("=== %s ===\n", argv[files[i]]);

            if(start[i]<0 || start[i]>=mapGetSize(m))
            {
                char erroMsg[100];
                sprintf(erroMsg,"The start city isn't in the random map : input %d , value should be between 1 and %d",start[i]+1,mapGetSize(m));
                throwErr("Main", erroMsg, argv[files[i]]);
            }

            if(! api)
            {
                executeAlgos(m, algos, start[i], graphics, tour); //, test);

                if(graphicsText)
                    mapDraw(m);

                if(graphics)
                {
                    guiAddMap(m);
                    guiShow();
                }
            }
            else // api
                executeApis(m, algos, start[i]);

            if(!graphics)
                mapDeleteRec(m);
        }
        else
            printf("[Main] Unable to read %s\n",argv[files[i]]);



    }

    freeArrIndexes(files);
    freeArrIndexes(start);

    if(graphics)
        guiStop();

    throwMsg("Main", "Bye !");

    return EXIT_SUCCESS;
}
