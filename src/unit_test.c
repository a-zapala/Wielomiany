//
// Created by andrzej on 16.06.17.
//

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include "writeAndRead.h"


#include <stdlib.h>

#include "cmocka.h"

#include "poly.h"

static void test_1(void **state)
{
    (void) state;

    Poly p = PolyZero();

    Poly* x=(Poly*)malloc(sizeof(Poly)); //deklaruje tablice wielomianów
    x[0]=PolyClone(&p); //zapełniam tablice

    unsigned count = 0;

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsZero(&result));

    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    free(x);
}

static void test_2(void **state)
{
    (void) state;

    Poly p = PolyZero();

    Poly* x=(Poly*)malloc(sizeof(Poly));
    x[0]=PolyFromCoeff(100);

    unsigned count = 1;

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsZero(&result));

    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    free(x);
}

static void test_3(void **state)
{
    (void) state;

    Poly p = PolyFromCoeff(100);

    Poly* x=(Poly*)malloc(sizeof(Poly));
    x[0]=PolyFromCoeff(200);

    unsigned count = 0;

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&result,&p));

    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    free(x);
}

static void test_4(void **state)
{
    (void) state;

    Poly p = PolyFromCoeff(100);

    Poly* x=(Poly*)malloc(sizeof(Poly));
    x[0]=PolyFromCoeff(200);

    unsigned count = 1;

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&result,&p));

    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    free(x);
}

static void test_5(void **state)
{
    (void) state;

    Poly q = PolyFromCoeff(1);

    unsigned maxExponent=10;
    Mono* monos=(Mono*)malloc(sizeof(Mono)*maxExponent);

    for(unsigned i=0;i<maxExponent;i++) //tworze wielomian postaci 1+x+x^2+...+x^9
    {
        monos[i]=MonoFromPoly(&q,i);
    }

    Poly p=PolyAddMonos(maxExponent,monos);

    unsigned count = 0;

    Poly* x=(Poly*)malloc(sizeof(Poly));
    x[0]=PolyFromCoeff(200);

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&result,&q)); //porownuje go do wielomianu stałego rownego 1, wszystkie jednomiany z x zostały wyzerowane

    PolyDestroy(&q);
    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    free(x);
    free(monos);
}

static void test_6(void **state)
{
    (void) state;

    Poly q = PolyFromCoeff(1);

    unsigned maxExponent=10;
    Mono* monos=(Mono*)malloc(sizeof(Mono)*maxExponent); //tworze taki sam wielomian jak w test_6

    for(unsigned i=0;i<maxExponent;i++)
    {
        monos[i]=MonoFromPoly(&q,i);
    }

    Poly p=PolyAddMonos(maxExponent,monos);

    Poly* x=(Poly*)malloc(sizeof(Poly));
    x[0]=PolyFromCoeff(10);

    Poly properResult=PolyFromCoeff(1111111111); //kazdy z jedomianow da skladnik postaci 10 do potegi takiej jaki jest wykladnik jednomianu,
                                                //jesli wielomian p ma wszystkie wykladniki od 0 do 9 to liczba bedzie 10 cyfrowa
                                                //oraz skladała sie z samych jedynek
    unsigned count = 1;

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&result,&properResult));

    PolyDestroy(&q);
    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    PolyDestroy(&properResult);
    free(x);
    free(monos);
}

static void test_7(void **state)
{
    (void) state;

    Poly q = PolyFromCoeff(1);

    unsigned maxExponent=10;
    Mono* monos=(Mono*)malloc(sizeof(Mono)*maxExponent);

    for(unsigned i=0;i<maxExponent;i++)
    {
        monos[i]=MonoFromPoly(&q,i);
    }

    Poly p=PolyAddMonos(maxExponent,monos);

    Poly* x=(Poly*)malloc(sizeof(Poly));
    Mono mono=MonoFromPoly(&q,2); //wielomian wstawiany bedzie postaci x^2
    x[0]=PolyAddMonos(1,&mono);

    Mono* monos2=(Mono*)malloc(sizeof(Mono)*maxExponent);
    for(unsigned i=0;i<maxExponent;i++)
    {
        monos2[i]=MonoFromPoly(&q,2*i); //budujemy odpowiedni wielomian jezeli do wielomianu postaci 1+x+x^2...
                                        //wstawimy za x odpowiednio x^2 to uzyskamy 1+x^2+x^4+...
                                        //czyli wykladniki w odpowiednich jednomianach będą dwa razy większe
    }

    unsigned count = 1;

    Poly properResult=PolyAddMonos(maxExponent,monos2);
    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&result,&properResult));

    PolyDestroy(&q);
    PolyDestroy(&p);
    PolyDestroy(&(x[0]));
    PolyDestroy(&result);
    PolyDestroy(&properResult);
    free(x);
    free(monos);
    free(monos2);
}



int main() {
    /* Definiujemy listę testów jednostkowych do wykonania. Każdy element
    tablicy jest wynikiem wykonania makra cmocka_unit_test,
    cmocka_unit_test_setup, cmocka_unit_test_setup_teardown,
    cmocka_unit_test_teardown lub tp. */
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_1),
            cmocka_unit_test(test_2),
            cmocka_unit_test(test_3),
            cmocka_unit_test(test_4),
            cmocka_unit_test(test_5),
            cmocka_unit_test(test_6),
            cmocka_unit_test(test_7),

    };

    /* Wykonujemy grupę testów wyspecyfikowanych w tablicy tests, drugi i trzeci
    parametr to wskaźniki odpowiednio na funkcję, która ma być wykonana przed
    wszystkimi testami, i funkcję, która ma być wykonana po wszystkich
    testach. */
    return cmocka_run_group_tests(tests, NULL, NULL);
}
