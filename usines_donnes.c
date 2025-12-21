#include "usines_donnes.h"
#include "lecture_fichier.h"
#include "Utilitaire.h"
#include "AVL.h"
#include <stdio.h>
#include <string.h>

int generer_histo(const char *chemin_csv, const char *mode, const char *chemin_sortie) {
    FILE *fichier_csv, *fichier_sortie;
    LigneCSV ligne;
    NoeudAVL *racine = NULL;
    NoeudAVL *noeud_usine;
    int mode_histo;

    fichier_csv = fopen(chemin_csv, "r");
    if (fichier_csv == NULL) { erreur("impossible d'ouvrir le fichier CSV"); return 2; }

    while (lire_ligne_csv(fichier_csv, &ligne)) {

        /* 1) Lecture du max (inchangé) */
        if (champ_est_vide_ou_tiret(ligne.col1) &&
            commence_par(ligne.col2, "Facility complex") &&
            champ_est_vide_ou_tiret(ligne.col3) &&
            !champ_est_vide_ou_tiret(ligne.col4)) {
    
            racine = avl_inserer_ou_trouver(racine, ligne.col2, &noeud_usine);
            if (noeud_usine == NULL) { fclose(fichier_csv); avl_liberer(racine); return 3; }
    
            noeud_usine->v_max = lire_double_ou_0(ligne.col4) / 1000.0;
        }
    
        /* 2) Lecture de src/real : amont quelconque -> usine */
        if (champ_est_vide_ou_tiret(ligne.col1) &&
            !champ_est_vide_ou_tiret(ligne.col2) &&
            !commence_par(ligne.col2, "Facility complex") &&
            commence_par(ligne.col3, "Facility complex") &&
            !champ_est_vide_ou_tiret(ligne.col4)) {
    
            double volume_source = lire_double_ou_0(ligne.col4) / 1000.0;
            double taux_fuite = lire_double_ou_0(ligne.col5);
    
            if (taux_fuite < 0.0) taux_fuite = 0.0;
            if (taux_fuite > 100.0) taux_fuite = 100.0;
    
            racine = avl_inserer_ou_trouver(racine, ligne.col3, &noeud_usine);
            if (noeud_usine == NULL) { fclose(fichier_csv); avl_liberer(racine); return 3; }
    
            noeud_usine->v_src  += volume_source;
            noeud_usine->v_real += volume_source * (1.0 - taux_fuite / 100.0);
        }
    }
      
    fclose(fichier_csv);

    fichier_sortie = fopen(chemin_sortie, "w");
    if (fichier_sortie == NULL) { avl_liberer(racine); erreur("sortie impossible"); return 2; }

    if (strcmp(mode, "max") == 0) mode_histo = 0;
    else if (strcmp(mode, "src") == 0) mode_histo = 1;
    else if (strcmp(mode, "real") == 0) mode_histo = 2;
    else mode_histo = 3;

    if (mode_histo == 0) fprintf(fichier_sortie, "identifier;max volume (M.m3.year-1)\n");
    else if (mode_histo == 1) fprintf(fichier_sortie, "identifier;source volume (M.m3.year-1)\n");
    else if (mode_histo == 2) fprintf(fichier_sortie, "identifier;real volume (M.m3.year-1)\n");
    else fprintf(fichier_sortie, "identifier;max volume (M.m3.year-1);source volume (M.m3.year-1);real volume (M.m3.year-1)\n");

    avl_ecrire_infixe_inverse(fichier_sortie, racine, mode_histo);
    fclose(fichier_sortie);
    avl_liberer(racine);
    return 0;
}
