//
// Created by liperasz on 04/07/25.
//

#ifndef STRING_H
#define STRING_H

int isStringEmpty(const char *str);
int isEqualName(const char *base, const char *fullString);
int copyName(char *destination, size_t dest_size, const char *source);
int verifyIsFile(const char* filename);
int parseInput(char* args[], int max_args);

#endif //STRING_H
