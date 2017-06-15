/** @file
   Implementacja stosu wielomianow

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/
#include "poly.h"
#include "stack.h"
#include <stdlib.h>


void push(Poly newPoly,Stack *stack)
{
    Node *newNode=(Node*)malloc(sizeof(Node));
    newNode->p=newPoly;
    newNode->next=stack->top;
    stack->top=newNode;
    stack->size=stack->size+1;
}

void init(Stack **stack)
{
    *stack=(Stack*)malloc(sizeof(Stack));
    (*stack)->top=NULL;
    (*stack)->size=0;
}

Poly pop(Stack *stack)
{
    Poly newPoly=stack->top->p;
    Node *pom=stack->top;
    stack->top=stack->top->next;
    stack->size=stack->size-1;
    free(pom);
    return newPoly;
}

long size(Stack *stack)
{
    return stack->size;
}

Poly top(Stack *stack)
{
    return stack->top->p;
}

void clear(Stack *stack)
{
    while (size(stack)>0)
    {
        Poly deletePoly=pop(stack);
        PolyDestroy(&deletePoly);
    }
    free(stack);
}

