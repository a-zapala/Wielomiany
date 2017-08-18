/** @file
   Implementacja klasy wielomianów

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-08-17
*/

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <memory.h>
#include "poly.h"


/** Maksymalna wartość wykładnika wielomianu */

#define POLY_EXP_MAX INT_MAX;


void ListDestroy(List *monoList) {
    List *currentElement = monoList;

    while (currentElement->next) {
        List *prevElement = currentElement;
        currentElement = currentElement->next;
        MonoDestroy(&(prevElement->m));
        free(prevElement);

    }
    *monoList=NULL;
}

void PolyDestroy(Poly *p) {
    if (PolyIsCoeff(p)) {
        return;
    }

    ListDestroy(p->monoList);
    p->monoList = NULL;
}

List *ListClone(const List *l) {
    assert(!l);
    List *newList = (List *) malloc(sizeof(List));
    newList->m = MonoClone(&(l->m));
    newList->next = NULL;
    if (l->next) {
        newList->next = ListClone(l->next);
    }
    return newList;
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return *p;
    }
    Poly r;
    r.monoList = ListClone(p->monoList);

    return r;
}

static Poly PolyAddPolyCoeff(const Poly *p, poly_coeff_t c) {
    if (c == 0) {

        return PolyClone(p);

    } else if (p->monoList->m.exp == 0) {

        Poly r = PolyAddCoeff(&(p->monoList->m.p), c);

        if (PolyIsZero(&r)) {
            Poly newPoly;
            newPoly.monoList = ListClone(p->monoList->next);

            return newPoly;
        } else if (PolyIsCoeff(&r) && p->monoList->next == NULL) //jest tylko jeden element w liscie
        {
            return r;
        } else {
            Poly newPoly;

            newPoly.monoList = ListClone(p->monoList);
            newPoly.monoList = putAtBeg(newPoly.monoList, MonoFromPoly(&r, 0));

            return newPoly;
        }
    } else
    {
        Poly newPoly;
        newPoly.monoList=ListClone(p->monoList);
        newPoly.monoList=putAtBeg(newPoly.monoList,(Mono) {.p=PolyFromCoeff(c), .exp=0});

        return newPoly;
    }
}

static Poly PolyAddCoeff(const Poly *p, poly_coeff_t c) {
    return PolyIsCoeff(p) ? PolyFromCoeff(p->coeff + c) : PolyAddPolyCoeff(p, c);
}

/**
 * Dodaje do wielomianu różnym od stałego współczynnik.
 * @param[in] p : wielomian
 * @param[in] c : współczynnik
 * @return `p + c`
 */

static List *putAtBeg(List *l, Mono m) {
    List *newList = (List *) malloc(sizeof(List));
    newList->next = l;
    newList->m = m;

    return newList;

}

static List *putAtEnd(List *l,Mono m) //zwraca element dodany na koncu
{
    List *newList=(List*) malloc(sizeof(List));
    newList->m=m;

    if(l)
    {
        return newList;
    } else{
        assert(l->next==NULL);
        l->next=newList;
        return newList;
    }
}

/**
 * Optymalizuje rozmiar przydzielonej pamięci wielomianowi w przypadku listy pozbywa sie niepoprawnych wielomianow
 * @param[in,out] p : optymalizowany wielomian
 */
static void PolyRealloc(Poly *p)
{
    if (p->monoList->next==NULL && p->monoList->m.exp == 0 && PolyIsCoeff(&p->monoList->m.p))
    {
        p->coeff = p->monoList->m.p.coeff;
        ListDestroy(p->monoList);
    }
    else if (p->monoList->next==NULL && PolyIsZero(&(p->monoList->m.p))) //wprowadzona zmiana by Andrzej Zapała
    {
        PolyDestroy(p);
        assert(p->monoList==NULL);
        p->coeff = 0;
    }
}

/**
 * Dodaje dwa normalne wielomiany.
 * @param[in] p : wielomian normalny
 * @param[in] q : wielomian normalny
 * @return
 */
static Poly PolyAddPolyPoly(const Poly *p, const Poly *q)
{
    Poly newPoly;

    List *pList=p->monoList;
    List *qList=q->monoList;

    while(qList || pList)
    {
        poly_exp_t pe = !pList ? pList->m.exp : POLY_EXP_MAX;
        poly_exp_t qe =  !qList ? qList->m.exp : POLY_EXP_MAX;

        if(pe < qe)
        {
            newPoly.monoList=putAtEnd(newPoly.monoList,MonoClone(&pList->m));
            pList=pList->next;
        }
        else if(pe>qe){
            newPoly.monoList=putAtEnd(newPoly.monoList,MonoClone(&qList->m));
            qList=qList->next;
        }
        else{
            Mono m = (Mono) {.p =PolyAdd(&pList->m.p,&qList->m.p), .exp = pe};

            if(!PolyIsZero(&m.p))
            {
                newPoly.monoList=putAtEnd(newPoly.monoList,m);
                pList=pList->next;
                qList=qList->next;
            }
        }

    }

    PolyRealloc(&newPoly);

    return newPoly;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p)) {
        return PolyAddCoeff(q, p->coeff);
    } else if (PolyIsCoeff(q)) {
        return PolyAddCoeff(p, q->coeff);
    } else {
        return PolyAddPolyPoly(p, q);
    }
}

/**
 * Pomocnicza funkcja do sortowania jednomianów.
 * @param a : wskaźnik do jednomianu
 * @param b : wskaźnik do jednomianu
 * @return wynik porównania wykładników jednomianów
 */
static poly_exp_t MonoCmp(const void *a, const void *b)
{
    return ((const Mono *) a)->exp - ((const Mono *) b)->exp;
}

Poly PolyAddMonos(unsigned count, const Mono monos[])
{
    Mono *arr = calloc(count, sizeof(Mono));
    memcpy(arr, monos, count * sizeof(Mono));
    qsort(arr, count, sizeof(Mono), MonoCmp);

    unsigned k = 0;
    for (unsigned i = 0; i < count; ++i)
    {
        if (k == 0 || arr[i].exp != arr[k - 1].exp)
        {
            arr[k++] = arr[i];
        }
        else
        {
            Mono m = {PolyAdd(&arr[k - 1].p, &arr[i].p), arr[i].exp};
            PolyDestroy(&arr[k - 1].p);
            PolyDestroy(&arr[i].p);
            if (PolyIsZero(&m.p))
            {
                k--;
            }
            else
            {
                arr[k - 1] = m;
            }
        }
    }

    Poly newPoly;

    for(unsigned i=0;i<k;i++)
    {
        newPoly.monoList=putAtEnd(newPoly.monoList,arr[i]);
    }

    PolyRealloc(&newPoly);

    return newPoly;
}