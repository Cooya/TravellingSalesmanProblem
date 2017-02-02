/**
 * \file gui/gui.c
 * \brief Programme principal de l'interface graphique.
 * \author Jason Pindat
 * \version 0.64
 * \date 26/03/2014
 * Programme de gestion de l'interface graphique
 */

#include <gtk/gtk.h>
#include <stdlib.h>

#include "gui.h"
#include "guisys.h"

#include "../map.h"
#include "../city.h"
#include "../point.h"
#include "../string.h"
#include "../fcts.h"
#include "../algos/algos.h"

/**
 * \struct gCt
 * \brief Objet contenant les villes.
 * gCt contient les noms , les positions et la Map à laquelle réfèrent les villes stockées pour l'affichage
 */

struct _gCt
{
    Point* pos;
    Str* name;
    int size;
    int len;
    Map m;
} gCt;

/**
 * \struct gCtl
 * \brief Objet contenant les chemins.
 * gCtl contient les positions de départ et d'arrivée de chaque flèches décrivant un chemin pour l'affichage
 */

struct _gCtl
{
    Point* pos1;
    Point* pos2;
    double *l;
    int size;
    int len;
} gCtl;

/**
 * \struct gCtt
 * \brief Objet contenant les labels.
 * gCtt contient les chaines de caractères à afficher en bas d'image
 */

struct _gCtt
{
    Str *lab;
    int size;
    int len;
} gCtt;

/**
 * \fn void guiInit(int* argc, char ***argv)
 * \brief Fonction d'initialisation des tampons pour l'affichage graphique
 * \param int *argc et char ***argv adresses respectives des paramètres de la fonction main
 * \return void
 */

void guiInit(int* argc, char ***argv)
{
    gtk_init(argc, argv);

    guiSysInit();

    gCt.len=0;
    gCt.size=10;

    gCt.pos=malloc(gCt.size*sizeof(Point));
    gCt.name=malloc(gCt.size*sizeof(Str));

    gCt.m=NULL;

    gCtl.len=0;
    gCtl.size=10;

    gCtl.pos1=malloc(gCtl.size*sizeof(Point));
    gCtl.pos2=malloc(gCtl.size*sizeof(Point));
    gCtl.l=malloc(gCtl.size*sizeof(double));

    gCtt.len=0;
    gCtt.size=10;

    gCtt.lab=malloc(gCtt.size*sizeof(Str));
}

/**
 * \fn void guiFreeMap()
 * \brief Libère le tampon de stockage des coordonnées de villes
 * \param void
 * \return void
 */

void guiFreeMap()
{
    for(int i=0; i<gCt.len; i++)
        free(gCt.name[i]);

    gCt.len=0;
}

/**
 * \fn void guiFreePaths()
 * \brief Libère le tampon de stockage des coordonnées de chemins
 * \param void
 * \return void
 */

void guiFreePaths()
{
    gCtl.len=0;
}

/**
 * \fn void guiFreeLabels()
 * \brief Libère le tampon de stockage des labels
 * \param void
 * \return void
 */

void guiFreeLabels()
{
    for(int i=0; i<gCtt.len; i++)
        free(gCtt.lab[i]);

    gCtt.len=0;
    guiSysFreeLabels();
}

/**
 * \fn void guiFree()
 * \brief Libère tous les tampons de stockage
 * \param void
 * \return void
 */

void guiFree()
{
    guiFreeMap();
    guiFreePaths();
    guiFreeLabels();
}

/**
 * \fn void guiStop()
 * \brief Libère tous les tampons de stockage et l'interface graphique
 * \param void
 * \return void
 */

void guiStop()
{
    guiFree();

    free(gCt.pos);
    free(gCt.name);

    free(gCtl.pos1);
    free(gCtl.pos2);
    free(gCtl.l);

    free(gCtt.lab);

    guiSysStop();
}

/**
 * \fn void guiAddPoint(Point pos, Str name)
 * \brief Ajoute un point dans le tampon des coordonnées
 * \param Point pos : Coordonnées de la ville
 * \param Str name : Nom de la ville
 * \return void
 */

