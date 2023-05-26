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

action *g_actions;

int main()
{
	action_line(0, 1, 100);
	action_line(2, 3, 200);
	action_line(2, 3, 200);
	do_actions();
}