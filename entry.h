#ifndef ENTRY_H
#define ENTRY_H

#define MAX_TITLE 100
#define MAX_DESC 500
#define MAX_TAGS 100
#define MAX_DATE 20

typedef struct {
    int id;
    char date[MAX_DATE];
    char title[MAX_TITLE];
    char description[MAX_DESC];
    char tags[MAX_TAGS];
} Entry;

#endif