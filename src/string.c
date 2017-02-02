/**
 * \file string.c
 * \brief Fichier écrit avant le projet par Jason pour gérer plus facilement les chaînes de caractères.
 * \author Jason Pindat
 * \version
 * \date 2012-2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "string.h"

const char EOS = '\0';
const char EOL = '\n';

int indexOfFrom(Str str, char c, int index)
{
    int i=0;

    while(*str!=EOS && i<index)
    {
        str++;
        i++;
    }
    if(*str==EOS)
        return -1;

    while(*str!=EOS && *str!=c)
    {
        str++;
        i++;
    }
    if(*str==EOS)
        return -1;
    else
        return i;
}

int indexOf(Str str, char c)
{
    return indexOfFrom(str, c, 0);
}

int lastIndexOfFrom(Str str, char c, int index)
{
    char *ptr=str;

    if(index>=0)
    {
        while(*ptr!=EOS && (ptr-str)<=index)
            ptr++;
        ptr--;
    }
    else
    {
        while(*ptr!=EOS)
            ptr++;
        ptr--;
        while(index<0)
        {
            ptr--;
            index++;
        }
    }

    while(ptr>=str && *ptr!=c)
        ptr--;

    if(ptr<str)
        return -1;
    else
        return ptr-str;
}

int lastIndexOf(Str str, char c)
{
    char *ptr=str;

    while(*ptr!=EOS)
        ptr++;

    while(ptr>=str && *ptr!=c)
        ptr--;

    if(ptr<str)
        return -1;
    else
        return ptr-str;
}

Str subString(Str str, int from, int to)   //!\\ doit etre free derriere
{
    if(to<=from)
    {
        to=0;
        from=0;
    }
    Str substr = (Str)malloc((to-from+1)*sizeof(char));
    strncpy(substr, str+from, to-from);
    substr[to-from]=EOS;
    return substr;
}

int strLength(Str str)
{
    return strlen(str);
}

void strSetLength(Str str, int p)
{
    str[p]='\0';
}

bool isEOS(char c)
{
    return c==EOS;
}

bool isEOL(char c)
{
    return isEOS(c) || c==EOL || c=='\r';
}

bool isSpaceChar(char c)
{
    return c==' ' || c=='\t' || c==EOL || c=='\r';
}

bool isSepChar(char c)
{
    return c=='=' || c=='+' || c=='-' || c=='*' || c=='/' || c=='&' || c=='|' || c=='!' || c==';' || c==':' || c==',' || c=='?' || c=='(' || c==')' || c=='[' || c==']' || c=='{' || c=='}' || isSpaceChar(c);
}

Str strCopy(Str str)
{
    int length=strLength(str);
    Str str2 = malloc((length+1)*sizeof(char));

    for(int i=0; i<length; i++)
    {
        str2[i]=str[i];
    }

    str2[length]=EOS;
    return str2;
}

bool strCmp(Str str1, Str str2)
{
    /*while(*str1!=EOS && *str2!=EOS && *str1==*str2) {
        str1++;
        str2++;
    }
    if(*str1==EOS && *str2==EOS)
        return true;
    return false;*/
    return strcmp(str1, str2)==0;
}

bool strArrayCmp(Str str, Str *arr, int arrSize)
{
    for(int i=0; i<arrSize; i++)
    {
        if(strCmp(str, arr[i]))
            return true;
    }
    return false;
}

bool isMajChar(char c)
{
    return c>='A' && c<='Z';
}

bool isMinChar(char c)
{
    return c>='a' && c<='z';
}

bool isLetterOrNumber(char c)
{
    return (c>='0' && c<='9') || isMinChar(c) || isMajChar(c);
}

bool isNumber(char c)
{
    return (c>='0' && c<='9') || c=='.' || c=='-' || c==',';
}

/*bool isClassName(Str str) {
    int length=strLength(str);

    if(length==0)
        return false;

    if(!isMajChar(str[0]))
        return false;

    for(int i=1; i<length; i++)
        if(!isLetterOrNumber(str[i]))
            return false;

    return true;
}

bool isVarName(Str str) {
    int length=strLength(str);

    if(length==0)
        return false;

    if(!isMinChar(str[0]))
        return false;

    for(int i=1; i<length; i++)
        if(!isLetterOrNumber(str[i]))
            return false;

    return true;
}*/

int findNext(Str str, char c, int index)
{
    bool ok=true;
    int parCpt=0;
    int accCpt=0;

    while(str[index]!='\0' && !(str[index]==c && ok))
    {
        if(str[index]=='[')
            accCpt++;
        else if(str[index]==']')
            accCpt--;
        else if(str[index]=='(')
            parCpt++;
        else if(str[index]==')')
            parCpt--;

        ok=(accCpt==0 && parCpt==0);

        index++;
    }

    printf("FindNext Result : par%d acc%d cfound%d\n", parCpt, accCpt, str[index]==c);

    if(str[index]!=c)
        return -1;
    return index;
}

void itoa(int entier, Str ptr, int base)
{
    sprintf(ptr, "%d", entier);
}

double Atof(Str str)
{
    if(indexOf(str, '.')==-1)
        return (double)atoi(str);
    else
        return atof(str);
}

//itoa(blockId, buff+1, 10);
//sprintf(buff+1, "%d", blockId);
