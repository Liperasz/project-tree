//
// Created by liperasz on 04/07/25.
//
#include "../Tree/Tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Archive.h"
#include <unistd.h>

#include "../Command/Command.h"

int loadFile(Tree* tree, char* name) {
    printf("Loading archive\n");

    if (tree == NULL || *tree == NULL || name == NULL) {
        return 0;
    }

    FILE *file = fopen(name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    char line[512];

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // remove \n do final
        char full_line[1024];
        strcpy(full_line, line);


        char* dir_suffix = strstr(full_line, " [DIR]");
        if (dir_suffix != NULL) {
            *dir_suffix = '\0';
        }

        if (!insertElement(tree, full_line, 1, *tree)) {
            printf("Error inserting element: %s\n", full_line);
        }
    }

    fclose(file);
    return 1;
}


int saveCredentials(Credentials* data, char* file_name) {

    if (data == NULL || file_name == NULL) {
        return 0;
    }

    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    fwrite(data, sizeof(Credentials), 1, file);
    fclose(file);
    return 1;
}

int loadCredentials(Credentials* data, char* file_name) {

    if (data == NULL || file_name == NULL) {
        return 0;
    }

    FILE *file = fopen(file_name, "rb");

    if (file == NULL) {
        printf("Archive could not be opened\n");


        strncpy(data->computer, "desktop-0000", 63);

        char *user = getenv("USER");
        if (user == NULL) {
            user = getenv("LOGNAME");
        }
        if (user != NULL) {
            strncpy(data->username, user, 63);
        }
        else {
            strncpy(data->username, "user", 63);
        }

        printf("Insert a new password: ");
        if (fgets(data->password, 64, stdin) != NULL) {
            data->password[strcspn(data->password, "\n")] = '\0';
            printf("\n");
        } else {
            printf("\nError, this password can't be used.\n");
            return 0;
        }

        printf("Saving credentials\n");
        if (saveCredentials(data, file_name) == 0) {
            printf("Error saving credentials\n");
            return 0;
        }

        return 1;
    }

    printf("Loading...\n");
    size_t items_read = fread(data, sizeof(Credentials), 1, file);
    fclose(file);

    if (items_read == 1) {
        return 1;
    }

    printf("Error loading credentials\n");
    return 0;
}


void printInfo(Credentials* data, Element* current_directory) {

    if (data != NULL) {

        printf("(base) [%s@%s %s]$ ", data->username, data->computer, current_directory->name);

    }

}

int login(Credentials* data) {

    if (data == NULL) {
        return 0;
    }

    char username[64];
    char password[64];

    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, data->username) == 0 && strcmp(password, data->password) == 0) {
        printf("\nLoading\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return 1;
    }
    printf("\n Wrong credentials\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return 0;
}

void writePathsRecursive(Element* node, char* path, FILE* file) {
    if (node == NULL) return;

    char current_path[1024];
    if (strlen(path) == 0) {
        snprintf(current_path, sizeof(current_path), "%s", node->name);
    } else {
        snprintf(current_path, sizeof(current_path), "%s/%s", path, node->name);
    }

    // Salva diretórios e arquivos
    fprintf(file, "%s%s\n", current_path, node->is_directory ? " [DIR]" : "");

    writePathsRecursive(node->son, current_path, file);
    writePathsRecursive(node->brother, path, file);
}

int saveTreeToFile(Tree* tree, const char* file_name) {
    if (tree == NULL || *tree == NULL || file_name == NULL) return 0;

    FILE* file = fopen(file_name, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para salvar diretórios\n");
        return 0;
    }

    writePathsRecursive(*tree, "", file);  // Começa da raiz, pra salvar tudo
    fclose(file);
    return 1;
}

