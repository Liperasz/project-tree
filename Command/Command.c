//
// Created by liperasz on 04/07/25.
//

#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Tree/Tree.h"
#include "../Archive/Archive.h"
#include "../String/String.h"
#define MAX_HISTORY 100
char* history[MAX_HISTORY];
int history_count = 0;


void command_cd(Element** current_directory, Element* root, char* path) {

    if (path == NULL || strcmp(path, "~") == 0) {
        *current_directory = root;
        return;
    }

    Element* target = *current_directory;
    char path_copy[512];
    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* token = strtok(path_copy, "/");
    while (token) {
        if (strcmp(token, "..") == 0) {
            if (target->father != NULL)
                target = target->father;
        } else if (strcmp(token, ".") != 0) {
            Element* it = target->son;
            int found = 0;
            while (it) {
                if (it->is_directory && strcmp(it->name, token) == 0) {
                    target = it;
                    found = 1;
                    break;
                }
                it = it->brother;
            }
            if (!found) {
                printf("cd: directory not found: %s\n", token);
                printSuggestions(target->son, token);
                return;
            }
        }
        token = strtok(NULL, "/");
    }

    *current_directory = target;
}


void command_search(Element* element, char* search_element, char* current_path, int* found_count) {

    if (element == NULL) {
        return;
    }

    char full_path[1024];
    if (strlen(current_path) == 0) {
        sprintf(full_path, "%s", element->name);
    } else {
        sprintf(full_path, "%s/%s", current_path, element->name);
    }

    if (strcmp(element->name, search_element) == 0) {
        printf("  Founded in: root/%s\n", full_path);
        (*found_count)++;
    }

    command_search(element->son, search_element, full_path, found_count);
    command_search(element->brother, search_element, current_path, found_count);
}

int command_rm(Tree* tree, Element* current_directory, int r, char* item_name) {

    if (!tree || !current_directory || !item_name || isStringEmpty(item_name)) {
        return 0;
    }

    char path_copy[512];
    strncpy(path_copy, item_name, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    Element* target = current_directory;

    char* token = strtok(path_copy, "/");
    while (token != NULL && target != NULL) {
        Element* child = target->son;
        int found = 0;
        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                target = child;
                found = 1;
                break;
            }
            child = child->brother;
        }
        if (!found) {
            printf("rm: item not found: %s\n", item_name);
            return 0;
        }
        token = strtok(NULL, "/");
    }

    if (target == NULL) {
        printf("rm: item not found: %s\n", item_name);
        return 0;
    }

    if (target->is_directory && !r) {
        printf("rm: cannot remove '%s': Is a directory\n", item_name);
        return 0;
    }

    if (!removeElement(&(current_directory->son), item_name)) {
        printf("rm: failed to remove '%s'\n", item_name);
        return 0;
    }

    return 1;
}


int command_mkdir(Tree* tree, Element* current_directory, int p, char* name) {
    if (!tree || !current_directory || !name || isStringEmpty(name)) {
        return 0;
    }

    char full_path[1024] = "";
    getPath(current_directory, full_path);
    if (strlen(full_path) > 0) {
        strcat(full_path, "/");
    }
    strcat(full_path, name);

    if (!insertElement(&(current_directory->son), name, p, current_directory)) {
        printf("mkdir: cannot create '%s': Path invalid or missing - use -p?\n", name);
        return 0;
    }

    return 1;
}



void command_list(Element* current_dir, int r) {

    if (current_dir != NULL) {

        if (r == 1) {
            print_tree_recursive(current_dir->son, 0);

        } else {

            Element *element = current_dir->son;
            if (element != NULL) {
                while (element != NULL) {
                    printf("|- %s %s\n", element->name, element->is_directory ? "[DIR]" : "[FILE]");
                    element = element->brother;
                }
            }
            else {
                printf("Directory is empty\n");
            }

        }

    }
}

void command_clear() {

    system("cls");


}

