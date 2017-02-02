/**
 * \file gui/guisys.c
 * \brief Fichier d'implémentation de l'interface graphique à bas niveau.
 * \author Jason Pindat
 * \version 0.64
 * \date 26/03/2014
 */

#include <gtk/gtk.h>
#include <pango/pango.h>
#include <math.h>
#include <stdlib.h>
#include "guisys.h"
#include "gui.h"
#include "../string.h"
#include "../map.h"
#include "../city.h"
#include "../fcts.h"
#include "../algos/algos.h"
#include "../tsp.h"
#define NBCOLORS 5
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
typedef struct _components
{
    GtkWidget *window;
    GtkWidget *openButton;
    GtkWidget *tourButton;
    GtkWidget *nnButton;
    GtkWidget *mstButton;
    GtkWidget *bfButton;
    GtkWidget *bbButton;
    GtkWidget *bbrButton;
    GtkWidget *bfrecButton;
    GtkWidget *bfmtButton;
    GtkWidget *bbrhkButton;
    GtkWidget *sCityLabel;
    GtkWidget *sCityEntry;
    GtkWidget *randomEntryLabel;
    GtkWidget *randomInstanceButton;
    GtkWidget *randomEntry;
    GtkWidget *err;
    GtkWidget *hspacer;
    GtkWidget *spacer;
    //GtkWidget *next;
    GtkWidget *dialog;
    GtkWidget *graphNavigationPanel;
    GtkWidget *graphNavigationPanel2;
    GtkWidget *navigPanel;
    GtkWidget *mainPanel;
    GtkWidget *drawingArea;
    GdkPixmap *pixMap;
} components;
GdkGC **contexts;
GdkColor *colors;
int iColors;
int offsetLabel;
static components this;
Map sysM;
/*static void next_handler( GtkWidget *widget,
                   gpointer   data )
{
    g_print ("next %s \n", (gchar *) data);
}*/

bool showQuestionWarning(GtkWidget *widget, gpointer window, char *title, char* msg)
{
    bool ret=0;
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    msg);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    if ( (gtk_dialog_run(GTK_DIALOG(dialog)))==GTK_RESPONSE_YES )
        ret=1;
    else
        ret=0;
    gtk_widget_destroy(dialog);
    return ret;
}

void showWarnDialog(GtkWidget *widget, gpointer window, char* title, char* msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_WARNING,
                                    GTK_BUTTONS_OK,
                                    msg);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
