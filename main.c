#include <stdio.h>
#include <string.h>

#include "usines_donnes.h"
#include "pertes.h"
#include "Utilitaire.h"

void afficher_usage(void)
{
    printf("Usage :\n");
    printf("  ./wildwater histo <csv> <max|src|real|all> <fichier_sortie>\n");
    printf("  ./wildwater leaks <csv> <identifiant_usine> <fichier_sortie>\n");
}

int main(int argc, char **argv)
{
    if (argc != 5) {
        erreur("mauvais nombre d'arguments");
        afficher_usage();
        return 1;
    }

    if (strcmp(argv[1], "histo") == 0) {
        if (strcmp(argv[3], "max") != 0 &&
            strcmp(argv[3], "src") != 0 &&
            strcmp(argv[3], "real") != 0 &&
            strcmp(argv[3], "all") != 0) {
            erreur("mode histo invalide");
            return 1;
        }
        return generer_histo(argv[2], argv[3], argv[4]);
    }

    if (strcmp(argv[1], "leaks") == 0) {
        return calculer_leaks(argv[2], argv[3], argv[4]);
    }

    erreur("commande inconnue");
    afficher_usage();
    return 1;
}