void command_help() {

    printf("\n--- Help: Available Commands ---\n\n");
    printf("Usage: command [argument]\n\n");

    printf("  help\t\t\t - Displays this help message.\n");
    printf("  list\t\t\t - Lists the files and folders in the current directory.\n");
    printf("  cd <directory>\t - Changes the current directory to the specified <directory>.\n");
    printf("  mkdir <name>\t\t - Creates a new directory with the given <name> in the current location.\n");
    printf("  mkdir -p <path>\t - Creates the entire directory structure (e.g., a/b/c).\n");
    printf("  rm <name>\t\t - Removes the file or directory <name> from the current location.\n");
    printf("  search <term>\t\t - Searches for a file or folder throughout the entire tree.\n");
    printf("  clear\t\t\t - Clears the terminal screen.\n");
    printf("  exit\t\t\t - Exits the program and frees all memory.\n\n");
    printf("------------------------------------\n\n");
}

int command_rename(Tree* tree, Element* current_directory, char* path, char* new_name) {
    if (!tree || !current_directory || !path || !new_name || isStringEmpty(path) || isStringEmpty(new_name)) {
        return 0;
    }

    Element* target = current_directory;
    char path_copy[512];
    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* token = strtok(path_copy, "/");
    while (token != NULL && target != NULL) {
        Element* child = target->son;
        int found = 0;
        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                target = child;
                found = 1;
                break;
            }
            child = child->brother;
        }
        if (!found) {
            return 0;
        }
        token = strtok(NULL, "/");
    }

    if (target == NULL || target->name == NULL) {
        return 0;
    }

    free(target->name);
    target->name = malloc(strlen(new_name) + 1);
    if (!target->name) return 0;

    strcpy(target->name, new_name);
    return 1;
}
Element* findElement(Element* root, const char* path) {
    if (root == NULL || path == NULL || *path == '\0') return NULL;

    // copia para manipular
    char path_copy[512];
    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    Element* current = root;

    char* token = strtok(path_copy, "/");
    while (token != NULL && current != NULL) {
        Element* child = current->son;
        int found = 0;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                current = child;
                found = 1;
                break;
            }
            child = child->brother;
        }

        if (!found) return NULL;  // não encontrou esse token no filho atual

        token = strtok(NULL, "/");
    }

    return current;
}

int findElementAndParent(Element* root, const char* path, Element** parent_out, Element** element_out) {

    if (!root || !path || !parent_out || !element_out) return 0;

    char path_copy[512];
    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* token = strtok(path_copy, "/");
    Element* current = root;
    Element* parent = NULL;

    while (token != NULL && current != NULL) {
        Element* child = current->son;
        parent = current;
        current = NULL;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                current = child;
                break;
            }
            child = child->brother;
        }

        token = strtok(NULL, "/");
    }

    if (current == NULL) {
        return 0;
    }

    *parent_out = parent;
    *element_out = current;
    return 1;
}

// Apenas remove o elemento da lista de filhos, sem liberar memória
Element* detachElement(Element** root, const char* name) {
    if (!root || !*root || !name) return NULL;

    Element* current = *root;
    Element* prev = NULL;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (prev) {
                prev->brother = current->brother;
            } else {
                *root = current->brother;
            }
            if (current->brother) {
                current->brother->sister = prev;
            }
            current->brother = NULL;
            current->sister = NULL;
            return current;  // Retorna o elemento desanexado
        }
        prev = current;
        current = current->brother;
    }

    return NULL;
}

int command_move(Tree* tree, Element* current_directory, char* source_path, char* dest_path) {
    if (!tree || !*tree || !source_path || !dest_path) {
        printf("move: parâmetros inválidos\n");
        return 0;
    }

    // 1. Localiza o elemento de origem
    Element* source_parent = NULL;
    Element* source = NULL;
    if (!findElementAndParent(*tree, source_path, &source_parent, &source)) {
        printf("move: origem nao encontrada: %s\n", source_path);
        return 0;
    }

    // 2. Localiza o diretório de destino
    Element* dest = findElement(*tree, dest_path);
    if (!dest || !dest->is_directory) {
        printf("move: destino inválido ou não é um diretório: %s\n", dest_path);
        return 0;
    }

    // 3. Evitar mover para dentro de um filho (loop)
    Element* iter = dest;
    while (iter != NULL) {
        if (iter == source) {
            printf("move: não é possível mover um diretório para dentro de si mesmo ou de um dos seus descendentes.\n");
            return 0;
        }
        iter = iter->father;
    }

    // 4. Remover do local original
    Element* detached = detachElement(&(source_parent->son), source->name);
    if (!detached) {
        printf("move: falha ao remover de origem\n");
        return 0;
    }
    source = detached;

    // 5. Re-inserir no destino
    source->brother = NULL;
    source->sister = NULL;
    source->father = dest;

    // Adiciona como filho do novo destino
    if (!dest->son) {
        dest->son = source;
    } else {
        Element* it = dest->son;
        while (it->brother) it = it->brother;
        it->brother = source;
        source->sister = it;
    }

    return 1;
}

