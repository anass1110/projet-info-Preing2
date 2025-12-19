#ifndef LECTURE_FICHIER_H
#define LECTURE_FICHIER_H

#include <stdio.h>

#define TAILLE_CHAMP 256
#define TAILLE_LIGNE 1024

typedef struct {
    char col1[TAILLE_CHAMP];
    char col2[TAILLE_CHAMP];
    char col3[TAILLE_CHAMP];
    char col4[TAILLE_CHAMP];
    char col5[TAILLE_CHAMP];
} LigneCSV;

/* Lit une ligne CSV  */
int lire_ligne_csv(FILE *fichier, LigneCSV *ligne);

/* Tests pour les champs */
int champ_est_vide_ou_tiret(const char *chaine);

/* Conversion pour sécurisée */
double lire_double_ou_0(const char *chaine);

#endif
