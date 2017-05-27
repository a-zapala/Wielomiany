/** @file
   Kalkulator wielomianow

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/

#include "writeAndRead.h"
#include "parser.h"
#include <stdio.h>
#include <malloc.h>
#include "stack.h"

/**
 * Funkcja wykonujaca jedno wczytana linie
 * @param line : tablica charow, wczytana linia
 * @param arrayOfComand : tablica komend
 * @param stack : stos
 * @param row : numer wiersza
 */

void makeLine(char *line, char **arrayOfComand, Stack *stack, int row)
{
    if (recognisePoly(line))
    {
        int column;
        if (parsePoly(line, &column))
        {
            Poly newPoly = readPoly(line);
            push(newPoly, stack);
        }
        else
        {
            fprintf(stderr,"ERROR %d %d\n", row, column+1);
        }
    }
    else
    {
        int numberOfCommand = parseCommand(line, arrayOfComand);

        switch (numberOfCommand)
        {

            case 0:
            {
                Poly polyZero = PolyZero();
                push(polyZero, stack);
            }
                break;


            case 1:
            {
                if (size(stack) == 0)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly poly = top(stack);
                    printf("%d\n", PolyIsCoeff(&poly));
                }
            }
                break;


            case 2:
            {
                if (size(stack) == 0)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly poly = top(stack);
                    printf("%d\n", PolyIsZero(&poly));
                }
            }
                break;


            case 3:
            {
                if (size(stack) == 0)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly poly = top(stack);
                    Poly clone = PolyClone(&poly);
                    push(clone, stack);
                }
            }
                break;


            case 4:
            {
                if (size(stack) < 2)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly p = pop(stack);
                    Poly q = pop(stack);
                    Poly poly = PolyAdd(&p, &q);
                    PolyDestroy(&p);
                    PolyDestroy(&q);
                    push(poly, stack);
                }
            }
                break;


            case 5:
            {
                if (size(stack) < 2)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly p = pop(stack);
                    Poly q = pop(stack);
                    Poly poly = PolyMul(&p, &q);
                    PolyDestroy(&p);
                    PolyDestroy(&q);
                    push(poly, stack);
                }
            }
                break;


            case 6:
            {
                if (size(stack) < 1)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly p = pop(stack);
                    Poly poly = PolyNeg(&p);
                    PolyDestroy(&p);
                    push(poly, stack);
                }
            }
            break;


            case 7:
            {
                if (size(stack) < 2)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly p = pop(stack);
                    Poly q = pop(stack);
                    Poly poly = PolySub(&p, &q);
                    PolyDestroy(&p);
                    PolyDestroy(&q);
                    push(poly, stack);
                }
            }
                break;


            case 8:
            {
                if (size(stack) < 2)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly p = pop(stack);
                    Poly q = pop(stack);
                    printf("%d\n", PolyIsEq(&p, &q));
                    push(q, stack);
                    push(p, stack);
                }
            }
                break;


            case 9:
            {
                if (size(stack) == 0)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly poly = top(stack);
                    printf("%d\n", PolyDeg(&poly));
                }
            }
                break;


            case 10:
            {
                if (size(stack) == 0)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly poly = top(stack);
                    writePolyln(&poly);
                }
            }
                break;


            case 11:
            {
                if (size(stack) == 0)
                {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                }
                else
                {
                    Poly poly = pop(stack);
                    PolyDestroy(&poly);
                }
            }
                break;


            case 12:
            {
                if (parsedegByArrgument(line))  //oprocz komendy musimy sparsowac argument
                {
                    if (size(stack) == 0)
                    {
                        fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                    }
                    else
                    {
                        int index=7;
                        unsigned int x = (unsigned int) readNumber(line, &index);
                        Poly poly = top(stack);
                        printf("%d\n", PolyDegBy(&poly, x));
                    }

                }
                else
                {
                    fprintf(stderr,"ERROR %d WRONG VARIABLE\n", row);
                }
            }
                break;


            case 13:
            {
                if (parsedAtArrgument(line)) //tutaj podobnie parsujemy argument
                {
                    if (size(stack) == 0)
                    {
                        fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", row);
                    }
                    else
                    {
                        int index=3;
                        long x = readNumber(line, &index);
                        Poly poly = pop(stack);
                        Poly newPoly = PolyAt(&poly, x);
                        PolyDestroy(&poly);
                        push(newPoly, stack);
                    }
                }
                else
                {
                    fprintf(stderr,"ERROR %d WRONG VALUE\n", row);
                }
            }
                break;


            case -1:
                fprintf(stderr,"ERROR %d WRONG COMMAND\n", row);
                break;
        }
    }
}

int main()
{
    Stack *stack;                           //inicjalizuje stos i pomocnicza tablice komend
    init(&stack);
    char **arrayOfComand = createArrayCommand();

    bool isEnd=false; //zmienna informujaca czy zostal wczytany EOF

    int row=1;
    char *line;

    while(!isEnd)
    {
        line = readLine(&isEnd);            //wczytuje dana linie
        if(!isEnd)
        {
            makeLine(line, arrayOfComand, stack, row);
            row++;
        }
        free(line);
    }

        clear(stack);               //usuwam stos wraz z pomocnicza tablica stringow
        free(arrayOfComand);

    return 0;
}


