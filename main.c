#include <stdio.h>
#include <stdlib.h>
#include "Tree/Tree.h"
#include "Archive/Archive.h"
#include "Command/Command.h"

int main () {


    Credentials *credentials = malloc(sizeof(Credentials));
    char credentials_path[100] = "../config/config.bin";
    if (loadCredentials(credentials, credentials_path) == 0) {
        free(credentials);
        printf("Exiting program due to an error...");
        return 0;
    }

    while (login(credentials) != 1) {}

    Tree* tree = createTree();
    char directory_path[100] = "../directory.txt";
    loadFile(tree, directory_path);

    Element **current_directory = tree;
    while (terminal(credentials, tree, current_directory) == 1) {}


    if (!saveTreeToFile(tree, directory_path)) {
        printf("Erro ao salvar a árvore de diretórios.\n");
    } else {
        printf("Árvore salva com sucesso em %s\n", directory_path);
    }

    freeTree(tree);
    free(tree);
    free_history();

    return 0;

}