void guiAddPoint(Point pos, Str name)
{
    if(gCt.len>=gCt.size)
    {
        gCt.size*=2;
        gCt.pos=realloc(gCt.pos, gCt.size*sizeof(Point));
        gCt.name=realloc(gCt.name, gCt.size*sizeof(Str));
    }

    gCt.pos[gCt.len]=pos;
    gCt.name[gCt.len]=strCopy(name);

    gCt.len++;
}

/**
 * \fn void guiAddLine(Point pos1, Point pos2, double l)
 * \brief Ajoute une flèche dans le tampon des chemins
 * \param Point pos1 : Coordonnées de départ de la flèche
 * \param Point pos2 : Coordonnées d'arrivée de la flèche
 * \param double l   : longueur de la flèche
 * \return void
 */

void guiAddLine(Point pos1, Point pos2, double l)
{
    if(gCtl.len>=gCtl.size)
    {
        gCtl.size*=2;
        gCtl.pos1=realloc(gCtl.pos1, gCtl.size*sizeof(Point));
        gCtl.pos2=realloc(gCtl.pos2, gCtl.size*sizeof(Point));
        gCtl.l=realloc(gCtl.l, gCtl.size*sizeof(double));
    }

    gCtl.pos1[gCtl.len]=pos1;
    gCtl.pos2[gCtl.len]=pos2;
    gCtl.l[gCtl.len]=l;

    gCtl.len++;
}

/**
 * \fn void guiAddLabel(Str str)
 * \brief Ajoute un texte dans le tampon des labels
 * \param char *str : Texte du label
 * \return void
 */

void guiAddLabel(Str str)
{
    if(gCtt.len>=gCtt.size)
    {
        gCtt.size*=2;
        gCtt.lab=realloc(gCtt.lab, gCtt.size*sizeof(Str));
    }

    gCtt.lab[gCtt.len]=strCopy(str);

    gCtt.len++;
}

/**
 * \fn void guiAddMap(Map m)
 * \brief Ajoute toutes les coordonnées des villes d'une Map dans le tampon de coordonnées
 * \param Map m : Map à ajouter
 * \return void
 */

void guiAddMap(Map m)
{
    if(!mapGetIsPos(m))
        throwWarn("Map", "Some cities in the map have no positions (mapDraw)", NULL);
    else
    {
        gCt.m=m;
        for(int i=0; i<mapGetSize(m); i++)
        {
            Str str=malloc(20*sizeof(char));
            itoa(i+1, str, 10);
            guiAddPoint(cityGetPos(mapGetCity(m, i)), str);
            free(str);
        }
    }
}

/**
 * \fn void guiAddPath(Map m, City *path, double exTime)
 * \brief Ajoute toutes les flèches d'un chemin ainsi que les labels de la distance totale et du temps d'éxecution
 * \param Map m : Map contenant les villes du parcours
 * \param City *path : liste ordonnées de villes décrivant le chemin
 * \param double exTime : Temps d'execution
 * \return void
 */

void guiAddPath(Map m, City *path, double exTime)
{
    for(int i=0; i<mapGetSize(m)-1; i++)
    {
        guiAddLine(cityGetPos(path[i]), cityGetPos(path[i+1]), cityGetDist(path[i], cityGetIndex(path[i+1])));
    }

    guiAddLine(cityGetPos(path[mapGetSize(m)-1]), cityGetPos(path[0]), cityGetDist(path[mapGetSize(m)-1], cityGetIndex(path[0])));

    double s=calcPathLength(m, path);

    Str lab=malloc(1000*sizeof(char));
    sprintf(lab, "Total size: %f", s);
    guiAddLabel(lab);
    //free(lab);

    //lab=malloc(1000*sizeof(char));
    sprintf(lab, "Exec time: %.3f ms", exTime);
    guiAddLabel(lab);
    free(lab);

    guiUpdColors();
}

