/** @file
 Interfejs stosu wielomianow

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/

#ifndef WIELOMIANY_STACK_H_H
#define WIELOMIANY_STACK_H_H


#include <stdlib.h>

/**
 * Struktura przechowujaca element stosu
 */
struct Node
{
    /**Wielomian przechowywany przez dany element*/
    Poly p;

    /** Wskaznik na nastepny element na stosie */
    struct Node *next;
};
typedef struct Node Node;

/**
 * Struktura przechowujaca stos
 */

struct Stack
{
    /** Wskaznik do wierzchołka stosu*/
    Node *top;
    /** Rozmiar stosu*/
    long size;
};
typedef struct Stack Stack;

/**
 * Funkcja wkladajaca na stos dany wielomian
 * @param newPoly : wielomian
 * @param stack : stos
 */
void push(Poly newPoly,Stack *stack);

/**
 * Funkcja tworzy pusty stos
 * @param stack
 */
void init(Stack **stack);

/**
 * Funkcja zwracaja wielomian znajdujacy sie na wierzcholku stosu
 * usuwajac go z niego
 * @param stack : stos
 * @return wielomian
 */
Poly pop(Stack *stack);

/**
 * Funkcja sprawdzajaca aktualna wielkosc stosu
 * @param stack : stos
 * @return wielkosc stosu
 */
long size(Stack *stack);

/**
 * Funkcja zwraca wielomian na wierzcholku stosu
 * nie usuwajac go z niego
 * @param stack :stos
 * @return wielomian
 */
Poly top(Stack *stack);

/**
 * Funkcja czyszczaca stos
 * @param stack  :stos
 */
void clear(Stack *stack);


#endif //WIELOMIANY_STACK_H_H
