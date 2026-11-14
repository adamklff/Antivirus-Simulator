#include "siglist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Sig *Signatures;

Sig *create_sig(const char *pattern)
{
    if (!pattern || !*pattern)
        return NULL;
    Sig *sig = malloc(sizeof *sig);
    if (!sig)
        return NULL;
    sig->pattern = malloc(strlen(pattern) + 1);
    if (!sig->pattern) {
        free(sig);
        return NULL;
    }
    strcpy(sig->pattern, pattern);
    sig->next = NULL;
    return sig;
}

void insert_sig(Sig **head, Sig *node)
{
    if (!node)
        return;
    node->next = *head;
    *head = node;
}

void delete_sig(Sig **head, const char *pattern)
{
    while (*head) {
        Sig *sig = *head;
        if (strcmp(sig->pattern, pattern) == 0) {
            *head = sig->next;
            free(sig->pattern);
            free(sig);
            return;
        }
        head = &sig->next;
    }
}

void print_sigs(Sig *head)
{
    puts("Signatures");
    for (; head; head = head->next)
        printf("  %s\n", head->pattern);
}

void free_sigs(Sig **head)
{
    while (*head) {
        Sig *sig = *head;
        *head = sig->next;
        free(sig->pattern);
        free(sig);
    }
}
