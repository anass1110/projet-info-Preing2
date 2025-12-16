#include <stdio.h>
#include <stdlib.h>


typedef struct usine{
    char* id;
    int vol_max;
    int vol_source;
    int vol_reel;
}Usine;


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


typedef struct Avl {
    int elmt;           
    struct Avl *fg;      
    struct Avl *fd;       
    int equilibre;          
} Avl;


Avl* creerNoeud(Element e) {
    Avl* n = malloc(sizeof(Avl));
    if(n == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }

    n->elmt = e;
    n->fg = NULL;
    n->fd = NULL;
    n->equilibre = 0;  

    return n;}


Avl* rotationGauche(Avl* a) {

    Avl* pivot = a->fd;

    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;

    
    a->fd = pivot->fg;
    pivot->fg = a;
    a->equilibre=eq_a - max1(eq_p, 0) - 1;
    pivot->equilibre= min2( eq_a-2, eq_a+eq_p-2, eq_p-1 );
    return pivot;
}



Avl* rotationDroite(Avl* a) {

    Avl* pivot = a->fg;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;

  
    a->fg = pivot->fd;
    pivot->fd = a;
    a->equilibre=eq_a - min1(eq_p, 0) + 1;
    pivot->equilibre= max2( eq_a+2, eq_a+eq_p+2, eq_p+1 );

    return pivot;




Avl* doubleRotationGauche(Avl* a) {


    a->fd = rotationDroite(a->fd);

  
    return rotationGauche(a);
}



Avl* doubleRotationDroite(Avl* a) {

    
    a->fg = rotationGauche(a->fg);

   
    return rotationDroite(a);
}



Avl* equilibrerAVL(Avl* a) {

  
    if (a->equilibre >= 2) {
        if (a->fd != NULL && a->fd->equilibre >= 0)
            return rotationGauche(a);
        else
            return doubleRotationGauche(a);
    }

   
    else if (a->equilibre <= -2) {
        if (a->fg != NULL && a->fg->equilibre <= 0)
            return rotationDroite(a);
        else
            return doubleRotationDroite(a);
    }

   
    return a;






Avl* insertionAVL(Avl* a, int e, int* h) {
    if (a == NULL) {
        *h = 1;              
        return creerArbre(e);
    }

    if (e < a->elmt) {      
        a->fg = insertionAVL(a->fg, e, h);
        *h = -(*h);          
    }
    else if (e > a->elmt) {   
        a->fd = insertionAVL(a->fd, e, h);
        
    }
    else {
       
        *h = 0;               
        return a;             
    }


    if (*h != 0) {
        a->equilibre = a->equilibre + *h;  
      a = equilibrageAVL(a);

        if (a->equilibre == 0) {
            *h = 0;           
        } else {
            *h = 1;           
        }
    }

    return a;
}



Avl suppMinAVL(Avl a, int *h, Element *pe) {
    Avl tmp;

    if (a->fg == NULL) {  
       
        *pe = a->elem;    
        *h = -1;

        tmp = a;
        a = a->fd;      
        free(tmp);      

        return a;
    } 
    else {
        
        a->fg = suppMinAVL(a->fg, h, pe);
        *h = -(*h);
    }

   
    if (*h != 0) {
        a->equilibre += *h;  

        if (a->equilibre == 0)
            *h = -1;
        else
            *h = 0;
    }

    return a;
}



int existeFilsDroit(Avl a) {
    return (a->fd != NULL);
}

Avl suppressionAVL(Avl a, Element e, int *h) {
    Avl tmp;

    if (a == NULL) {
        *h = 0;
        return a;
    }

    if (e > a->elem) {
        a->fd = suppressionAVL(a->fd, e, h);
    }
    else if (e < a->elem) {
        a->fg = suppressionAVL(a->fg, e, h);
        *h = -(*h);
    }
    else {  
       
        if (existeFilsDroit(a)) {
           
            a->fd = suppMinAVL(a->fd, h, &a->elem);
        }
        else {
   
            tmp = a;
            a = a->fg;
            free(tmp);
            *h = -1;
            return a;
        }
    }

  
    if (*h != 0) {
        a->equilibre += *h;

        if (a->equilibre == 0)
            *h = -1;
        else
            *h = 0;
    }

    return a;

}













#include "AVL.h"
#include <stdlib.h>
#include <string.h>

/* ---------- OUTILS ---------- */

static int hauteur(NoeudAVL *n)
{
    if (n == NULL) return 0;
    return n->hauteur;
}

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




