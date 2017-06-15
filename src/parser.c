/** @file
   Implementacja Parsera Wielomianow

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/

#include <stdbool.h>
#include "poly.h"
#include <malloc.h>
#include <memory.h>
#include "parser.h"
#include <limits.h>

#define NUMBER_OF_COMAND 15

bool parseMono(const char *line, int *column);

bool parseListOfMono(const char *line, int *column);

bool parseExp(const char *line, int *column);

bool parsePolyPom(const char *line, int *column);

bool recognisePoly(char *line)
{
    char c = line[0];
    if (c <= 'z' && c >= 'A')
    {
        return false;
    }
    else
    {
        return true;
    }
}



/**
 * Funkcja parsuje liste jednomianow
 * @param line : tablica charow
 * @param column : index
 * @return czy jest poprawna lista jednomianow
 */
bool parseListOfMono(const char *line, int *column)
{
    bool partResult;

    while (1) //wczytujemy do momentu az wczytamy wszystko lub bedzie blad podczas wczytywania jednomianu
    {
        partResult = parseMono(line, column);

        if (partResult == false)
            return partResult;

        if (line[*column] == '+') //kolejny jednomian
        {
            (*column)++;
        }
        else
        {
            return partResult;
        }
    }
}

bool parsePoly(const char *line, int *column);

bool parsePoly(const char *line, int *column) {
    *column = 0;
    if (parsePolyPom(line, column) && line[*column] == '\n')
        return true;
    else
        return false;
}

/**
 * Funkcja parsujaca jednomian
 * @param line : tablica charow
 * @param column : index
 * @return czy jest poprawny jednomian
 */
bool parseMono(const char *line, int *column)
{
    if (line[*column] != (char) '(')
    {
        return false;
    }

    (*column)++;

    if (!parsePolyPom(line, column))
    {
        return false;
    }

    if (line[*column] != (char) ',')
    {
        return false;
    }

    (*column)++;

    if (!parseExp(line, column))
    {
        return false;
    }

    if (line[*column] != (char) ')')
    {
        return false;
    }

    (*column)++;

    return true;
}

/**
 * Funkcja parsujaca wspolczynnik wielomian
 * @param line : tablica charow
 * @param column : index
 * @return czy sprasowano wspolczynnik wielomian
 */
bool parseCeof(const char *line, int *column)
{
    unsigned long int ceof = 0;

    int index = 0;
    int sign;
    bool end = false;


    if (line[*column + index] == '-') //wylapuje minusa
    {
        sign = -1;
        (*column)++;
    }
    else
    {
        sign = 1;
    }

    while (index < 19 && !end) //wczytuje 19 znakow
    {
        if (line[*column + index] >= '0' && line[*column + index] <= '9')
        {
            ceof = 10 * ceof + line[*column+index] - '0';
            index++;
        }
        else
        {
            end = true;
        }
    }


    if(index<19 && index>0)
    {
        *column=(*column)+index;
        return true;
    }
    else if(index==19)
    {
        if(((sign==-1) && ceof > (unsigned long)LONG_MAX+1 )|| ( (sign==1) && ceof > (unsigned long)LONG_MAX))
        {                                                                   //sprawdzan czy liczba z 19 znakami przekroczyła zakres
            *column=(*column)+index-1;
            return false;
        }
        else
        {
            *column=*column+index;
            if (line[*column] >= '0' && line[*column] <= '9') //jezeli jest jeszczez jedna liczba
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }
}

/**
 * Funkcja parsujaca wykladnik jednomianu
 * @param line : tablica charow
 * @param column : index
 * @return czy sparsowano wykladnik jednomianu
 */
bool parseExp(const char *line, int *column)
{
    long long ceof = 0;

    int index = 0;
    bool end = false;


    if (line[*column + index] == '-') //wylapuje minusa
    {
        return false;
    }
    while (index < 11 && !end)
    {
        if (line[*column + index] >= '0' && line[*column + index] <= '9')
        {
            ceof = 10 * ceof + line[*column+index] - '0';
            index++;
        }
        else
        {
            end = true;
        }
    }

    if(index<10 && index>0) //wieksze od zera bo jesli nie nie wczytano liczby
    {
        *column=(*column)+index;
        return true;
    }
    else if(index==10)
    {
        if( ceof > UINT_MAX)
        {
            *column=(*column)+index-1;
            return false;
        }
        else
        {
            *column=*column+index;
            if (line[*column] >= '0' && line[*column] <= '9')
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }
}

bool parsedegByArrgument(const char *line)
{
    int column=7;
    long long ceof = 0;

    int index = 0;
    bool end = false;


    if (line[column + index] ==  '-') //wylapuje minusa
    {
        return false;
    }
    while (index < 11 && !end)
    {
        if (line[column + index] >= '0' && line[column + index] <= '9')
        {
            ceof = 10 * ceof + line[column +index] - '0';
            index++;
        }
        else
        {
            end = true;
        }
    }

    if(line[column+index]!='\n')
    {
        return false;
    }
    else {

        if (index < 10 && index > 0)
        {
            return true;
        }
        else if (index == 10)
        {
            if (ceof > UINT_MAX) {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
}

bool parseComposeArgument(const char *line) //funkcja analogiczna do parsedegByArrgument
{                                           //TODO mozna poprawic aby była jedna
    return parsedegByArrgument(line+1);
}

bool parsedAtArrgument(const char *line)
{
    int column=3;
    bool parse=parseCeof(line,&column);
    if(parse)
    {
        if(line[column]=='\n')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

char **createArrayCommand()
{
    char **arrayOfComand = (char **) malloc(sizeof(char *) * NUMBER_OF_COMAND);

    arrayOfComand[0] = "ZERO\n";
    arrayOfComand[1] = "IS_COEFF\n";
    arrayOfComand[2] = "IS_ZERO\n";
    arrayOfComand[3] = "CLONE\n";
    arrayOfComand[4] = "ADD\n";
    arrayOfComand[5] = "MUL\n";
    arrayOfComand[6] = "NEG\n";
    arrayOfComand[7] = "SUB\n";
    arrayOfComand[8] = "IS_EQ\n";
    arrayOfComand[9] = "DEG\n";
    arrayOfComand[10] = "PRINT\n";
    arrayOfComand[11] = "POP\n";
    arrayOfComand[12] = "DEG_BY ";
    arrayOfComand[13] = "AT ";
    arrayOfComand[14] ="COMPOSE ";

    return arrayOfComand;
}

int parseCommand(char *line, char **arrayOfCommand)
{
    int i=0;

    for(i=0;i<NUMBER_OF_COMAND-2;i++)
    {
        if(strcmp(line,arrayOfCommand[i])==0) //na porownuje jak stringi bo znam konkretne dlugosci tych komend
        {
            return i;
        }
    }
    if(strncmp(line, arrayOfCommand[i], 7) == 0) //porownoje poczatkowe znaki
    {
        return i;
    }

    i++;

    if(strncmp(line,arrayOfCommand[i],3)==0) //porownuje poczatkowe znak
    {
        return i;
    }

    i++;

    if(strncmp(line,arrayOfCommand[i],8)==0) //dopisanie do compose
    {
        return i;
    }

    return -1;
}

/**
 * Pomocnicza funkcja parsujaca wielomian
 * @param line : tablica charow
 * @param column : index
 * @return czy jest poprawny wielomian
 */
bool parsePolyPom(const char *line, int *column)
{
    if (line[*column] == '(')
    {
        return parseListOfMono(line, column);
    }
    else
    {
        return parseCeof(line, column);
    }
}