#include "cleanup.h"
#include "scan.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void help(void)
{
    puts("ADD_SIG <pattern> | DEL_SIG <pattern> | LOAD <name> <text>");
    puts("SCAN | QUAR <name> | RESTORE <name> | REPORT | PURGE | EXIT");
}

static char *split_word(char *input)
{
    while (*input && !isspace((unsigned char)*input))
        ++input;
    if (*input)
        *input++ = '\0';
    while (isspace((unsigned char)*input))
        ++input;
    return input;
}

static void load_sample(char *name)
{
    char *sample = split_word(name);
    if (!*name || !*sample) {
        puts("Usage: LOAD <name> <text>");
        return;
    }
    if (find_file(Clean, name) || find_file(Suspect, name) ||
        find_file(Quarantine, name)) {
        puts("A sample with that name already exists.");
        return;
    }
    FileRec *file = create_file(name, (const unsigned char *)sample, strlen(sample));
    if (!file)
        fputs("Could not allocate sample.\n", stderr);
    else
        insert_file(&Clean, file);
}

int main(void)
{
    char line[4096];
    help();
    while (fgets(line, sizeof line, stdin)) {
        if (!strchr(line, '\n') && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            fputs("Input line is too long.\n", stderr);
            continue;
        }
        line[strcspn(line, "\r\n")] = '\0';
        char *command = line;
        while (isspace((unsigned char)*command))
            ++command;
        char *arg = split_word(command);

        if (strcmp(command, "EXIT") == 0)
            break;
        if (strcmp(command, "ADD_SIG") == 0) {
            Sig *sig = create_sig(arg);
            if (!sig)
                fputs("Could not add signature. Enter a non-empty pattern.\n", stderr);
            else
                insert_sig(&Signatures, sig);
        } else if (strcmp(command, "DEL_SIG") == 0) {
            delete_sig(&Signatures, arg);
        } else if (strcmp(command, "LOAD") == 0) {
            load_sample(arg);
        } else if (strcmp(command, "SCAN") == 0) {
            scan_files(&Clean, &Suspect, Signatures);
        } else if (strcmp(command, "QUAR") == 0) {
            if (!find_file(Suspect, arg))
                puts("Sample not found in Suspect.");
            else
                move_file(&Suspect, &Quarantine, arg);
        } else if (strcmp(command, "RESTORE") == 0) {
            FileRec *file = remove_file(&Quarantine, arg);
            if (file) {
                file->suspicious = 0;
                insert_file(&Clean, file);
            } else {
                puts("Sample not found in Quarantine.");
            }
        } else if (strcmp(command, "REPORT") == 0) {
            print_list("Clean", Clean);
            print_list("Suspect", Suspect);
            print_list("Quarantine", Quarantine);
            print_sigs(Signatures);
        } else if (strcmp(command, "PURGE") == 0) {
            purge_all();
        } else if (*command) {
            help();
        }
    }
    int failed = ferror(stdin);
    purge_all();
    return failed ? 1 : 0;
}
