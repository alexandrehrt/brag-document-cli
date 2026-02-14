# Brag CLI

A simple command-line tool to track your daily accomplishments. Written in C.

## Features

- Add entries with title, description, and tags
- List all entries
- Search entries by keyword (case-insensitive)
- Export entries to a Markdown file
- Remove entries by ID

## Requirements

- GCC
- macOS or Linux

## Build

```bash
make
```

## Install

```bash
sudo make install
```

This compiles the project and copies the binary to `/usr/local/bin`.

## Uninstall

```bash
sudo make uninstall
```

## Usage

```
brag add                 Add a new entry (interactive)
brag list                List all entries
brag search <term>       Search entries by keyword
brag export [file]       Export entries to Markdown (default: ~/brag.md)
brag remove <id>         Remove an entry by ID
brag help                Show help
```

### Examples

```bash
# Add a new entry
brag add

# List all entries
brag list

# Search for entries containing "bugfix"
brag search bugfix

# Export to ~/brag.md
brag export

# Export to ~/report.md
brag export report

# Remove entry with ID 3
brag remove 3
```

## Storage

Entries are stored in `~/.brag/brag.db` using a pipe-delimited format:

```
id|date|title|description|tags
```

## Project Structure

```
main.c       Entry point
cli.c        Command-line interface
cli.h        CLI header
storage.c    Data persistence (save, list, search, export, remove)
storage.h    Storage header
entry.h      Entry struct definition
Makefile     Build configuration
```
