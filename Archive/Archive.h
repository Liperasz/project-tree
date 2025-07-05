//
// Created by liperasz on 04/07/25.
//

#ifndef ARCHIEVE_H
#define ARCHIEVE_H

typedef struct {

    char computer[64];
    char username[64];
    char password[64];

} Credentials;

int loadFile(Tree* tree, char* name);
int loadCredentials(Credentials* data, char* file_name);
int saveCredentials(Credentials* data, char* file_name);
void printInfo(Credentials* data, Element* current_directory);
int login(Credentials* data);
int saveTreeToFile(Tree* tree, const char* file_name);

#endif //ARCHIEVE_H
