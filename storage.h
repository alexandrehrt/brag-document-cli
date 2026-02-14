#ifndef STORAGE_H
#define STORAGE_H

#include "entry.h"

int save_entry(Entry *entry);
int list_entries();
int search_entries(const char *term);
int export_entries(const char *output_file);

#endif