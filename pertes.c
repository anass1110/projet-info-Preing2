#include "pertes.h"
#include "lecture_fichier.h"
#include "Utilitaire.h"
#include "AVL.h"
#include "arbre.h"
#include <stdio.h>
#include <string.h>



int obtenir_noeud_reseau(NoeudAVL **index,const char *identifiant,NoeudReseau **noeud_reseau){
    
    NoeudAVL *noeud_avl;
    NoeudReseau *nouveau_noeud;

    noeud_avl = avl_rechercher(*index, identifiant);
    if (noeud_avl != NULL && noeud_avl->ptr != NULL) {
        *noeud_reseau = (NoeudReseau *)noeud_avl->ptr;
        return 1;
    }

    nouveau_noeud = creer_noeud(identifiant);
    if (nouveau_noeud == NULL) {
        return 0;
    }

    noeud_avl = NULL;
    *index = avl_inserer_ptr(*index, identifiant, nouveau_noeud, &noeud_avl);
    if (noeud_avl == NULL) {
        liberer_noeud(nouveau_noeud);
        return 0;
    }

    *noeud_reseau = nouveau_noeud;
    return 1;
}


void liberer_noeuds_index(NoeudAVL *index)
{
    if (index == NULL) return;

    liberer_noeuds_index(index->gauche);
    liberer_noeuds_index(index->droit);

    if (index->ptr != NULL) {
        liberer_noeud((NoeudReseau *)index->ptr);
        index->ptr = NULL;
    }
}



int calculer_volume_reel_usine(const char *chemin_csv,const char *identifiant_usine,double *volume_reel)
{
    FILE *fichier_csv;
    LigneCSV ligne;
    double volume;

    volume = 0.0;
    fichier_csv = fopen(chemin_csv, "r");
    if (fichier_csv == NULL) {
        return 0;
    }

    while (lire_ligne_csv(fichier_csv, &ligne)) {
        if (champ_est_vide_ou_tiret(ligne.col1) &&             /* On cherche les lignes Spring -> Usine */
            commence_par(ligne.col2, "Spring") &&
            strcmp(ligne.col3, identifiant_usine) == 0 &&
            !champ_est_vide_ou_tiret(ligne.col4)) {

            double volume_source;
            double taux_fuite;

            volume_source = lire_double_ou_0(ligne.col4) / 1000.0;      /* Conversion m3 -> milliers de m3 si nécessaire, ou lecture directe */
            taux_fuite = lire_double_ou_0(ligne.col5);

            volume += volume_source * (1.0 - taux_fuite / 100.0);
        }
    }

    fclose(fichier_csv);
    *volume_reel = volume;
    return 1;
}




double calculer_pertes(NoeudReseau *noeud,double volume_entree){
    int nb_enfants;
    double part_volume;
    double pertes;
    Enfant *enfant;

    if (noeud == NULL) return 0.0;

    nb_enfants = compter_enfants(noeud);
    if (nb_enfants == 0) return 0.0;

    part_volume = volume_entree / (double)nb_enfants;
    pertes = 0.0;

    enfant = noeud->enfants;
    while (enfant != NULL) {
        double volume_perdu;
        double volume_apres;

        volume_perdu = part_volume * (enfant->fuite / 100.0);
        volume_apres = part_volume - volume_perdu;

        pertes += volume_perdu;
        pertes += calculer_pertes(enfant->noeud, volume_apres);

        enfant = enfant->suiv;
    }

    return pertes;
}

/* Bonus */
void trouver_troncon_max(NoeudReseau *noeud,double volume_entree,TronconMax *best){
    int nb_enfants;
    double part_volume;
    Enfant *enfant;

    if (noeud == NULL) return;

    nb_enfants = compter_enfants(noeud);
    if (nb_enfants == 0) return;

    part_volume = volume_entree / (double)nb_enfants;

    enfant = noeud->enfants;
    while (enfant != NULL) {
        double perdu;
        double apres;

        perdu = part_volume * (enfant->fuite / 100.0);
        apres = part_volume - perdu;

        if (perdu > best->perte_max) {
            best->perte_max = perdu;
            
            strncpy(best->amont, noeud->identifiant, 255);
            best->amont[255] = '\0';

            strncpy(best->aval, enfant->noeud->identifiant, 255);
            best->aval[255] = '\0';
        }

        trouver_troncon_max(enfant->noeud, apres, best);
        enfant = enfant->suiv;
    }
}


