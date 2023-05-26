#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
typedef struct cells {
    int type;
    int init_res;
    int neigh_0;
    int neigh_1;
    int neigh_2;
    int neigh_3;
    int neigh_4;
    int neigh_5;
    struct cells *next;
} cells;

int main()
{
    int i = 0;
    int number_of_cells;
    scanf("%d", &number_of_cells);

    cells mapcells;
        for (int i = 0; i < number_of_cells; i++) {
        // 0 for empty, 1 for eggs, 2 for crystal
        int type;
        // the initial amount of eggs/crystals on this cell
        int initial_resources;
        // the index of the neighbouring cell for each direction
        int neigh_0;
        int neigh_1;
        int neigh_2;
        int neigh_3;
        int neigh_4;
        int neigh_5;
        scanf("%d%d%d%d%d%d%d%d", &type, &initial_resources, &neigh_0, &neigh_1, &neigh_2, &neigh_3, &neigh_4, &neigh_5);
        if (type == 2)
        {
            head->resources = initial_resources;
            head->idx = i;
            head->next = calloc(sizeof(geminfo), 1);
            head = head->next;
        }
    }

    int totalbase;
    scanf("%d", &totalbase); // total amount of bases

    int mybases[totalbase];
    int oppbases[totalbase];


    for (i = 0; i < totalbase; i++) {
        scanf("%d", &mybases[i]);
    }
    for (i = 0; i < totalbase; i++) {
        scanf("%d", &oppbases[i]);
    }

    int resources;
    int my_ants;
    int opp_ants;
    // game loop
    while (1) {
        for (int i = 0; i < number_of_cells; i++) {
            scanf("%d%d%d", &resources, &my_ants, &opp_ants);
        }

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");

        fprintf(stderr, "type: %d\n", mapcells.type);
        // WAIT | LINE <sourceIdx> <targetIdx> <strength> | BEACON <cellIdx> <strength> | MESSAGE <text>
        printf("WAIT\n");
    }

    return 0;
}
