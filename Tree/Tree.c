//
// Created by liperasz on 04/07/25.
//

#include "Tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../String/String.h"


Tree* createTree() {

    Tree* tree = (Tree*) malloc(sizeof(Tree));
    if (tree != NULL) {

        Element* root = malloc(sizeof(Element));
        if (root == NULL) {
            free(tree);
            return NULL;
        }

        root->name = malloc(5 * sizeof(char));
        strcpy(root->name, "root");
        root->brother = NULL;
        root->son = NULL;
        root->father = NULL;
        root->sister = NULL;
        root->is_directory = 1;

        *tree = root;
    }
    return tree;

}

void freeTree(Tree* tree) {

    if (tree == NULL || *tree == NULL) {
        return;
    }

    if ((*tree)->son != NULL) {
        freeTree(&(*tree)->son);
    }

    if ((*tree)->brother != NULL) {
        freeTree(&(*tree)->brother);
    }

    free((*tree)->name);
    free(*tree);
    *tree = NULL;
}


void print_tree_recursive(Element* node, int depth) {

    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("|- %s %s\n", node->name, node->is_directory ? "[DIR]" : "[FILE]");

    if (node->son != NULL) {
        print_tree_recursive(node->son, depth + 1);
    }

    if (node->brother != NULL) {
        print_tree_recursive(node->brother, depth);
    }
}

void printTree(Tree* tree) {

    if (tree == NULL || *tree == NULL) {
        printf("Árvore está vazia.\n");
        return;
    }
    printf("Estrutura da Árvore:\n");
    print_tree_recursive(*tree, 0);

    printf("\n\n\n");
}


int insertElement(Tree* tree, char* name, int p, Element* element_father) {
    if (tree == NULL || name == NULL || *name == '\0' || isStringEmpty(name)) {
        return 0;
    }

    // CASO BASE: nome simples (sem '/')
    if (strchr(name, '/') == NULL) {
        // Verifica duplicata no nível atual
        Element* check = *tree;
        while (check != NULL) {
            if (strcmp(check->name, name) == 0) {
                return 0;  // Já existe, não insere de novo
            }
            check = check->brother;
        }

        // Cria novo elemento
        Element* element = (Element*) malloc(sizeof(Element));
        if (element == NULL) return 0;

        element->name = malloc((strlen(name) + 1) * sizeof(char));
        strcpy(element->name, name);
        element->is_directory = !verifyIsFile(element->name);
        element->son = NULL;
        element->brother = NULL;
        element->father = element_father;

        // Insere como primeiro ou último irmão
        if (*tree == NULL) {
            *tree = element;
            element->sister = NULL;
        } else {
            Element* current = *tree;
            while (current->brother != NULL) {
                current = current->brother;
            }
            current->brother = element;
            element->sister = current;
        }

        return 1;
    }

    // CASO RECURSIVO: caminho com '/'
    char find_directory_two[256];
    if (!copyName(find_directory_two, sizeof(find_directory_two), name)) {
        return 0;
    }

    // Procura diretório no nível atual
    Element* find_element = *tree;
    while (find_element != NULL) {
        if (strcmp(find_element->name, find_directory_two) == 0 && find_element->is_directory) {
            break;
        }
        find_element = find_element->brother;
    }

    // Se não encontrou e -p está ativado, cria diretório
    if (find_element == NULL && p == 1) {
        // Verifica duplicata antes de criar
        Element* check = *tree;
        while (check != NULL) {
            if (strcmp(check->name, find_directory_two) == 0) {
                find_element = check;
                break;
            }
            check = check->brother;
        }

        if (find_element == NULL) {
            Element* new_dir = (Element*) malloc(sizeof(Element));
            new_dir->name = malloc(strlen(find_directory_two) + 1);
            strcpy(new_dir->name, find_directory_two);
            new_dir->is_directory = 1;
            new_dir->son = NULL;
            new_dir->brother = NULL;
            new_dir->father = element_father;

            // Insere novo diretório
            if (*tree == NULL) {
                *tree = new_dir;
                new_dir->sister = NULL;
            } else {
                Element* current = *tree;
                while (current->brother != NULL) {
                    current = current->brother;
                }
                current->brother = new_dir;
                new_dir->sister = current;
            }

            find_element = new_dir;
        }
    }

    // Falhou ao localizar/criar diretório
    if (find_element == NULL) {
        printf("Error finding directory: %s\n", find_directory_two);
        return 0;
    }

    // Chamada recursiva para o restante do caminho
    return insertElement(&find_element->son, strchr(name, '/') + 1, p, find_element);
}


int removeElement(Tree* tree, char* name) {


    if (tree == NULL || *tree == NULL || (*tree)->name == NULL || name == NULL || isStringEmpty(name)) {
        return 0;
    }

    if (strchr(name, '/') == NULL) {
        Element *previous_element = NULL;
        Element *find_element = *tree;
        while (find_element != NULL && strcmp(name, find_element->name)) {
            previous_element = find_element;
            find_element = find_element->brother;
        }
        if (find_element == NULL) {
            printf("Error finding item\n");
            return 0;
        }
        if (strcmp(name, find_element->name) == 0) {
            if (previous_element != NULL) {
                previous_element->brother = find_element->brother;
            } else {
                *tree = find_element->brother;
            }

            if (find_element->brother != NULL) {
                find_element->brother->sister = previous_element;
            }

            find_element->son = NULL;
            find_element->brother = NULL;
            freeTree(&find_element);

            return 1;
        }

        printf("Unexpected error\n");
        return 0;

    }


    Element *find_element = *tree;
    while (find_element != NULL) {
        while (!isEqualName(find_element->name, name)) {
            find_element = find_element->brother;
        }
        if (isEqualName(find_element->name, name) && find_element->is_directory) {
            break;
        }
    }


    if (find_element == NULL || !isEqualName(find_element->name, name) || !find_element->is_directory) {
        printf("Error finding directory\n");
        return 0;
    }

   if (removeElement(&find_element->son, strchr(name, '/') + 1)) {
       return 1;
   }


    return 0;

}

// A função sobe os nós até chegar no root
void getPath(Element* node, char* path) {

    if (node == NULL || node->father == NULL) {
        return;
    }

    getPath(node->father, path);
    if (strlen(path) > 0) {
        strcat(path, "/");
    }
    strcat(path, node->name);
}
void printSuggestions(Element* current, const char* prefix) {
    int found = 0;

    while (current != NULL) {
        if (current->is_directory && strncmp(current->name, prefix, strlen(prefix)) == 0) {
            printf("  Possivel alternativa: %s\n", current->name);
            found = 1;
        }
        current = current->brother;
    }

    if (!found) {
        printf("Diretório não encontrado\n");
    }
}

int renameElement(Tree* tree, const char* path, const char* new_name) {
    if (tree == NULL || *tree == NULL || path == NULL || new_name == NULL) {
        return 0;
    }

    Element* current = *tree;
    char buffer[256];
    strncpy(buffer, path, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char* token = strtok(buffer, "/");
    Element* parent = NULL;

    while (token != NULL && current != NULL) {
        Element* found = NULL;
        while (current != NULL) {
            if (strcmp(current->name, token) == 0) {
                found = current;
                break;
            }
            current = current->brother;
        }

        if (!found) {
            return 0; // Caminho inválido
        }

        parent = found;
        current = found->son;
        token = strtok(NULL, "/");
    }

    if (parent == NULL) return 0;

    // Renomear
    free(parent->name);
    parent->name = malloc(strlen(new_name) + 1);
    strcpy(parent->name, new_name);

    return 1;
}