void add_to_history(const char* command) {
    if (history_count >= MAX_HISTORY) return;

    history[history_count] = malloc(strlen(command) + 1);
    if (history[history_count]) {
        strcpy(history[history_count], command);
        history_count++;
    }
}
void show_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}
void free_history() {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
    history_count = 0;
}


int terminal(Credentials *data, Tree* tree, Element** current_directory) {

    printInfo(data, *current_directory);

    char* args[10];
    int arg_count = parseInput(args, 10);

    if (arg_count == -1) {
        printf("\nQuiting program...\n");
        return 0;
    }

    if (arg_count == 0) {
        return 1;
    }

    // Monta a linha do comando para adicionar ao histórico
    char input_line[512] = "";
    for (int i = 0; i < arg_count; i++) {
        strcat(input_line, args[i]);
        if (i < arg_count - 1) strcat(input_line, " ");
    }
    add_to_history(input_line);  // Adiciona comando ao histórico

    char* command = args[0];

    if (strcmp(command, "help") == 0) {
        command_help();
    }
    else if (strcmp(command, "clear") == 0) {
        command_clear();
    }
    else if (strcmp(command, "list") == 0) {
        if (arg_count > 1) {
            if (strcmp(args[1], "-r") == 0) {
                command_list(*current_directory, 1);
            } else {
                printf("Usage: list <argument>\n");
            }
        }
        else {
            command_list(*current_directory, 0);
        }
    }
    else if (strcmp(command, "cd") == 0) {
        if (arg_count > 1) {
            command_cd(current_directory, *tree,args[1]);
        } else {
            printf("Usage: cd <directory>\n");
        }
    }
    else if (strcmp(command, "mkdir") == 0) {
        int p = 0;
        char* directory_name = args[1];

        if (arg_count > 2 && strcmp(args[1], "-p") == 0) {
            p = 1;
            directory_name = args[2];
        }

        if (directory_name == NULL) {
            printf("Usage: mkdir [-p] <name>\n");
        } else {
            command_mkdir(tree, *current_directory, p, directory_name);
        }
    }
    else if (strcmp(command, "rm") == 0) {
        int r = 0;
        char* item_name = args[1];

        if (arg_count > 2 && strcmp(args[1], "-r") == 0) {
            r = 1;
            item_name = args[2];
        }

        if (item_name == NULL) {
            printf("Usage: rm [-r] <name>\n");
        } else {
            command_rm(tree, *current_directory, r, item_name);
        }
    }
    else if (strcmp(command, "search") == 0) {
        if (arg_count > 1) {
            printf("Looking for '%s'...\n", args[1]);

            int found_count = 0;
            char initial_path[1] = "";

            command_search((*tree)->son, args[1], initial_path, &found_count);

            if (found_count == 0) {
                printf("Item not founded'%s'.\n", args[1]);
            }
            printf("\n");

        } else {
            printf("Usage: search <name>\n");
        }
    }
    else if (strcmp(command, "rename") == 0) {
        if (arg_count < 3) {
            printf("Usage: rename <path> <new_name>\n");
        } else {
            if (command_rename(tree, *current_directory, args[1], args[2])) {
                printf("Renomeado com sucesso.\n");
            } else {
                printf("Erro ao renomear. Verifique o caminho.\n");
            }
        }
    }
    else if (strcmp(command, "move") == 0) {
        if (arg_count < 3) {
            printf("Usage: move <source> <destination>\n");
        } else {
            if (!command_move(tree, *current_directory, args[1], args[2])) {
                printf("move: operação falhou\n");
            }
        }
    }
    else if (strcmp(command, "history") == 0) {
        show_history();  // Mostra o histórico de comandos
    }
    else if (strcmp(command, "exit") == 0) {
        return 0;
    }
    else {
        printf("Unknown command\n");
    }

    return 1;
}