static bool verifyMapLoaded()
{
    if(sysM==NULL)
    {
        throwWarn("GUI System", "No map loaded", NULL);
        showWarnDialog(this.err, this.window, "No file loaded", "Please open a file.");
        return 0;
    }
    return 1;

}
static int getStartCity()
{
    return atoi(gtk_entry_get_text(GTK_ENTRY(this.sCityEntry)));
}
static void buttonAct(int nbAlgo, int startCity)
{
    guiFreePaths();
    guiFreeLabels();
    bool go=1;
    if( (mapGetSize(sysM)>=14) && (nbAlgo>=2) && nbAlgo<=6 )
    {
        go=showQuestionWarning(this.dialog, this.window, "Warning about possible long calculations", "According to the number of cities in the Map, running this algorithm\nwill take some time.\nDo you want to run it?");
    }
    if(go)
    {
        bool algos[NB_ALGOS];
        for(int i=0; i<NB_ALGOS; i++)
        {
            if(i==nbAlgo)
                algos[i]=true;
            else
                algos[i]=false;
        }
        executeAlgos(sysM, algos, startCity, true, false);//, false);
        refreshDisplay();
        guiDraw();
    }


}
static void openButton_handler( GtkWidget *widget,
                                gpointer   data )
{
    //buttonAct(0, 0);
    this.dialog = gtk_file_chooser_dialog_new ("Open File",
                  (GtkWindow *)this.window,
                  GTK_FILE_CHOOSER_ACTION_OPEN,
                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                  NULL);
    if (gtk_dialog_run (GTK_DIALOG (this.dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (this.dialog));
        printf("filename : %s \n",filename);
        Map m=tspLoad(filename);
        if(m!=NULL)
        {
            if(mapGetIsPos(m))
            {
                if(mapGetSize(m)>1)
                {
                    if(sysM)
                        mapDeleteRec(sysM);
                    sysM=m;
                    guiFree();
                    refreshDisplay();
                    guiAddMap(sysM);
                    guiDraw();
                }
                else
                {
                    showWarnDialog(this.err, this.window, "Map : not enough cities", "Please load a map with more cities (have 1 only).");
                }
            }
            else {
                showWarnDialog(this.err, this.window, "No display", "There isn't any display data in this map, unable to use it in graphic mode.");
            }
        }
        else
        {
            showWarnDialog(this.err, this.window, "Incorrect TSP File", "TSP File is incorrect.\nPlease check TSPLIB Format : EDGE_WEIGHT_FORMAT : FULL_MATRIX");
        }
        g_free (filename);

    }
    gtk_widget_destroy (this.dialog);
}

static void tourButton_handler( GtkWidget *widget,
                                gpointer   data )
{
    if(verifyMapLoaded())
    {
        bool done=false;
        for(int i=0; i<NB_ALGOS; i++)
        {
            if(!done && mapGetPath(sysM, i))
            {

                bool algosS[NB_ALGOS];
                for(int j=0; j<NB_ALGOS; j++)
                    algosS[j]=false;

                executeAlgos(sysM, algosS, mapGetStartCity(sysM), false, true);

                done=true;
            }
        }
        if(!done)
        {
            showWarnDialog(this.err, this.window, "No algorithm executed yet", "Please execute at least 1 algorithm to write a tour file");
        }
        else
        {
            showWarnDialog(this.err, this.window, "Successfully saved", "File as been saved as .tour file.");
        }
    }

}

static void randomInstanceButton_handler( GtkWidget *widget,
        gpointer   data )
{
    int nb=atoi(gtk_entry_get_text(GTK_ENTRY(this.randomEntry)));
    if(nb>1 && nb<=99999)
    {
        if(sysM)
            mapDeleteRec(sysM);
        sysM=mapCreateRandom(nb);
        guiFree();
        refreshDisplay();
        guiAddMap(sysM);
        guiDraw();

    }
    else
    {
        showWarnDialog(this.err, this.window, "Wrong amount for Random Instance", "Please put a valid amount for random Instance. (2-99999)");
    }

}

static void nnButton_handler( GtkWidget *widget,
                              gpointer   data )
{

    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(0,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }

}
static void mstButton_handler( GtkWidget *widget,
                               gpointer   data )
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(1,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }


}
static void bfButton_handler( GtkWidget *widget,
                              gpointer   data )
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(2,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }


}
static void bfrecButton_handler( GtkWidget *widget,
                                 gpointer   data )
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(4,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }


}
static void bfmtButton_handler( GtkWidget *widget,
                                gpointer   data )
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(5,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }


}
static void bbButton_handler( GtkWidget *widget,
                              gpointer   data )
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(3,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }


}
static void bbrButton_handler( GtkWidget *widget,
                               gpointer   data )
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(6,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }


}
static void bbrhkButton_handler( GtkWidget *widget, gpointer data)
{
    if(verifyMapLoaded())
    {
        int sC=getStartCity();
        if(sC>0 && sC<=mapGetSize(sysM))
            buttonAct(7,sC-1);
        else
            showWarnDialog(this.err,this.window,"Wrong Start City", "The specified start city is out of bounds");
    }
}
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    return FALSE;
}
/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}
/* Draw a rectangle on the screen */
/*static void
draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
  const int size=3;*/
/*gdk_draw_rectangle (this.pixMap,
              widget->style->black_gc,
              TRUE,
            update_rect.x, update_rect.y,
            update_rect.width, update_rect.height);*/
