#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "storage.h"

#define DB_FILE "brag.db"

static int strcasestr_match(const char *haystack, const char *needle);
static int parse_line(const char *line, Entry *entry);
static void format_date(const char *stored_date, char *output, size_t size);
static void print_entry(const Entry *entry, const char *display_date);


int save_entry(Entry *entry) {
    FILE *file;
    char line[1024];
    int last_id = 0;
    int current_id;

    // Find last ID
    file = fopen(DB_FILE, "r");

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "%d", &current_id) == 1) {
                last_id = current_id;
            }
        }
        fclose(file);
    }

    int new_id = last_id + 1;
    entry->id = new_id;

    // Generate current date
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    strftime(entry->date, sizeof(entry->date),
             "%Y-%m-%d %H:%M:%S", local);

    // Open file for append
    file = fopen(DB_FILE, "a");

    if (file == NULL) {
        printf("Error: could not open file.\n");
        return 1;
    }

    // Write in delimited format
    fprintf(file, "%d|%s|%s|%s|%s\n",
            entry->id,
            entry->date,
            entry->title,
            entry->description,
            entry->tags);

    fclose(file);

    return 0;
}

int list_entries() {
    FILE *file = fopen(DB_FILE, "r");

    if (file == NULL) {
        return 1;
    }

    char line[1024];
    Entry entry;
    char display_date[MAX_DATE];

    while (fgets(line, sizeof(line), file)) {
        if (parse_line(line, &entry)) {
            format_date(entry.date, display_date, sizeof(display_date));
            print_entry(&entry, display_date);
        }
    }

    fclose(file);

    return 0;
}

int search_entries(const char *term) {
    FILE *file = fopen(DB_FILE, "r");

    if (file == NULL) {
        return 1;
    }

    char line[1024];
    Entry entry;
    char display_date[MAX_DATE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (!parse_line(line, &entry)) {
            continue;
        }

        if (strcasestr_match(entry.title, term) ||
            strcasestr_match(entry.description, term) ||
            strcasestr_match(entry.tags, term)) {
            format_date(entry.date, display_date, sizeof(display_date));
            print_entry(&entry, display_date);
            found++;
        }
    }

    fclose(file);

    if (found == 0) {
        printf("No results found for \"%s\"\n", term);
    }

    return 0;
}

int export_entries(const char *output_file) {
    FILE *db = fopen(DB_FILE, "r");

    if (db == NULL) {
        printf("Error: could not open database.\n");
        return 1;
    }

    FILE *out = fopen(output_file, "w");

    if (out == NULL) {
        printf("Error: could not create %s\n", output_file);
        fclose(db);
        return 1;
    }

    fprintf(out, "# Brag Document\n\n");

    char line[1024];
    Entry entry;
    char display_date[MAX_DATE];

    while (fgets(line, sizeof(line), db)) {
        if (!parse_line(line, &entry)) {
            continue;
        }

        format_date(entry.date, display_date, sizeof(display_date));

        fprintf(out, "## %s\n", entry.title);
        fprintf(out, "**Date:** %s\n\n", display_date);

        if (entry.description[0] != '\0') {
            fprintf(out, "%s\n\n", entry.description);
        }

        if (entry.tags[0] != '\0') {
            fprintf(out, "**Tags:** %s\n\n", entry.tags);
        }

        fprintf(out, "---\n\n");
    }

    fclose(db);
    fclose(out);

    printf("Exported to %s\n", output_file);

    return 0;
}

static int strcasestr_match(const char *haystack, const char *needle) {
    size_t needle_len = strlen(needle);
    size_t haystack_len = strlen(haystack);

    if (needle_len > haystack_len) {
        return 0;
    }

    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        size_t j;
        for (j = 0; j < needle_len; j++) {
            if (tolower((unsigned char)haystack[i + j]) != tolower((unsigned char)needle[j])) {
                break;
            }
        }
        if (j == needle_len) {
            return 1;
        }
    }

    return 0;
}

static int parse_line(const char *line, Entry *entry) {
    return sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^\n]",
                  &entry->id, entry->date, entry->title,
                  entry->description, entry->tags) == 5;
}

static void format_date(const char *stored_date, char *output, size_t size) {
    int year, month, day;

    if (sscanf(stored_date, "%d-%d-%d", &year, &month, &day) == 3) {
        snprintf(output, size, "%02d-%02d-%04d", day, month, year);
    } else {
        snprintf(output, size, "%s", stored_date);
    }
}

static void print_entry(const Entry *entry, const char *display_date) {
    printf("%d|%s|%s|%s|%s\n",
           entry->id, display_date,
           entry->title, entry->description, entry->tags);
}