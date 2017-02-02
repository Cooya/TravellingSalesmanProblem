/**
 * \file gui/guisys.h
 * \brief Fichier d'en-tête de l'interface graphique à bas niveau.
 * \author Jason Pindat
 * \version 0.64
 * \date 26/03/2014
 */

#ifndef GUISYS_H
#define GUISYS_H

#include "../map.h"
#include "../string.h"

#define WINSX 800
#define WINSY 800

void createWindow();
void layoutWidgets();
void show();
void createDrawingArea();
void drawCity(double, double, char*);
void drawPath(double, double, double, double, double);
void drawLabel(Str);
void guiSysInit();
void guiSysStop();
void guiUpdColors();
void guiInitWindow();
void guiSysSendMap(Map);
void guiSysFreeLabels();
void refreshDisplay();

#endif


