#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char	*strdup(const char *s1)
{
	int		len;
	char	*s2;

	len = strlen(s1);
	s2 = calloc(len + 1, sizeof(char));
	if (!s2)
		return (NULL);
	strncpy(s2, s1, len + 1);
	return (s2);
}


char *strjoin(const char* s1, const char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);

    if (result) // thanks @pmg
    {
        strcpy(result, s1);
        strcat(result, s2);
    }

    return result;
}

typedef struct cellinfo cellinfo;
typedef struct action action;

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

void do_actions()
{
    action *curr = g_actions;
    char *actions;
    action *tmp;

    while (curr)
    {
        actions = strjoin(curr->action, ";");

        tmp = curr;
        curr = curr->next;
        
        free(tmp->action);
        free(tmp);
    }
    printf("%s\n", actions);
    free(actions);
}

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

cellinfo *get_cell_relative(int cell, int dir)
{
    switch(dir)
    {
        case 0:
            return get_cell(get_cell(cell)->nmr_0);
        case 1:
            return get_cell(get_cell(cell)->ntr_1);
        case 2:
            return get_cell(get_cell(cell)->ntl_2);
        case 3:
            return get_cell(get_cell(cell)->nml_3);
        case 4:
            return get_cell(get_cell(cell)->nbl_4);
        case 5:
            return get_cell(get_cell(cell)->nbr_5);
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
        //geminfo *gem = gems;
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

        action_line(my_base_index, idx, 100);
        action_line(my_base_index, get_cell_relative(get_cell(my_base_index)->idx, 0)->idx, 100);
        do_actions();

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");
        // WAIT | LINE <sourceIdx> <targetIdx> <strength> | BEACON <cellIdx> <strength> | MESSAGE <text>

        //printf("WAIT\n");
    }

    return 0;
}