//
// Created by liperasz on 04/07/25.
//

#ifndef TREE_H
#define TREE_H

typedef struct element {

    int is_directory;
    char *name;
    struct element *son;
    struct element *brother;

    struct element *father;
    struct element *sister; // Ponteiro anterior, tipo o brother mas para a esquerda

} Element;

typedef Element * Tree;

Tree* createTree();
void freeTree(Tree* tree);
void print_tree_recursive(Element* node, int depth);
void printTree(Tree* tree);

int insertElement(Tree* tree, char* name, int p, Element *element_father);
int removeElement(Tree* tree, char* name);

void getPath(Element* node, char* path);

void printSuggestions(Element* current, const char* prefix);

int renameElement(Tree* tree, const char* path, const char* new_name);


#endif //TREE_H
