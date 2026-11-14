#include "scan.h"

#include <stdio.h>
#include <string.h>

static int matches(const FileRec *file, const Sig *sig)
{
    for (; sig; sig = sig->next) {
        size_t length = strlen(sig->pattern);
        if (!length || length > file->size)
            continue;
        for (size_t offset = 0; offset <= file->size - length; ++offset) {
            if (memcmp(file->bytes + offset, sig->pattern, length) == 0)
                return 1;
        }
    }
    return 0;
}

void scan_files(FileRec **clean, FileRec **suspect, Sig *sigs)
{
    while (*clean) {
        FileRec *file = *clean;
        if (matches(file, sigs)) {
            *clean = file->next;
            file->suspicious = 1;
            insert_file(suspect, file);
            printf("Suspect: %s\n", file->name);
        } else {
            clean = &file->next;
        }
    }
}
