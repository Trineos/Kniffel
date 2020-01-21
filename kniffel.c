#include "kniffel.h"
#include <stdlib.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

void wuerfel (int* w, int* b, int try)
{
    while (try < 3 && (b[0] || b[1] || b[2] || b[3] || b[4]))
    {
        for (int i = 0; i < 5; i++)
        {
            if (b[i])
            {
                w[i] = 1 + rand() % 6;
            }
        }
        printwuerfel (w,5);
        printf ("Welche neu? ");
        scanf("%d %d %d %d %d", &b[0], &b[1], &b[2], &b[3], &b[4]);
        printf ("\n");

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