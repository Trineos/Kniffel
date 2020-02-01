#include "kniffel.h"
#include <stdlib.h>
#include <stdio.h>
#include "communication.h"

#define TRUE 1
#define FALSE 0

void wuerfel (int* w, int* b, int try)
{
    if (try < 3 && (b[0] || b[1] || b[2] || b[3] || b[4]))
    {
        for (int i = 0; i < 5; i++)
        {
            if (b[i])
            {
                w[i] = 1 + rand() % 6;
            }
        }
        try++;
    }
    return;
}


void printwuerfel (int* wuerfel, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf ("%d ", wuerfel [i]);
    }
}