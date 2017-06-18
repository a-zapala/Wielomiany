/** @file
   Implementacja klasy wielomianów

   @author Jakub Pawlewicz <pan@mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-03-04
*/

#include "poly.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/** Maksymalna wartość wykładnika wielomianu */
#define POLY_EXP_MAX INT_MAX

/**
 * Zwraca liczbę jednomianów w wielomianie normalnym.
 * @param[in] p : wielomian normalny
 * @return liczba jednomianów
 */
static inline unsigned PolyLen(const Poly *p)
{
    return p->size;
}

/**
 * Ustala rozmiar i przydziela pamięć na @p size jednomianów.
 * @param[in] size : liczba jednomianów
 * @return zaalokowany wielomian
 */
static Poly PolyAlloc(unsigned size)
{
    Mono *arr = calloc(size, sizeof(Mono));
    return (Poly) {.size = size, .arr = arr};
}

/**
 * Optymalizuje rozmiar przydzielonej pamięci wielomianowi.
 * @param[in,out] p : optymalizowany wielomian
 */
static void PolyRealloc(Poly *p)
{
    if (PolyLen(p) == 0)
    {
        PolyDestroy(p);
        p->coeff = 0;
    }
    else if (PolyLen(p) == 1 && p->arr[0].exp == 0 && PolyIsCoeff(&p->arr[0].p))
    {
        p->coeff = p->arr[0].p.coeff;
        free(p->arr);
        p->arr = NULL;
    }
    else if (PolyLen(p) == 1 && PolyIsZero(&(p->arr[0].p))) //wprowadzona zmiana by Andrzej Zapała
    {
        PolyDestroy(p);
        p->arr = NULL;
        p->coeff = 0;
    }
    else
    {
        p->arr = realloc(p->arr, PolyLen(p) * sizeof(Mono));
    }
}

void PolyDestroy(Poly *p)
{
    if (PolyIsCoeff(p))
    {
        return;
    }
    for (unsigned i = 0; i < PolyLen(p); ++i)
        MonoDestroy(&p->arr[i]);
    free(p->arr);
    p->arr = NULL;
}

Poly PolyClone(const Poly *p)
{
    if (PolyIsCoeff(p))
    {
        return *p;
    }
    Poly r = PolyAlloc(p->size);
    for (unsigned i = 0; i < PolyLen(p); ++i)
        r.arr[i] = MonoClone(&p->arr[i]);
    return r;
}

static Poly PolyAddCoeff(const Poly *p, poly_coeff_t c);

/**
 * Dodaje wielomian normalny i współczynnik.
 * @param[in] p : wielomian normalny
 * @param[in] c : współczynnik
 * @return
 */
static Poly PolyAddPolyCoeff(const Poly *p, poly_coeff_t c)
{
    if (c == 0)
    {
        return PolyClone(p);
    }
    else if (p->arr[0].exp == 0)
    {
        Poly r0 = PolyAddCoeff(&p->arr[0].p, c);
        if (PolyIsZero(&r0))
        {
            Poly r = PolyAlloc(PolyLen(p) - 1);
            for (unsigned i = 0; i < PolyLen(&r); ++i)
                r.arr[i] = MonoClone(&p->arr[i + 1]);
            return r;
        }
        else if (PolyIsCoeff(&r0) && PolyLen(p) == 1)
        {
            return r0;
        }
        else
        {
            Poly r = PolyAlloc(PolyLen(p));
            r.arr[0] = MonoFromPoly(&r0, 0);
            for (unsigned i = 1; i < PolyLen(p); ++i)
                r.arr[i] = MonoClone(&p->arr[i]);
            return r;
        }
    }
    else
    {
        Poly r = PolyAlloc(PolyLen(p) + 1);
        r.arr[0] = (Mono) {.p = PolyFromCoeff(c), .exp = 0};
        for (unsigned i = 0; i < PolyLen(p); ++i)
            r.arr[i + 1] = MonoClone(&p->arr[i]);
        return r;
    }
}

/**
 * Dodaje do wielomianu współczynnik.
 * @param[in] p : wielomian
 * @param[in] c : współczynnik
 * @return `p + c`
 */
