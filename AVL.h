#ifndef AVL_H
#define AVL_H

#include <stdio.h>

typedef struct NoeudAVL {
    char identifiant[256];

    /* Pour l'histogramme */
    double v_max;
    double v_src;
    double v_real;

    /* Pour les pertes */
    void *ptr;

    int hauteur;
    struct NoeudAVL *gauche;
    struct NoeudAVL *droit;
} NoeudAVL;



/* recherche un noeud existant */
NoeudAVL *avl_rechercher(NoeudAVL *racine, const char *identifiant);

/* insère avec le pointeur ptr */
NoeudAVL *avl_inserer_ptr(NoeudAVL *racine,const char *identifiant,void *ptr,NoeudAVL **noeud_trouve);

NoeudAVL *avl_inserer_ou_trouver(NoeudAVL *racine,const char *identifiant,NoeudAVL **noeud_trouve);

void avl_ecrire_infixe_inverse(FILE *fichier,NoeudAVL *racine,int mode_histo);

void avl_liberer(NoeudAVL *racine);

#endif

