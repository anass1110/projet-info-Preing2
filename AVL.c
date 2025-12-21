#include "AVL.h"
#include <stdlib.h>
#include <string.h>

int hauteur(NoeudAVL *n) {
    return (n == NULL) ? 0 : n->hauteur;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

/* Création d'un noeud initialisé */
NoeudAVL *nouveau_noeud(const char *id) {
    if (id == NULL) return NULL;
    NoeudAVL *n = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if (n == NULL) return NULL;

    strncpy(n->identifiant, id, 255);
    n->identifiant[255] = '\0';
    n->v_max = 0.0;
    n->v_src = 0.0;
    n->v_real = 0.0;
    n->ptr = NULL;  
    n->hauteur = 1;
    n->gauche = NULL;
    n->droit = NULL;
    return n;
}

/*fonction de rotation et d'AVL */
NoeudAVL *rotation_droite(NoeudAVL *y) {
    NoeudAVL *x = y->gauche;
    NoeudAVL *T2 = x->droit;
    x->droit = y;
    y->gauche = T2;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droit)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droit)) + 1;
    return x;
}

NoeudAVL *rotation_gauche(NoeudAVL *x) {
    NoeudAVL *y = x->droit;
    NoeudAVL *T2 = y->gauche;
    y->gauche = x;
    x->droit = T2;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droit)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droit)) + 1;
    return y;
}

int facteur_equilibre(NoeudAVL *n) {
    return (n == NULL) ? 0 : hauteur(n->gauche) - hauteur(n->droit);
}

NoeudAVL *equilibrer(NoeudAVL *n) {
    int eq = facteur_equilibre(n);
    if (eq > 1 && facteur_equilibre(n->gauche) >= 0) return rotation_droite(n);
    if (eq > 1 && facteur_equilibre(n->gauche) < 0) {
        n->gauche = rotation_gauche(n->gauche);
        return rotation_droite(n);
    }
    if (eq < -1 && facteur_equilibre(n->droit) <= 0) return rotation_gauche(n);
    if (eq < -1 && facteur_equilibre(n->droit) > 0) {
        n->droit = rotation_droite(n->droit);
        return rotation_gauche(n);
    }
    return n;
}

/* Fonction de Recherche */
NoeudAVL *avl_rechercher(NoeudAVL *racine, const char *identifiant) {
    if (identifiant == NULL) return NULL;
    if (racine == NULL) return NULL;
    int cmp = strcmp(identifiant, racine->identifiant);
    if (cmp == 0) return racine;
    if (cmp < 0) return avl_rechercher(racine->gauche, identifiant);
    return avl_rechercher(racine->droit, identifiant);
}

/* Insère un identifiant dans l’AVL s’il n’existe pas encore ou le retrouve */

NoeudAVL *avl_inserer_ou_trouver(NoeudAVL *racine, const char *identifiant, NoeudAVL **noeud_trouve) {
    if (racine == NULL) {
        *noeud_trouve = nouveau_noeud(identifiant);
        if (*noeud_trouve == NULL) {
        return NULL;
        }
    return *noeud_trouve;

    }
    int cmp = strcmp(identifiant, racine->identifiant);
    if (cmp == 0) {
        *noeud_trouve = racine;
        return racine;
    } else if (cmp < 0) {
        racine->gauche = avl_inserer_ou_trouver(racine->gauche, identifiant, noeud_trouve);
    } else {
        racine->droit = avl_inserer_ou_trouver(racine->droit, identifiant, noeud_trouve);
    }
    racine->hauteur = 1 + max(hauteur(racine->gauche), hauteur(racine->droit));
    return equilibrer(racine);
}

/* Insère ou retrouve un identifiant dans l'AVL et l’associe à un pointeur */

NoeudAVL *avl_inserer_ptr(NoeudAVL *racine, const char *identifiant, void *ptr, NoeudAVL **noeud_trouve) {
    if (racine == NULL) {
        *noeud_trouve = nouveau_noeud(identifiant);
        if (*noeud_trouve == NULL) {
        return NULL;
        }
    (*noeud_trouve)->ptr = ptr;
    return *noeud_trouve;

    }
    int cmp = strcmp(identifiant, racine->identifiant);
    if (cmp == 0) {
        *noeud_trouve = racine;
        if (racine->ptr == NULL) {
            racine->ptr = ptr;
        }
        return racine;
    } else if (cmp < 0) {
        racine->gauche = avl_inserer_ptr(racine->gauche, identifiant, ptr, noeud_trouve);
    } else {
        racine->droit = avl_inserer_ptr(racine->droit, identifiant, ptr, noeud_trouve);
    }
    racine->hauteur = 1 + max(hauteur(racine->gauche), hauteur(racine->droit));
    return equilibrer(racine);
}

/* sortie et libération mémoire*/
void avl_ecrire_infixe_inverse(FILE *fichier, NoeudAVL *racine, int mode_histo) {
    if (racine == NULL) return;
    avl_ecrire_infixe_inverse(fichier, racine->droit, mode_histo);
    
    if (mode_histo == 0) fprintf(fichier, "%s;%.6f\n", racine->identifiant, racine->v_max);
    else if (mode_histo == 1) fprintf(fichier, "%s;%.6f\n", racine->identifiant, racine->v_src);
    else if (mode_histo == 2) fprintf(fichier, "%s;%.6f\n", racine->identifiant, racine->v_real);
    else fprintf(fichier, "%s;%.6f;%.6f;%.6f\n", racine->identifiant, racine->v_max, racine->v_src, racine->v_real);

    avl_ecrire_infixe_inverse(fichier, racine->gauche, mode_histo);
}

void avl_liberer(NoeudAVL *racine) {
    if (racine == NULL) return;
    avl_liberer(racine->gauche);
    avl_liberer(racine->droit);
    free(racine);
}




