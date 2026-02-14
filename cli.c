#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "storage.h"

static int cli_add(int argc, char *argv[]);
static int cli_list(int argc, char *argv[]);
static int cli_search(int argc, char *argv[]);
static int cli_export(int argc, char *argv[]);
static void print_help(void);

int cli_handle(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    const bool help_command = strcmp(argv[1], "help") == 0 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0;
    const bool add_command = strcmp(argv[1], "add") == 0;
    const bool list_command = strcmp(argv[1], "list") == 0;
    const bool search_command = strcmp(argv[1], "search") == 0;
    const bool export_command = strcmp(argv[1], "export") == 0;

    if (help_command) {
        print_help();
        return 0;
    }

    if (add_command) {
        return cli_add(argc, argv);
    } else if (list_command) {
        return cli_list(argc, argv);
    } else if (search_command) {
        return cli_search(argc, argv);
    } else if (export_command) {
        return cli_export(argc, argv);
    } else {
        printf("Unknown command: %s\n\n", argv[1]);
        print_help();
        return 1;
    }
}

static void print_help(void) {
    printf("Brag CLI\n");
    printf("Usage:\n");
    printf("  brag add\n");
    printf("  brag list\n");
    printf("  brag search <term>\n");
    printf("  brag export [file]\n");
    printf("  brag help | -h | --help\n");
}

static int cli_add(int argc, char *argv[]) {
    (void)argv;
    if (argc > 2) {
        printf("Usage: brag add\n");
        return 1;
    }

    Entry entry;

    printf("Title: ");
    fgets(entry.title, sizeof(entry.title), stdin);
    entry.title[strcspn(entry.title, "\n")] = '\0';

    if (entry.title[0] == '\0') {
        printf("Title cannot be empty\n");
        return 1;
    }

    printf("Description: ");
    fgets(entry.description, sizeof(entry.description), stdin);
    entry.description[strcspn(entry.description, "\n")] = '\0';

    printf("Tags: ");
    fgets(entry.tags, sizeof(entry.tags), stdin);
    entry.tags[strcspn(entry.tags, "\n")] = '\0';

    if (save_entry(&entry) != 0) {
        printf("Error saving entry\n");
        return 1;
    }

    return 0;
}

static int cli_list(int argc, char *argv[]) {
    (void)argv;
    if (argc > 2) {
        printf("Usage: brag list\n");
        return 1;
    }
    
    return list_entries();
}

static int has_md_extension(const char *filename) {
    size_t len = strlen(filename);

    return len >= 3 && strcmp(filename + len - 3, ".md") == 0;
}

static int cli_export(int argc, char *argv[]) {
    char output_file[256];

    if (argc < 3) {
        snprintf(output_file, sizeof(output_file), "brag.md");
    } else if (has_md_extension(argv[2])) {
        snprintf(output_file, sizeof(output_file), "%s", argv[2]);
    } else {
        snprintf(output_file, sizeof(output_file), "%s.md", argv[2]);
    }

    return export_entries(output_file);
}

static int cli_search(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: brag search <term>\n");
        return 1;
    }

    return search_entries(argv[2]);
}