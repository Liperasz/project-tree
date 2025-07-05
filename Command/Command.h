//
// Created by liperasz on 04/07/25.
//

#ifndef FUNCION_H
#define FUNCION_H
#include "../Tree/Tree.h"
#include "../Archive/Archive.h"

void command_cd(Element** current_directory, Element* root, char* path);
void command_search(Element* element, char* search_element, char* current_path, int* found_count);
int command_rm(Tree* tree, Element* current_directory, int r, char* item_name);
void command_list(Element* current_dir, int r);
int command_mkdir(Tree* tree, Element* current_directory, int p, char* name);
void command_clear();
void command_help();
int terminal(Credentials *data, Tree* tree, Element** current_directory);
int command_rename(Tree* tree, Element* current_directory, char* path, char* new_name);
int command_move(Tree* tree, Element* current_directory, char* source_path, char* dest_path);
int findElementAndParent(Element* root, const char* path, Element** parent_out, Element** element_out);
Element* detachElement(Element** root, const char* name);
void add_to_history(const char* command);
void show_history();
void free_history();



#endif //FUNCION_H