/**
 * \fn void guiDraw()
 * \brief Dessine tout ce qui se trouve dans les tampons
 * \param void
 * \return void
 */

void guiDraw()
{
    int offsetMin=10;
    int offsetMax=10;
    int sizey=WINSY-offsetMin-offsetMax;
    int sizex=WINSX-offsetMin-offsetMax;

    Point min, max, tmp;

    Point pts[gCt.len];

    Point pos1[gCtl.len];
    Point pos2[gCtl.len];

    min=gCt.pos[0];
    max=min;

    for(int i=1; i<gCt.len; i++)
    {
        tmp=gCt.pos[i];
        if(tmp.x<min.x)
            min.x=tmp.x;
        if(tmp.y<min.y)
            min.y=tmp.y;
        if(tmp.x>max.x)
            max.x=tmp.x;
        if(tmp.y>max.y)
            max.y=tmp.y;
    }

    for(int i=0; i<gCtl.len; i++)
    {
        tmp=gCtl.pos1[i];
        if(tmp.x<min.x)
            min.x=tmp.x;
        if(tmp.y<min.y)
            min.y=tmp.y;
        if(tmp.x>max.x)
            max.x=tmp.x;
        if(tmp.y>max.y)
            max.y=tmp.y;

        tmp=gCtl.pos2[i];
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

    double coef;

    if(coefx<coefy)
        coef=coefx;
    else
        coef=coefy;

    for(int i=0; i<gCt.len; i++)
    {
        pts[i]=gCt.pos[i];
        pts[i].x-=min.x;
        pts[i].y-=min.y;
        pts[i].x*=coef;
        pts[i].y*=coef;
        pts[i].x+=offsetMin;
        pts[i].y+=offsetMin;
    }

    for(int i=0; i<gCtl.len; i++)
    {
        pos1[i]=gCtl.pos1[i];
        pos2[i]=gCtl.pos2[i];

        pos1[i].x-=min.x;
        pos1[i].y-=min.y;
        pos1[i].x*=coef;
        pos1[i].y*=coef;
        pos1[i].x+=offsetMin;
        pos1[i].y+=offsetMin;

        pos2[i].x-=min.x;
        pos2[i].y-=min.y;
        pos2[i].x*=coef;
        pos2[i].y*=coef;
        pos2[i].x+=offsetMin;
        pos2[i].y+=offsetMin;
    }

    for(int i=0; i<gCtl.len; i++)
        drawPath(pos1[i].x, pos1[i].y, pos2[i].x, pos2[i].y, gCtl.l[i]);

    for(int i=0; i<gCt.len; i++)
        drawCity(pts[i].x, pts[i].y, gCt.name[i]);

    for(int i=0; i<gCtt.len; i++)
        drawLabel(gCtt.lab[i]);
}

/**
 * \fn void guiShow()
 * \brief Affiche la fenêtre et dessine ce qui se trouve dans les tampons
 * \param void
 * \return void
 */

void guiShow()
{

    showSplashScreen("tsplogo.png",2000,700,500);

    guiSysSendMap(gCt.m);


    guiInitWindow();


    guiDraw();


    gtk_main();

    guiFree();
}


/* Close the splash screen */
gboolean close_screen(gpointer data)
{
    gtk_widget_destroy((GtkWidget*)data);
    gtk_main_quit ();
    return(FALSE);
}

/**
 * \fn void guiShow()
 * \brief Affiche le logo du groupe avant de lancer la GUI
 * \param char* image_name : Nom de l'image à charger.
 * \param int time : Durée pendant laquelle l'image est affichée.
 * \param int width : Largeur de l'image.
 * \param int height : Longueur de l'image.
 * \return void
 */

void showSplashScreen(char *image_name, int time, int width, int height)
{
    GtkWidget *image, *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (window, width, height);
    gtk_window_set_decorated(GTK_WINDOW (window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    image=gtk_image_new_from_file(image_name);
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all (window);
    g_timeout_add (time, close_screen, window);

    gtk_main ();
}
