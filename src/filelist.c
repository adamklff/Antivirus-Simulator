#include "filelist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FileRec *Clean;
FileRec *Suspect;
FileRec *Quarantine;

static void free_file(FileRec *file)
{
    free(file->bytes);
    free(file->name);
    free(file);
}

FileRec *create_file(const char *name, const unsigned char *data, size_t size)
{
    if (!name || (size && !data))
        return NULL;

    FileRec *file = calloc(1, sizeof *file);
    if (!file)
        return NULL;

    file->name = malloc(strlen(name) + 1);
    file->bytes = size ? malloc(size) : NULL;
    if (!file->name || (size && !file->bytes)) {
        free_file(file);
        return NULL;
    }

    strcpy(file->name, name);
    if (size)
        memcpy(file->bytes, data, size);
    file->size = size;
    return file;
}

void insert_file(FileRec **head, FileRec *node)
{
    if (!node)
        return;
    node->next = *head;
    *head = node;
}

FileRec *remove_file(FileRec **head, const char *name)
{
    while (*head) {
        FileRec *file = *head;
        if (strcmp(file->name, name) == 0) {
            *head = file->next;
            file->next = NULL;
            return file;
        }
        head = &file->next;
    }
    return NULL;
}

FileRec *find_file(FileRec *head, const char *name)
{
    for (; head; head = head->next) {
        if (strcmp(head->name, name) == 0)
            return head;
    }
    return NULL;
}

void move_file(FileRec **from, FileRec **to, const char *name)
{
    insert_file(to, remove_file(from, name));
}

void print_list(const char *title, FileRec *head)
{
    printf("%s\n", title);
    for (; head; head = head->next)
        printf("  %s (%zu bytes)\n", head->name, head->size);
}

void free_list(FileRec **head)
{
    while (*head) {
        FileRec *file = *head;
        *head = file->next;
        free_file(file);
    }
}
