/**
 * \file string.h
 * \brief Fichier écrit avant le projet par Jason pour gérer plus facilement les chaînes de caractères.
 * \author Jason Pindat
 * \version
 * \date 2012-2013
 */

#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <string.h>

typedef char *Str;

int indexOfFrom(Str, char, int);
int indexOf(Str, char);
int lastIndexOfFrom(Str, char, int);
int lastIndexOf(Str, char);
Str subString(Str, int, int);
int strLength(Str);
void strSetLength(Str, int);
bool isEOS(char);
bool isEOL(char);
bool isSpaceChar(char);
bool isSepChar(char);
Str strCopy(Str);
bool strCmp(Str, Str);
bool strArrayCmp(Str, Str *, int);
bool isMajChar(char);
bool isMinChar(char);
bool isLetterOrNumber(char);
bool isNumber(char);
bool isClassName(Str);
bool isVarName(Str);
int findNext(Str, char, int);
void itoa(int, Str, int);
double Atof(Str);

#endif