int calculer_leaks(const char *chemin_csv,const char *identifiant_usine,const char *chemin_sortie){
    FILE *fichier_csv;
    FILE *fichier_sortie;
    LigneCSV ligne;

    NoeudAVL *index_noeuds;
    NoeudReseau *racine_reseau;

    int usine_trouvee;
    double volume_reel;
    double volume_perdu;

    if (!calculer_volume_reel_usine(chemin_csv, identifiant_usine, &volume_reel)) {
        erreur("lecture du CSV impossible");                /* Pour vérifier les erreurs et problèmes */
        return 2;
    }

    fichier_csv = fopen(chemin_csv, "r");
    if (fichier_csv == NULL) {
        erreur("impossible d'ouvrir le fichier CSV");
        return 2;
    }

    index_noeuds = NULL;
    racine_reseau = NULL;
    usine_trouvee = 0;

    if (!obtenir_noeud_reseau(&index_noeuds, identifiant_usine, &racine_reseau)) {
        fclose(fichier_csv);
        erreur("manque memoire (racine reseau)");
        return 3;
    }

    
    while (lire_ligne_csv(fichier_csv, &ligne)) {          /* Construction du réseau */

        if (champ_est_vide_ou_tiret(ligne.col2) ||
            champ_est_vide_ou_tiret(ligne.col3)) {
            continue;
        }

        if (strcmp(ligne.col2, identifiant_usine) == 0 ||     /* L'usine existe si elle apparaît en col2 (émetteur)ou en col3 (récepteur) */
            strcmp(ligne.col3, identifiant_usine) == 0) {
            usine_trouvee = 1;
        }

        if (avl_rechercher(index_noeuds, ligne.col2) == NULL) {       /*  Si le parent (col2) n'est pas connu dans l'index, ce tronçon n'est pas (ou pas encore) relié à notre usine */
            continue;
        }

        {
            NoeudReseau *noeud_parent;       /* Le parent est connu, on peut traiter et attacher le fils */
            NoeudReseau *noeud_enfant;
            double taux_fuite;
            int r;

            taux_fuite = lire_double_ou_0(ligne.col5); 

            if (!obtenir_noeud_reseau(&index_noeuds, ligne.col2, &noeud_parent) ||
                !obtenir_noeud_reseau(&index_noeuds, ligne.col3, &noeud_enfant)) {

                fclose(fichier_csv);
                liberer_noeuds_index(index_noeuds);
                avl_liberer(index_noeuds);
                erreur("manque memoire (noeud reseau)");
                return 3;
            }

            r = ajouter_enfant(noeud_parent, noeud_enfant, taux_fuite);
            if (r == -1) {
                fclose(fichier_csv);
                liberer_noeuds_index(index_noeuds);
                avl_liberer(index_noeuds);
                erreur("manque memoire (liste enfants)");
                return 3;
            }
        }
    }

    fclose(fichier_csv);

    
    TronconMax best;           /* Calcul des résultats */
    best.perte_max = -1.0;
    best.amont[0] = '\0';
    best.aval[0] = '\0';

    if (!usine_trouvee) {
        volume_perdu = -1.0;    /* Code pour "Usine inconnue" */
    } else {
        volume_perdu = calculer_pertes(racine_reseau, volume_reel);
        trouver_troncon_max(racine_reseau, volume_reel, &best);
    }

    fichier_sortie = fopen(chemin_sortie, "a");       /* Écriture fichier de sortie (Mode Append) */
    if (fichier_sortie == NULL) {
        liberer_noeuds_index(index_noeuds);
        avl_liberer(index_noeuds);
        erreur("impossible d'ouvrir le fichier de sortie");
        return 2;
    }

    
    fseek(fichier_sortie, 0, SEEK_END);         /* Écriture de l'en-tête si le fichier est vide */
    if (ftell(fichier_sortie) == 0) {
        fprintf(fichier_sortie,
                "identifier;Leak volume (M.m3.year-1);"
                "worst_from;worst_to;worst_loss (M.m3.year-1)\n");
    }

    if (!usine_trouvee) {     /* usine inconnue => -1 partout pour indiquer l'erreur */
        fprintf(fichier_sortie, "%s;-1.000000;-;-;-1.000000\n", identifiant_usine);
    } else {
        fprintf(fichier_sortie, "%s;%.6f;%s;%s;%.6f\n",
                identifiant_usine,
                volume_perdu,
                best.amont,
                best.aval,
                best.perte_max);
    }

    fclose(fichier_sortie);

    
    liberer_noeuds_index(index_noeuds);    /* Nettoyage mémoire */
    avl_liberer(index_noeuds);

    return 0;
}
