#include <stdbool.h>

struct alias {
	char *alias_name;
	char *full_command;
	bool visited; //Used to determine alias loop
};
 
struct node {
    struct alias* data;
    struct node* next;
};
 
/* Returns the alias with the given name, or NULL if not found */
struct alias* get_alias_with_name(char * alias_name);
 
/* not used */
int get_index_for_alias_name(char * alias_name);
 
/* Adds an alias to the end of the list */
void add_alias(struct alias* alias);
int remove_alias_with_name(char * alias_name);

void resetVisited();
 
void print_aliases();