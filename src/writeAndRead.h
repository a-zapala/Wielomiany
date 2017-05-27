/** @file
   Interfejs funkcji wczytujacych

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/

#include "poly.h"

#ifndef WIELOMIANY_WRITER_H
#define WIELOMIANY_WRITER_H

/**
 * Wypisuje wielomian na wyjscie standardowe od nowej lini
 * @param p : wielomian
 */
void writePolyln(const Poly *p);

/**
 * Wypisuje wielomian na wyjscie standardowe
 * @param p : wielomian
 */
void writePoly(const Poly *p);

/**
 * Drukuje linie
 * @param linia : wskaznik do wczytanej lini
 */
void printLine(const char *linia);

/**
 * Wczytuje tablice charow z wejscia standardowego i informuje czy nastapil koniec pliku
 * @param isEnd :
 * @return wskaznik do tablicy charow
 */
char *readLine(bool *isEnd);

/**
 * Zwraca wielomian wczytany z tablicy charow
 * @param line : tablica char
 * @return wielomian utworzony
 */
Poly readPoly(const char *line);



/**
 * Czyta liczbe z podanej tablicy
 * @param line : tablica char
 * @param column : index
 * @return number
 */
long readNumber(const char *line, int *column);


#endif //WIELOMIANY_WRITER_H
