/** @file
   Implementacja funkcji wczytujcyh

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/
#include <stdio.h>
#include "poly.h"
#include "writeAndRead.h"
#include <stdlib.h>

void writeMono(const Mono *m);

void writePolyln(const Poly *p)
{
    writePoly(p);
    printf("\n");
}

void writePoly(const Poly *p)
{
    if (PolyIsCoeff(p))
    {
        printf("%ld", (poly_coeff_t) (p->coeff));
    } else
    {
        for (unsigned i = 0; i < (p->size - 1); i++)
        {
            writeMono(&(p->arr[i]));
            printf("+");
        }
        writeMono(&(p->arr[p->size - 1]));
    }
}
/**
 * Wypisuje jednomian na wyjscie standardowe
 * @param m : jednomian
 */
void writeMono(const Mono *m)
{
        printf("(");
        writePoly(&(m->p));
        printf(",");
        printf("%d", (int) (m->exp));
        printf(")");
}

void printLine(const char *linia)
{
    int i = 0;

    char c;
    do
    {
        c = linia[i];
        printf("%c", c);
        i++;

    } while (c != '\n');
}

char *readLine(bool *isEnd)
{
    unsigned size = 1;

    char *linia = (char *) malloc(size * sizeof(char));
    char c = (char)getchar();
    unsigned index = 0;

    if(c==EOF || c=='$')
    {
        *isEnd=true;
        return linia;
    }
    else {

        while (c != (char) '\n') {

            linia[index] = c;
            index++;
            if (index == size) {
                size = size * 2;
                linia = realloc(linia, size * sizeof(char
                                              ));
            }
            c = getchar();

        }
        linia[index] = c;
        linia = realloc(linia, (index + 2) * sizeof(char));
        linia[index + 1] = '\0';
        return linia;
    }
}

Mono readMono(const char *line, int *column);

Poly readListOfMono(const char *line, int *column);

/**
 * Pomomcnicza funkcja do czytania wielomianu
 * @param line : tablica charow
 * @param column : index
 * @return wielomian utworzony
 */
Poly readPolyPom(const char *line, int *column)
{
    if (line[*column] == '(')
    {
        return readListOfMono(line,column);
    } else
    {
        return PolyFromCoeff(readNumber(line, column));
    }
}

Poly readPoly(const char *line)
{
    int column = 0;
    return readPolyPom(line, &column);
}

/**
 * Wczytuje jednomiany z tablicy charow i tworzy z nich wielomian
 * za pomoca funkcji AddMonos
 * @param line : tablica char
 * @param column : index
 * @return Wielomian utworzony z jednomianow
 */
Poly readListOfMono(const char *line, int *column)
{
    unsigned size = 1, index = 0;
    Mono *listOfMono = (Mono *) malloc(sizeof(Mono) * size);
    bool end=false;

    while (!end)
    {

        Mono newMono=readMono(line, column);
        listOfMono[index] =newMono;
        index++;

        if (index == size)
        {
            size = size * 2;
            listOfMono = realloc(listOfMono,size* sizeof(Mono));
        }

        if (line[*column] == '+')
        {
            (*column)++;
        }
        else
        {
            end=true;
            size=index;
        }

    }
    Poly newPoly=PolyAddMonos(size,listOfMono);
    free(listOfMono);
    return newPoly;
}

long readNumber(const char *line, int *column)
{
    char *end;
    long number = strtoll(line + *column, &end, 10);

    while (line + *column != end)
        (*column)++;

    return number;
}

/**
 * Zwraca jednomian z tablicy charow
 * @param line : tablica char
 * @param column : index
 * @return jednomian
 */
Mono readMono(const char *line, int *column)
{
    (*column)++;
    Poly pomPoly = readPolyPom(line, column);
    (*column)++;
    int exp = (int)readNumber(line, column);
    (*column)++;

    return MonoFromPoly(&pomPoly, exp);
}



