

#include "AVL.h"
#include <stdlib.h>
#include <string.h>

/* ---------- OUTILS ---------- */

static int hauteur(NoeudAVL *n)
{
    if (n == NULL) return 0;
    return n->hauteur;
}



int max2(int a,int b,int c){
    int d;
    d=max1(a,b);
    return max1(d,c);        
}

int max1(int a,int b){
    if(a>=b){
        return a;
    }
    else{
        return b;
        }
}

int min1(int a,int b){
    if(a<=b){
        return a;
    }
    else{
        return b;
        }
}

int min2(int a,int b,int c){
    int d;
    d=min1(a,b);
    return min1(d,c);
}


NoeudAVL* rotationGauche(NoeudAVL* a) {

    NoeudAVL* pivot = a->droit;

    int eq_a = a->hauteur;
    int eq_p = pivot->hauteur;

    
    a->droit = pivot->gauche;
    pivot->gauche = a;
    a->hauteur=eq_a - max1(eq_p, 0) - 1;
    pivot->hauteur= min2( eq_a-2, eq_a+eq_p-2, eq_p-1 );
    return pivot;
}



NoeudAVL* rotationDroite(NoeudAVL* a) {

    NoeudAVL* pivot = a->gauche;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;

  
    a->gauche = pivot->droite;
    pivot->droit = a;
    a->hauteur=eq_a - min1(eq_p, 0) + 1;
    pivot->hauteur= max2( eq_a+2, eq_a+eq_p+2, eq_p+1 );

    return pivot;}












static int max(int a, int b)
{
    return (a > b) ? a : b;
}

static NoeudAVL *nouveau_noeud(const char *id)
{
    NoeudAVL *n = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if (n == NULL) return NULL;

    strncpy(n->identifiant, id, 255);
    n->identifiant[255] = '\0';

    n->v_max = 0.0;
    n->v_src = 0.0;
    n->v_real = 0.0;

    n->hauteur = 1;
    n->gauche = NULL;
    n->droit = NULL;

    return n;
}

static NoeudAVL *rotation_droite(NoeudAVL *y)
{
    NoeudAVL *x = y->gauche;
    NoeudAVL *T2 = x->droit;

    x->droit = y;
    y->gauche = T2;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droit)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droit)) + 1;

    return x;
}

static NoeudAVL *rotation_gauche(NoeudAVL *x)
{
    NoeudAVL *y = x->droit;
    NoeudAVL *T2 = y->gauche;

    y->gauche = x;
    x->droit = T2;

    x->hauteur = max(hauteur(x->gauche), hauteur(x->droit)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droit)) + 1;

    return y;
}

static int facteur_equilibre(NoeudAVL *n)
{
    if (n == NULL) return 0;
    return hauteur(n->gauche) - hauteur(n->droit);
}

/* ---------- INSERTION ---------- */

NoeudAVL *avl_inserer_ou_trouver(NoeudAVL *racine,
                                const char *identifiant,
                                NoeudAVL **noeud_trouve)
{
    if (racine == NULL) {
        *noeud_trouve = nouveau_noeud(identifiant);
        return *noeud_trouve;
    }

    int cmp = strcmp(identifiant, racine->identifiant);

    if (cmp == 0) {
        *noeud_trouve = racine;
        return racine;
    }
    else if (cmp < 0) {
        racine->gauche = avl_inserer_ou_trouver(racine->gauche,
                                                 identifiant,
                                                 noeud_trouve);
    }
    else {
        racine->droit = avl_inserer_ou_trouver(racine->droit,
                                               identifiant,
                                               noeud_trouve);
    }

    racine->hauteur = 1 + max(hauteur(racine->gauche),
                              hauteur(racine->droit));

    int eq = facteur_equilibre(racine);

    /* Cas AVL classiques */
    if (eq > 1 && strcmp(identifiant, racine->gauche->identifiant) < 0)
        return rotation_droite(racine);

    if (eq < -1 && strcmp(identifiant, racine->droit->identifiant) > 0)
        return rotation_gauche(racine);

    if (eq > 1 && strcmp(identifiant, racine->gauche->identifiant) > 0) {
        racine->gauche = rotation_gauche(racine->gauche);
        return rotation_droite(racine);
    }

    if (eq < -1 && strcmp(identifiant, racine->droit->identifiant) < 0) {
        racine->droit = rotation_droite(racine->droit);
        return rotation_gauche(racine);
    }

    return racine;
}

/* ---------- ECRITURE HISTO ---------- */
/* TRI ALPHABETIQUE INVERSE : DROITE -> RACINE -> GAUCHE */

void avl_ecrire_infixe_inverse(FILE *fichier,
                               NoeudAVL *racine,
                               int mode_histo)
{
    if (racine == NULL) return;

    avl_ecrire_infixe_inverse(fichier, racine->droit, mode_histo);

    if (mode_histo == 0) {
        fprintf(fichier, "%s;%.6f\n",
                racine->identifiant,
                racine->v_max);
    }
    else if (mode_histo == 1) {
        fprintf(fichier, "%s;%.6f\n",
                racine->identifiant,
                racine->v_src);
    }
    else if (mode_histo == 2) {
        fprintf(fichier, "%s;%.6f\n",
                racine->identifiant,
                racine->v_real);
    }
    else {
        fprintf(fichier, "%s;%.6f;%.6f;%.6f\n",
                racine->identifiant,
                racine->v_max,
                racine->v_src,
                racine->v_real);
    }

    avl_ecrire_infixe_inverse(fichier, racine->gauche, mode_histo);
}

/* ---------- LIBERATION ---------- */

void avl_liberer(NoeudAVL *racine)
{
    if (racine == NULL) return;

    avl_liberer(racine->gauche);
    avl_liberer(racine->droit);
    free(racine);
}




