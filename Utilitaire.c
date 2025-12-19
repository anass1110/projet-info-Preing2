#include "Utilitaire.h"
#include <string.h>

int commence_par(const char *chaine, const char *prefixe){
    size_t n;
    if (chaine == NULL || prefixe == NULL) return 0;
    n = strlen(prefixe);
    return strncmp(chaine, prefixe, n) == 0;
}

void erreur(const char *message){
    if (message == NULL) return;
    fprintf(stderr, "Erreur : %s\n", message);
}
