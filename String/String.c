#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "String.h"

// Função que verifica se uma string está vazia ou contém apenas espaços em branco.
int isStringEmpty(const char *str) {
    if (str == NULL) {
        return 1;
    }

    while (*str != '\0') {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }

    return 1;
}

// Verifica se o nome é correto
int isEqualName(const char *base, const char *fullString) {
    const char *slashPosition = strchr(fullString, '/');

    if (slashPosition == NULL) {
        return 0;
    }

    size_t lengthBeforeSlash = slashPosition - fullString;

    if (lengthBeforeSlash != strlen(base)) {
        return 0;
    }

    if (strncmp(base, fullString, lengthBeforeSlash) == 0) {
        return 1;
    }

    return 0;
}

// Função que copioa tudo antes do /
int copyName(char *destination, size_t dest_size, const char *source) {

    const char *slash_position = strchr(source, '/');

    if (slash_position == NULL) {
        return 0;
    }

    size_t length_to_copy = slash_position - source;

    if (dest_size < length_to_copy + 1) {
        printf("Error: buffer out of space \n");
        return 0;
    }

    strncpy(destination, source, length_to_copy);

    destination[length_to_copy] = '\0';

    return 1; // Sucesso!
}

// Verifica através do nome se é um arquivo ou um diretório
int verifyIsFile(const char* filename) {
    const char* valid_extensions[] = {
        "txt", "pdf", "PDF", "log",
        "jpg", "jpeg", "JPG", "png", "PNG",
        "doc", "docx", "xls", "xlsx",
        "exe", "zip", "tar", "gz"
    };

    if (filename == NULL || *filename == '\0') {
        return 0;
    }

    const char* dot_ptr = strrchr(filename, '.');

    if (dot_ptr == NULL || dot_ptr == filename || *(dot_ptr + 1) == '\0') {
        return 0;
    }

    const char* extension = dot_ptr + 1;

    int num_extensions = sizeof(valid_extensions) / sizeof(valid_extensions[0]);
    for (int i = 0; i < num_extensions; i++) {
        if (strcmp(extension, valid_extensions[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

// Divide o input em várias palavras
int parseInput(char* args[], int max_args) {

    static char input_buffer[512];

    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return -1;
    }

    input_buffer[strcspn(input_buffer, "\n")] = '\0';

    int arg_count = 0;
    char* token = strtok(input_buffer, " ");

    while (token != NULL && arg_count < max_args - 1) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ");
    }

    args[arg_count] = NULL;

    return arg_count;
}
