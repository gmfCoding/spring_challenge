#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

// UTILITY FUNCTIONS
char *strjoin(const char* s1, const char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
    }
    return result;
}

char *strjoinmany(int amount, ...)
{
	char *str;
	char *full = strdup("");
	char *tmp;
	va_list va;

	va_start(va, amount);
	while (amount > 0)
	{
		str = va_arg(va, char*);
		tmp = full;
		full = strjoin(full, str);
		free(tmp);
		free(str);
		amount--;
	}
    return full;
}

// ACTION INFORMATION AND PROCESSING
typedef struct action action;

// using extern allows us to forward declare g_actions, 
// otherwise it cannot be found, because it is really defined down near g_cells.
extern action *g_actions;

typedef struct action
{
    char *action;
    action *next;
    action *last;
} action;

action *create_action()
{
    if (g_actions == NULL)
    {
        g_actions = calloc(sizeof(action), 1);
        g_actions->last = g_actions;
        return (g_actions);
    }
    g_actions->last->next = calloc(sizeof(action), 1);
    g_actions->last = g_actions->last->next;
    return (g_actions->last);
}

void action_line(int src, int dst, int strn)
{
    char buffer[200];
    sprintf(buffer, "LINE %d %d %d", src, dst, strn);
    create_action()->action = strdup(buffer);
}

void action_beacon(int dst, int strn)
{    
    char buffer[200];
    sprintf(buffer, "BEACON %d %d", dst, strn);
    create_action()->action = strdup(buffer);
}

void action_message(const char *message)
{    
    char buffer[200];
    sprintf(buffer, "MESSAGE %s", message);
    create_action()->action = strdup(buffer);
}

void do_actions()
{
    char *actions = g_actions->action;
    action *curr = g_actions->next;
	action *tmp;
    int amount = 0;

    while (curr)
    {
        amount++;
		actions = strjoinmany(3, actions, strdup(";"), curr->action);
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    g_actions = NULL;
    if (amount == 0)
    {
        printf("WAIT\n");
        return;
    }
    printf("%s\n", actions);
    free(actions);
}

void assert(const char *err, bool condition)
{
    if (condition)
		error(err);
}

void error(const char *err)
{
    fprintf(stderr, "%s\n", err);
}
// CELL INFORMATION AND PROCESSING
typedef struct cellinfo cellinfo;

cellinfo *g_cell;
action *g_actions = NULL;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
typedef struct cellinfo
{
    int idx;

    int resources;
    int eggs;
    int my_ants;
    int op_ants;
    int type;                   //0 for blank, 1 for Eggs, 2 for Crystals
    int initial_resources;      
    int nmr_0;                  //right
    int ntr_1;                  //top right
    int ntl_2;                  //top left
    int nml_3;                  //left
    int nbl_4;                  //bot left
    int nbr_5;                  //bot right
    struct cellinfo *next;
} cellinfo;

cellinfo *get_cell(int cell)
{
    cellinfo *curr;

    curr = g_cell;
    while(curr)
    {
        if (curr->idx == cell)
            return (curr);
        curr = curr->next;
    }
    return (NULL);
}

cellinfo *get_cell_relative(cellinfo *cell, int dir)
{
    if (cell == NULL)
        return NULL;
    switch(dir)
    {
        case 0:
            return get_cell(cell->nmr_0);
        case 1:
            return get_cell(cell->ntr_1);
        case 2:
            return get_cell(cell->ntl_2);
        case 3:
            return get_cell(cell->nml_3);
        case 4:
            return get_cell(cell->nbl_4);
        case 5:
            return get_cell(cell->nbr_5);
        default:
            return (NULL);
    }
    return (NULL);
}

int main()
{
    cellinfo *curr;
    g_cell = calloc(sizeof(cellinfo), 1);
    curr = g_cell;

    // amount of hexagonal cells in this map
    int number_of_cells;
    scanf("%d", &number_of_cells);
    for (int i = 0; i < number_of_cells; i++) 
    {
        curr->idx = i;
        scanf("%d%d%d%d%d%d%d%d", &curr->type, &curr->resources, &curr->nmr_0, &curr->ntr_1, &curr->ntl_2, &curr->nml_3, &curr->nbl_4, &curr->nbr_5);
        curr->next = calloc(sizeof(cellinfo), 1);
        curr = curr->next;
    }

	// TODO: create a struct that contains the stats of the map such as
	// number of bases and their indices, this could be just a cellinfo *bases_head;
	// where each node is just the node to the next owned base
	// we could also have multiple seperate linked lists for:
	// collectables, eggs, enemy_bases

    int number_of_bases;
    int my_base_index;

    scanf("%d", &number_of_bases);
    for (int i = 0; i < number_of_bases; i++) 
    {
        scanf("%d", &my_base_index);
    }
    for (int i = 0; i < number_of_bases; i++) 
    {
        int opp_base_index;
        scanf("%d", &opp_base_index);
    }
    bool isCollecting = 0;

    // game loop
    while (1) 
    {
        int idx = 0;
        int max = 0;
        int ants = 0;

        for (int i = 0; i < number_of_cells; i++) 
        {
            cellinfo *curr;
            curr = get_cell(i);
            int l_ants;
            scanf("%d%d%d", &curr->resources, &l_ants, &curr->op_ants);
            ants += l_ants;
            curr->my_ants = l_ants;

            if (curr->type == 2)
            {
                if (isCollecting == 0 && curr->resources > max)
                {
                    isCollecting = 1;
                    max = curr->resources;
                    idx = i;
                }
                if (isCollecting)
                {
                    if (idx == i && curr->resources == 0)
                        isCollecting = false;
                }
            }
        }

		// WILL CRASH IF THE CELLS RETURNED ARE NULL, TAILOR THE DIRECTION TO THE MAP.
		// EXAMPLE USAGE: Get the cell, n1 which is to the left (3) of the base.
		// Then get the cell to the left (3) of n1, called n2.
        cellinfo *n1 = get_cell_relative(get_cell(my_base_index), 3);
        cellinfo *n2 = get_cell_relative(n1, 3);
		assert("NULL CELL CRASH IMMINMENT", !n1 || !n2);

		// Places beacons at base, n1, n2
		action_message("WOAH!!");
        action_beacon(my_base_index, 100);
        action_beacon(n1->idx, 100);
        action_beacon(n2->idx, 100);

        // Create an action using action_<name>(arguments...)
        // To debug: use error and assert
		// action_line(int sourceIdx, int targetIdx, int strength) 
		// action_beacon(int cellIdx, int strength)
		// action_message(char *msg);
        // Or do nothing
		do_actions(); // ALWAYS NEEDED, if no actions are queued default action is 'WAIT'
    }
    return 0;
}