//C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//My Stuff
#include "aliaslist.h"

struct node* head = NULL;

void print_aliases() {
	struct node* current = head; 
	
	while (current != NULL) {
		printf("%s : %s\n", current->data->alias_name, current->data->full_command);
		current = current->next;
	}
}

//Not Using this
int get_index_for_alias_name(char * alias_name) {
	struct node* current = head; 
	int index = 0;
	
	while (current != NULL) {
		if (strcmp(alias_name, current->data->alias_name) == 0) {
			return index;
		}

		index++;
	}

	return -1;
}

struct alias* get_alias_with_name(char * alias_name) {
	struct node* current = head; 
	
	while (current != NULL) {
		if (strcmp(alias_name, current->data->alias_name) == 0) {
			return current->data;
		}

		current = current->next;
	}

	return NULL;
}

void add_alias(struct alias* alias) {
	if (head == NULL) {
        head = malloc(sizeof(struct node));
        head->data = alias;
        head->next = NULL;
		return;
	}
    
    struct node* current = head;
	
	while (current->next != NULL) {
		current = current->next;
	}

    current->next = malloc(sizeof(struct node));
    current->next->data = alias;
    current->next->next = NULL;
}

int remove_alias_with_name(char * alias_name) {
	if (head == NULL) {
		return -1;
	}
	if (strcmp(alias_name, head->data->alias_name) == 0) {
		if (head->next == NULL) {
			head = NULL;
		}
		else {
			head = head->next;
		}
		
		return 0;
	}

	struct node* current = head; 
	struct node* previous = head;
	
	while (current != NULL) {
		if (strcmp(alias_name, current->data->alias_name) == 0) {
			previous->next = current->next;
			return 0;
		}

		previous = current;
		current = current->next;
	}

	return -1;

}

void resetVisited() {
	struct node* current = head; 
	
	while (current != NULL) {
		current->data->visited = false;
		current = current->next;
	}
}