/*gdk_draw_line (this.pixMap, widget->style->black_gc, x-size, y-size, x+size, y+size);
gdk_draw_line (this.pixMap, widget->style->black_gc, x-size, y+size, x+size, y-size);
int width, height;
PangoLayout *layout = gtk_widget_create_pango_layout(this.window, "Toto");
//pango_layout_set_text (layout, "Salut", -1);
PangoFontDescription *fontdesc = pango_font_description_from_string("Sans 10");
pango_layout_set_font_description (layout, fontdesc);
pango_layout_get_pixel_size (layout, &width, &height);
gdk_draw_layout(this.pixMap, widget->style->black_gc, x+5, y+5, layout);
gtk_widget_queue_draw_area (widget,
                            x-size, y-size,
                    size+1+5+width, size+1+5+height);
//free(fontdesc);
//free(layout);
}*/
/* Redraw the screen from the backing pixmap */
static gboolean
expose_event( GtkWidget *widget, GdkEventExpose *event )
{
    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      this.pixMap,
                      event->area.x, event->area.y,
                      event->area.x, event->area.y,
                      event->area.width, event->area.height);
    return FALSE;
}
/* Create a new backing pixmap of the appropriate size */
static gboolean
configure_event( GtkWidget *widget, GdkEventConfigure *event )
{
    if (this.pixMap)
        g_object_unref(this.pixMap);
    this.pixMap = gdk_pixmap_new(widget->window,
                                 widget->allocation.width,
                                 widget->allocation.height,
                                 -1);
    gdk_draw_rectangle (this.pixMap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);
    return TRUE;
}
/*static gboolean
button_press_event( GtkWidget *widget, GdkEventButton *event )
{
  if (event->button == 1 && this.pixMap != NULL)
      draw_brush (widget, event->x, event->y);
  return TRUE;
}*/
/*static gboolean
motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
  int x, y;
  GdkModifierType state;
  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = event->state;
    }
  if (state & GDK_BUTTON1_MASK && this.pixMap != NULL)
    draw_brush (widget, x, y);
  return TRUE;
}*/
static void createButtons()
{
    //this.next = gtk_button_new_with_label ("Next");
    this.openButton= gtk_button_new_with_label ("Open TSP");
    this.tourButton= gtk_button_new_with_label ("Write best executed tour");
    this.randomInstanceButton = gtk_button_new_with_label ("Create random instance");
    this.randomEntryLabel = gtk_label_new("Amount of random cities : ");
    this.randomEntry = gtk_entry_new_with_max_length(5);
    gtk_entry_set_text(GTK_ENTRY(this.randomEntry),"10");
    gtk_entry_set_width_chars(GTK_ENTRY(this.randomEntry),5);
    this.spacer=gtk_vseparator_new();
    this.hspacer=gtk_hseparator_new();
    this.sCityLabel = gtk_label_new("Start City: ");
    this.sCityEntry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(this.sCityEntry),"1");
    gtk_entry_set_width_chars(GTK_ENTRY(this.sCityEntry),5);
    this.nnButton = gtk_button_new_with_label ("Nearest Neighbour");
    this.mstButton = gtk_button_new_with_label ("Minimum Spanning Tree");
    this.bfButton = gtk_button_new_with_label ("Brute Force");
    this.bfrecButton = gtk_button_new_with_label ("Recursive Brute Force");
    this.bfmtButton = gtk_button_new_with_label ("MultiThreaded Brute Force");
    this.bbButton = gtk_button_new_with_label ("Branch and Bound");
    this.bbrButton = gtk_button_new_with_label ("Branch and Bound NN-MST");
    this.bbrhkButton = gtk_button_new_with_label ("Branch and Bound Held-Karp");

    //g_signal_connect (G_OBJECT (this.next), "clicked", G_CALLBACK (next_handler), NULL);
    g_signal_connect (G_OBJECT (this.openButton), "clicked", G_CALLBACK (openButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.tourButton), "clicked", G_CALLBACK (tourButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.randomInstanceButton), "clicked", G_CALLBACK (randomInstanceButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.nnButton), "clicked", G_CALLBACK (nnButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.mstButton), "clicked", G_CALLBACK (mstButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.bfButton), "clicked", G_CALLBACK (bfButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.bfrecButton), "clicked", G_CALLBACK (bfrecButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.bfmtButton), "clicked", G_CALLBACK (bfmtButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.bbButton), "clicked", G_CALLBACK (bbButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.bbrButton), "clicked", G_CALLBACK (bbrButton_handler), NULL);
    g_signal_connect (G_OBJECT (this.bbrhkButton), "clicked", G_CALLBACK (bbrhkButton_handler), NULL);
}
void createWindow()
{
    this.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (this.window), 10);
    gtk_window_set_title (GTK_WINDOW (this.window), "Traveling Salesman Problem Solver");
    g_signal_connect (G_OBJECT (this.window), "delete_event",
                      G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (this.window), "destroy",
                      G_CALLBACK (destroy), NULL);
    gtk_window_set_resizable(GTK_WINDOW(this.window), FALSE);
}
void layoutWidgets()
{
    /* Create the graph navigation panel and add it to the window. */
    this.graphNavigationPanel = gtk_hbox_new (FALSE, 0);
    this.graphNavigationPanel2 = gtk_hbox_new (FALSE, 0);
    this.navigPanel = gtk_hbox_new(FALSE, 0);
    this.mainPanel = gtk_vbox_new(FALSE, 0);
    gtk_container_add  (GTK_CONTAINER (this.window), this.mainPanel);
    gtk_box_pack_start (GTK_BOX(this.navigPanel), this.openButton, FALSE,FALSE,0);
    gtk_box_pack_start (GTK_BOX(this.navigPanel), this.tourButton, FALSE,FALSE,0);
    gtk_box_pack_start (GTK_BOX(this.navigPanel), this.spacer, FALSE, FALSE, 10);
    gtk_box_pack_start (GTK_BOX(this.navigPanel), this.randomInstanceButton, FALSE, FALSE , 0);
    gtk_box_pack_start (GTK_BOX(this.navigPanel), this.randomEntryLabel, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(this.navigPanel), this.randomEntry, FALSE, FALSE, 0);
    /* Add the buttons to the graph navigation panel. */
    //  gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.openButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.sCityLabel, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.sCityEntry, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.nnButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.mstButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.bfButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel2), this.bfrecButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel2), this.bfmtButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.bbButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel2), this.bbrButton, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel2), this.bbrhkButton, TRUE, TRUE, 0);

    //  gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.hspacer, TRUE, TRUE, 0);
    //gtk_box_pack_start (GTK_BOX(this.graphNavigationPanel), this.next,     TRUE, TRUE, 0);

    //Add navigPanel to mainPanel
    gtk_box_pack_start (GTK_BOX(this.mainPanel), this.navigPanel, TRUE,TRUE,0);
    /*Add the graph navigation panel to the main panel. */
    gtk_box_pack_start (GTK_BOX(this.mainPanel), this.graphNavigationPanel, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX(this.mainPanel), this.graphNavigationPanel2, TRUE, TRUE, 0);


    /* Add the draw-able area to the main panel. */
    gtk_box_pack_start (GTK_BOX(this.mainPanel), this.hspacer, TRUE, TRUE, 10);

    gtk_box_pack_start (GTK_BOX(this.mainPanel), this.drawingArea, TRUE, TRUE, 0);

}
void show()
{
    gtk_widget_show (this.drawingArea);
    gtk_widget_show (this.mainPanel);
    //gtk_widget_show (this.next);
    gtk_widget_show (this.openButton);
    gtk_widget_show (this.tourButton);
    gtk_widget_show (this.spacer);
    gtk_widget_show(this.hspacer);
    gtk_widget_show (this.randomInstanceButton);
    gtk_widget_show (this.randomEntryLabel);
    gtk_widget_show (this.randomEntry);
    gtk_widget_show (this.sCityLabel);
    gtk_widget_show (this.sCityEntry);
    gtk_widget_show (this.nnButton);
    gtk_widget_show (this.mstButton);
    gtk_widget_show (this.bfButton);
    gtk_widget_show (this.bfrecButton);
    gtk_widget_show (this.bfmtButton);
    gtk_widget_show (this.bbButton);
    gtk_widget_show (this.bbrButton);
    gtk_widget_show (this.bbrhkButton);
    gtk_widget_show (this.graphNavigationPanel);
    gtk_widget_show (this.graphNavigationPanel2);
    gtk_widget_show (this.navigPanel);
    gtk_widget_show (this.window);
}
void createDrawingArea()
{
    this.drawingArea = gtk_drawing_area_new();
    gtk_signal_connect (GTK_OBJECT (this.drawingArea), "expose_event",
                        (GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT(this.drawingArea),"configure_event",
                        (GtkSignalFunc) configure_event, NULL);
    /*gtk_signal_connect (GTK_OBJECT (this.drawingArea), "motion_notify_event",
                  (GtkSignalFunc) motion_notify_event, NULL);
    gtk_signal_connect (GTK_OBJECT (this.drawingArea), "button_press_event",
                  (GtkSignalFunc) button_press_event, NULL);*/
    gtk_widget_set_events (this.drawingArea, GDK_EXPOSURE_MASK
                           | GDK_LEAVE_NOTIFY_MASK
                           | GDK_BUTTON_PRESS_MASK
                           | GDK_POINTER_MOTION_MASK
                           | GDK_POINTER_MOTION_HINT_MASK);
    gtk_drawing_area_size((GtkDrawingArea*)this.drawingArea, WINSX, WINSY);
}

/*void drawCity (gdouble x, gdouble y, char *name)
{
    const int size=3;
    gdk_draw_line (this.pixMap, this.drawingArea->style->black_gc, x-size, y-size, x+size, y+size);
    gdk_draw_line (this.pixMap, this.drawingArea->style->black_gc, x-size, y+size, x+size, y-size);
    int width, height;
    PangoLayout *layout = gtk_widget_create_pango_layout(this.window, name);
    PangoFontDescription *fontdesc = pango_font_description_from_string("Sans 10");
    pango_layout_set_font_description (layout, fontdesc);
    pango_layout_get_pixel_size (layout, &width, &height);
    gdk_draw_layout(this.pixMap, this.drawingArea->style->black_gc, x+5, y+5, layout);
    gtk_widget_queue_draw_area (this.drawingArea,
                                x-size, y-size,
                                size+1+5+width, size+1+5+height);
}*/

void drawCity (gdouble x, gdouble y, char *name)
{
    int width, height, circle=64*360;
    int offsetx, offsety;
    PangoFontDescription *fontdesc;

    if(strLength(name)==1)
    {
        fontdesc = pango_font_description_from_string("Sans 10");
        offsetx=6;
        offsety=3;
    }
    else if(strLength(name)==2)
    {
        fontdesc = pango_font_description_from_string("Sans 9");
        offsetx=2;
        offsety=3;
    }
    else if(strLength(name)==3)
    {
        fontdesc = pango_font_description_from_string("Sans 7");
        offsetx=2;
        offsety=5;
    }
    else
    {
        fontdesc = pango_font_description_from_string("Sans 5");
        offsetx=2;
        offsety=6;
    }

    gdk_draw_arc (this.pixMap, this.drawingArea->style->black_gc, true, x-10, y-10, 20, 20, 0, circle);
    //gdk_draw_arc (this.pixMap, this.drawingArea->style->black_gc, false, x-10, y-10, 20, 20, 0, 64*360);

    PangoLayout *layout = gtk_widget_create_pango_layout(this.window, name);
    pango_layout_set_font_description (layout, fontdesc);
    pango_layout_get_pixel_size (layout, &width, &height);
    gdk_draw_layout(this.pixMap, this.drawingArea->style->white_gc, x-10+offsetx+1, y-10+offsety+1, layout);
    pango_font_description_free(fontdesc);
    g_object_unref(layout);

    gtk_widget_queue_draw_area (this.drawingArea, x-10, y-10, 21, 21);
}

void drawPath(gdouble x, gdouble y, gdouble x2, gdouble y2, double l)
{
    if(x!=x2 || y!=y2)
    {
        double alpha=atan2(y2-y, x2-x);
        double offsetdx=cos(alpha)*10;
        double offsetdy=sin(alpha)*10;
        double offsetax=cos(alpha)*15;
        double offsetay=sin(alpha)*15;
        double arrow1x=cos(alpha+M_PI_2+M_PI_4)*10+x2-offsetax;
        double arrow1y=sin(alpha+M_PI_2+M_PI_4)*10+y2-offsetay;
        double arrow2x=cos(alpha-M_PI_2-M_PI_4)*10+x2-offsetax;
        double arrow2y=sin(alpha-M_PI_2-M_PI_4)*10+y2-offsetay;
        //gtk_widget_modify_fg(this.drawingArea, GTK_STATE_NORMAL, &colors[iColors]);
        //gdk_gc_set_foreground(this.drawingArea->style->fg_gc[GTK_STATE_NORMAL], &colors[iColors]);
        gdk_draw_line (this.pixMap, this.drawingArea->style->black_gc, x+offsetdx, y+offsetdy, x2-offsetax, y2-offsetay); //fg_gc[GTK_STATE_NORMAL]
        gdk_draw_line (this.pixMap, this.drawingArea->style->black_gc, arrow1x, arrow1y, x2-offsetax, y2-offsetay);
        gdk_draw_line (this.pixMap, this.drawingArea->style->black_gc, arrow2x, arrow2y, x2-offsetax, y2-offsetay);
        gtk_widget_queue_draw_area (this.drawingArea, x, y, x2-x+1, y2-y+1);

        if(l>=0)
        {
            int toffsetx=cos(alpha+M_PI_2)*12;
            int toffsety=sin(alpha+M_PI_2)*12;
            int posx=(x+x2)/2;
            int posy=(y+y2)/2;
            int width, height;

            if(toffsetx<0)
            {
                posx-=toffsetx;
                posy-=toffsety;
            }
            else
            {
                posx+=toffsetx;
                posy+=toffsety;
            }

            char lstr[40];
            sprintf(lstr, "%.3f", l);

            PangoLayout *layout = gtk_widget_create_pango_layout(this.window, lstr);
            PangoFontDescription *fontdesc = pango_font_description_from_string("Sans 7");
            pango_layout_set_font_description (layout, fontdesc);
            pango_layout_get_pixel_size (layout, &width, &height);
            gdk_draw_layout(this.pixMap, this.drawingArea->style->black_gc, posx, posy, layout);
            pango_font_description_free(fontdesc);
            g_object_unref(layout);

            gtk_widget_queue_draw_area (this.drawingArea, posx, posy, width+1, height+1);
        }
    }
}
void drawLabel(Str str)
{
    int width, height;
    PangoLayout *layout = gtk_widget_create_pango_layout(this.window, str);
    PangoFontDescription *fontdesc = pango_font_description_from_string("Sans 10");
    pango_layout_set_font_description (layout, fontdesc);
    pango_layout_get_pixel_size (layout, &width, &height);
    gdk_draw_layout(this.pixMap, this.drawingArea->style->black_gc, 10, WINSY-25-offsetLabel, layout);
    pango_font_description_free(fontdesc);
    g_object_unref(layout);

    gtk_widget_queue_draw_area (this.drawingArea,
                                10, WINSY-25-offsetLabel,
                                width+1, height+1);
    offsetLabel+=height+5;
}
void guiSysInit()
{
    offsetLabel=0;
    colors=malloc(NBCOLORS*sizeof(GdkColor));
    //contexts=malloc(NBCOLORS*sizeof(GdkGC*));
    /*for(int i=0; i<NBCOLORS; i++)
        contexts[i]=gdk_gc_new(GDK_DRAWABLE(this.drawingArea));*/
    gdk_color_parse ("blue", &colors[0]);
    gdk_color_parse ("red", &colors[1]);
    gdk_color_parse ("brown", &colors[2]);
    gdk_color_parse ("green", &colors[3]);
    gdk_color_parse ("purple", &colors[4]);
    /*gdk_gc_set_foreground(&contexts[0], &colors[0]);
    gdk_gc_set_foreground(&contexts[1], &colors[1]);
    gdk_gc_set_foreground(&contexts[2], &colors[2]);
    gdk_gc_set_foreground(&contexts[3], &colors[3]);
    gdk_gc_set_foreground(&contexts[4], &colors[4]);*/
    iColors=0;
    sysM=NULL;
}
void guiSysStop()
{
    free(colors);
    //free(contexts);
}
void guiUpdColors()
{
    iColors++;
    iColors%=NBCOLORS;
}
void guiInitWindow()
{
    createWindow();
    createButtons();
    createDrawingArea();
    layoutWidgets();
    show();
}
void guiSysSendMap(Map m)
{
    sysM=m;
}
void guiSysFreeLabels()
{
    offsetLabel=0;
}
void refreshDisplay()
{
    gdk_draw_rectangle (this.pixMap, this.drawingArea->style->white_gc, true, 0, 0, WINSX, WINSY);
    gtk_widget_queue_draw_area (this.drawingArea, 0, 0, WINSX, WINSY);
}