static Poly PolyAddCoeff(const Poly *p, poly_coeff_t c)
{
    if (PolyIsCoeff(p))
    {
        return PolyFromCoeff(p->coeff + c);
    }
    else
    {
        return PolyAddPolyCoeff(p, c);
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
    Poly r = PolyAlloc(PolyLen(p) + PolyLen(q));
    unsigned i = 0, j = 0, k = 0;
    while (i < PolyLen(p) || j < PolyLen(q))
    {
        poly_exp_t pe = i < PolyLen(p) ? p->arr[i].exp : POLY_EXP_MAX;
        poly_exp_t qe = j < PolyLen(q) ? q->arr[j].exp : POLY_EXP_MAX;
        if (pe < qe)
        {
            r.arr[k++] = MonoClone(&p->arr[i++]);
        }
        else if (pe > qe)
        {
            r.arr[k++] = MonoClone(&q->arr[j++]);
        }
        else
        {
            Mono m = (Mono) {.p = PolyAdd(&p->arr[i++].p, &q->arr[j++].p),
                    .exp = pe};
            if (!PolyIsZero(&m.p))
            {
                r.arr[k++] = m;
            }
        }
    }
    r.size = k;
    PolyRealloc(&r);
    return r;
}

Poly PolyAdd(const Poly *p, const Poly *q)
{
    if (PolyIsCoeff(p))
    {
        return PolyAddCoeff(q, p->coeff);
    }
    else if (PolyIsCoeff(q))
    {
        return PolyAddCoeff(p, q->coeff);
    }
    else
    {
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
    Poly r = (Poly) {.size = k, .arr = arr};
    PolyRealloc(&r);
    return r;
}

/**
 * Mnoży dwa wielomiany normalne.
 * @param[in] p : wielomian normalny
 * @param[in] q : wielomian normalny
 * @return `p * q`
 */
static Poly PolyMulPolyPoly(const Poly *p, const Poly *q)
{
    unsigned count = PolyLen(p) * PolyLen(q);
    Mono *arr = calloc(count, sizeof(struct Mono));
    unsigned k = 0;
    for (unsigned i = 0; i < PolyLen(p); ++i)
        for (unsigned j = 0; j < PolyLen(q); ++j)
            arr[k++] = (Mono) {.p = PolyMul(&p->arr[i].p, &q->arr[j].p),
                    .exp = p->arr[i].exp + q->arr[j].exp};
    Poly res = PolyAddMonos(count, arr);
    free(arr);
    return res;
}

static Poly PolyMulCoeff(const Poly *p, poly_coeff_t c);

/**
 * Mnoży wielomian normalny przez współczynnik.
 * @param[in] p : wielomian normalny
 * @param[in] c : współczynnik
 * @return `p * c`
 */
static Poly PolyMulPolyCoeff(const Poly *p, poly_coeff_t c)
{
    if (c == 0)
    {
        return PolyZero();
    }
    Poly r = PolyAlloc(PolyLen(p));
    r.size = 0;
    for (unsigned i = 0; i < PolyLen(p); ++i)
    {
        Mono m = (Mono) {.p = PolyMulCoeff(&p->arr[i].p, c),
                .exp = p->arr[i].exp};
        if (!PolyIsZero(&m.p))
        {
            r.arr[r.size++] = m;
        }
    }
    PolyRealloc(&r);
    return r;
}

/**
 * Mnoży wielomian przez współczynnik.
 * @param[in] p : wielomian
 * @param[in] c : współczynnik
 * @return `p * c`
 */
static Poly PolyMulCoeff(const Poly *p, poly_coeff_t c)
{
    if (PolyIsCoeff(p))
    {
        return PolyFromCoeff(p->coeff * c);
    }
    else
    {
        return PolyMulPolyCoeff(p, c);
    }
}

Poly PolyMul(const Poly *p, const Poly *q)
{
    if (PolyIsCoeff(p))
    {
        return PolyMulCoeff(q, p->coeff);
    }
    else if (PolyIsCoeff(q))
    {
        return PolyMulCoeff(p, q->coeff);
    }
    else
    {
        return PolyMulPolyPoly(p, q);
    }
}

Poly PolyNeg(const Poly *p)
{
    return PolyMulCoeff(p, -1);
}

Poly PolySub(const Poly *p, const Poly *q)
{
    Poly neg_q = PolyNeg(q);
    Poly res = PolyAdd(p, &neg_q);
    PolyDestroy(&neg_q);
    return res;
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx)
{
    if (PolyIsCoeff(p))
    {
        return PolyIsZero(p) ? -1 : 0;
    }
    else if (var_idx == 0)
    {
        return p->arr[PolyLen(p) - 1].exp;
    }
    else
    {
        poly_exp_t deg = -1;
        for (unsigned i = 0; i < PolyLen(p); ++i)
        {
            poly_exp_t d = PolyDegBy(&p->arr[i].p, var_idx - 1);
            if (d > deg)
            {
                deg = d;
            }
        }
        return deg;
    }
}

poly_exp_t PolyDeg(const Poly *p)
{
    if (PolyIsCoeff(p))
    {
        return PolyIsZero(p) ? -1 : 0;
    }
    else
    {
        poly_exp_t deg = -1;
        for (unsigned i = 0; i < PolyLen(p); ++i)
        {
            poly_exp_t d = PolyDeg(&p->arr[i].p) + p->arr[i].exp;
            if (d > deg)
            {
                deg = d;
            }
        }
        return deg;
    }
}

bool PolyIsEq(const Poly *p, const Poly *q)
{
    if (PolyIsCoeff(p) != PolyIsCoeff(q))
    {
        return false;
    }
    else if (PolyIsCoeff(p))
    {
        return p->coeff == q->coeff;
    }
    else if (PolyLen(p) != PolyLen(q))
    {
        return false;
    }
    else
    {
        for (unsigned i = 0; i < PolyLen(p); ++i)
            if (p->arr[i].exp != q->arr[i].exp ||
                !PolyIsEq(&p->arr[i].p, &q->arr[i].p))
            {
                return false;
            }
        return true;
    }
}

/**
 * Wylicza @f$ab^n@f$.
 * @param[in] a - mnożnik
 * @param[in] b - podstawa
 * @param[in] n - wykładnik
 * @return @f$ab^n@f$
 */
static poly_coeff_t PowHelp(poly_coeff_t a, poly_coeff_t b, poly_exp_t n)
{
    if (n == 0)
    {
        return a;
    }
    else
    {
        return PowHelp(n % 2 == 0 ? a : a * b, b * b, n / 2);
    }
}

/**
 * Wylicza @f$a^n@f$.
 * @param[in] a - podstawa
 * @param[in] n - wykładnik
 * @return @f$a^n@f$
 */
static inline poly_coeff_t Pow(poly_coeff_t a, poly_exp_t n)
{
    return PowHelp(1, a, n);
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
    if (PolyIsCoeff(p))
    {
        return PolyClone(p);
    }
    Poly r = PolyZero();
    poly_exp_t n = 0;
    poly_coeff_t x_to_n = 1;
    for (unsigned i = 0; i < PolyLen(p); ++i)
    {
        x_to_n *= Pow(x, p->arr[i].exp - n);
        n = p->arr[i].exp;
        Poly pi = PolyMulCoeff(&p->arr[i].p, x_to_n);
        Poly q = PolyAdd(&r, &pi);
        PolyDestroy(&pi);
        PolyDestroy(&r);
        r = q;
    }
    return r;
}


Poly PolyPower1(const Poly *p, poly_exp_t power) //wykonujemy mnozenie iterowane
{
    Poly currentPoly;
    Poly prevPoly = PolyFromCoeff(1);

    for (unsigned i = 0; i < (unsigned) power; i++)
    {
        currentPoly = PolyMul(&prevPoly, p);
        PolyDestroy(&prevPoly);
        prevPoly = currentPoly;
    }

    return prevPoly;
}

/**
 * Funkcja zwracajaca wielomian do danej potegi naturalnej nie modyfikujaca podstawy
 * @param p wielomian podnoszacy do potegi
 * @param power wykladnik potegi
 * @return
 */

Poly PolyPower(const Poly *p, poly_exp_t power) //niby szybciej ale wolniej
{
    if (!power)
    {
        return PolyFromCoeff(1);
    }

    if (power % 2 == 1)
    {
        Poly pom1 = PolyPower(p, (power - 1) / 2);
        Poly pom2 = PolyClone(&pom1);
        Poly pom3 = PolyMul(&pom1, &pom2);
        Poly pom4 = PolyMul(p, &pom3);

        PolyDestroy(&pom1);
        PolyDestroy(&pom2);
        PolyDestroy(&pom3);

        return pom4;
    }
    else
    {
        Poly pom1 = PolyPower(p, power / 2);
        Poly pom2 = PolyClone(&pom1);
        Poly pom3 = PolyMul(&pom1, &pom2);

        PolyDestroy(&pom1);
        PolyDestroy(&pom2);

        return pom3;
    }
}


Poly PolyCompose(const Poly *p, unsigned count, const Poly x[])
{
    if (PolyIsCoeff(p))
    {
        return PolyClone(p);
    }
    else if (count == 0)
    {
        if (PolyIsCoeff(&(p->arr->p))) //kiedy w liscie jest wielomian stały
        {
            return PolyClone(&(p->arr->p));
        }
        else
        {
            return PolyZero();
        }
    }
    else
    {
        Poly currentPoly = x[0]; //pierwszy wielomian w liscie bedzie aktualnym, robiac rekursje zmniejszamy liste i zmianiamy wskaznik tablicy
        Poly resultPoly = PolyZero(); //bedziemy dodawac do tego wielomianu wszystkie wielomiany stworzone z jednomianow z listy

        for (unsigned i = 0; i < p->size; i++) //iteruje po jednomianach w liscie
        {
            Poly monoCompose = PolyCompose(&(((p->arr) + i)->p), count - 1, x +
                                                                            1); //przesuwam indeks tablicy zmniejszajac count, podaje do PolyCompose wielomian
            //odpowiedniego jednomianu
            Poly powCurrentPoly = PolyPower(&currentPoly, ((p->arr) + i)->exp); //podnoszę do potegi

            Poly polyComposeFromMono = PolyMul(&powCurrentPoly, &monoCompose);


            Poly pom = PolyAdd(&resultPoly,
                               &polyComposeFromMono); //dodaje do wynikowego wielomianu tego ktory został policzony w danej iteracji

            PolyDestroy(&polyComposeFromMono);
            PolyDestroy(&resultPoly);
            PolyDestroy(&powCurrentPoly);
            PolyDestroy(&monoCompose);

            resultPoly = pom;
        }

        return resultPoly;
    }
}

