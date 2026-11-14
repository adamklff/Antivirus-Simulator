#include "cleanup.h"

void purge_all(void)
{
    free_list(&Clean);
    free_list(&Suspect);
    free_list(&Quarantine);
    free_sigs(&Signatures);
}
