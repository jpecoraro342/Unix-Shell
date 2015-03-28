//C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
//My Stuff
#include "aliaslist.h"
 
struct node* head = NULL;
int size;
 
/*int main() {
    //test main
    print_aliases();
 
    struct alias alias1 = {"alias1", "alias1 command"};
    struct alias alias2 = {"alias2", "alias2 command"};
    struct alias alias3 = {"alias3", "alias3 command"};
    struct alias alias4 = {"alias4", "alias4 command"};
 
    add_alias(&alias1);
    add_alias(&alias2);
    add_alias(&alias3);
    //add_alias(&alias4);
 
    print_aliases();
 
    remove_alias_with_name(alias2.alias_name);
 
    print_aliases();
 
    return 0;
}*/
 
void print_aliases() {
    struct node* current = head;
     
    while (current != NULL) {
        printf("%s: %s\n", current->data->alias_name, current->data->full_command);
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
    struct node* current = head;
    if (head == NULL) {
        struct node newnode = {alias, NULL};
 
        head = &newnode;
        return;
    }
     
    while (current->next != NULL) {
        current = current->next;
    }
 
    struct node newnode = {alias, NULL};
 
    current->next = &newnode;
}
 
int remove_alias_with_name(char * alias_name) {
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