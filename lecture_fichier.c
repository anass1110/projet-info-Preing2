#include "lecture_fichier.h"
#include <string.h>
#include <stdlib.h>

static void nettoyer_fin_ligne(char *chaine)
{
    size_t n;
    if (chaine == NULL) return;
    n = strlen(chaine);
    while (n > 0 && (chaine[n - 1] == '\n' || chaine[n - 1] == '\r')) {
        chaine[n - 1] = '\0';
        n--;
    }
}

static void copier_champ(char *destination, const char *source)
{
    if (destination == NULL) return;
    if (source == NULL) {
        destination[0] = '\0';
        return;
    }
    strncpy(destination, source, TAILLE_CHAMP - 1);
    destination[TAILLE_CHAMP - 1] = '\0';
    nettoyer_fin_ligne(destination);
}

int champ_est_vide_ou_tiret(const char *chaine)
{
    if (chaine == NULL) return 1;
    if (chaine[0] == '\0') return 1;
    if (strcmp(chaine, "-") == 0) return 1;
    return 0;
}

double lire_double_ou_0(const char *chaine)
{
    if (champ_est_vide_ou_tiret(chaine)) return 0.0;
    return atof(chaine);
}

int lire_ligne_csv(FILE *fichier, LigneCSV *ligne)
{
    char buffer[TAILLE_LIGNE];
    char *token;

    if (fichier == NULL || ligne == NULL) return 0;

    if (fgets(buffer, sizeof(buffer), fichier) == NULL) {
        return 0;
    }

    token = strtok(buffer, ";");
    copier_champ(ligne->col1, token);

    token = strtok(NULL, ";");
    copier_champ(ligne->col2, token);

    token = strtok(NULL, ";");
    copier_champ(ligne->col3, token);

    token = strtok(NULL, ";");
    copier_champ(ligne->col4, token);

    token = strtok(NULL, ";");
    copier_champ(ligne->col5, token);

    return 1;
}
