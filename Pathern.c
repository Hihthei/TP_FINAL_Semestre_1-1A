#include <stdio.h>
#include <math.h>

struct Position
{
    float x;
    float y;

    float vie;
};
typedef struct Position Position;

void pathern1 (Position* enemy)
{
    //const int x = enemy->x;
    const int y = enemy->y;
    float i = 0.7;
    int compteur = 0;

    while ( enemy->vie > 0)
    {
        i = 0.7;   
        while (i < 2.3)
        {
            //enemy->x = x * (sinf(i));
            enemy->y = y * (sin(i));
            i += 0.1;

            printf("[%f] [%f]\n -- vie enemie : %f --\n", enemy->x, enemy->y, enemy->vie);

            ( enemy->vie ) -= 1;
            if ((enemy->vie) <= 0) {
                break;
            }
            
            compteur++;
        }
        printf("\n-----SALVE_SUIVANTE-----\n");
    }

    printf("Il y a eu %d patterne.e.s.\n", compteur);
}

int main()
{
    Position enemy = { 1500, 700, 150 };


    pathern1( &enemy );
}