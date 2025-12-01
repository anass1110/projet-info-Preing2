#include <stdio.h>
#include <stdlib.h>

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

// Structure d'un noeud AVL
typedef struct Arbre {
    int elmt;            // contenu du nœud
    struct Arbre *fg;        // fils gauche
    struct Arbre *fd;        // fils droit
    int equilibre;           // facteur d’équilibre 
} Arbre;


Arbre* creerNoeud(Element e) {
    Arbre* n = (Arbre*)malloc(sizeof(Arbre));
    if(n == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }

    n->elmt = e;
    n->fg = NULL;
    n->fd = NULL;
    n->equilibre = 0;   // au début, un nouveau nœud est équilibré

    return n;}


Arbre* rotationGauche(Arbre* a) {

    Arbre* pivot = a->fd;

    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;

    // Rotation
    a->fd = pivot->fg;
    pivot->fg = a;
    a->equilibre=eq_a - max1(eq_p, 0) - 1;
    pivot->equilibre= min2( eq_a-2, eq_a+eq_p-2, eq_p-1 );
    return pivot;
}



Arbre* rotationDroite(Arbre* a) {

    Arbre* pivot = a->fg;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;

    // Rotation
    a->fg = pivot->fd;
    pivot->fd = a;
    a->equilibre=eq_a - min1(eq_p, 0) + 1;
    pivot->equilibre= max2( eq_a+2, eq_a+eq_p+2, eq_p+1 );

    return pivot;




Arbre* doubleRotationGauche(Arbre* a) {

    // Étape 1 : rotation droite sur le fils droit
    a->fd = rotationDroite(a->fd);

    // Étape 2 : rotation gauche sur a
    return rotationGauche(a);
}



Arbre* doubleRotationDroite(Arbre* a) {

    // Étape 1 : rotation gauche sur le fils gauche
    a->fg = rotationGauche(a->fg);

    // Étape 2 : rotation droite sur a
    return rotationDroite(a);
}



Arbre* equilibrerAVL(Arbre* a) {

    // Sous-arbre droit trop profond
    if (a->equilibre >= 2) {
        if (a->fd != NULL && a->fd->equilibre >= 0)
            return rotationGauche(a);
        else
            return doubleRotationGauche(a);
    }

    // Sous-arbre gauche trop profond
    else if (a->equilibre <= -2) {
        if (a->fg != NULL && a->fg->equilibre <= 0)
            return rotationDroite(a);
        else
            return doubleRotationDroite(a);
    }

    // Sinon, aucun déséquilibre
    return a;






Arbre* insertionAVL(Arbre* a, int e, int* h) {
    if (a == NULL) {
        *h = 1;               // on ajoute un élément → l’équilibre change
        return creerArbre(e);
    }

    if (e < a->elmt) {        // insertion à gauche
        a->fg = insertionAVL(a->fg, e, h);
        *h = -(*h);           // différence de -1 si on insère à gauche
    }
    else if (e > a->elmt) {   // insertion à droite
        a->fd = insertionAVL(a->fd, e, h);
        // *h reste positif (rien à faire)
    }
    else {
        // e == a->elmt → élément déjà présent
        *h = 0;               // pas de changement d’équilibre
        return a;             // aucune modification
    }

    // Si la hauteur a changé, mettre à jour l’équilibre
    if (*h != 0) {
        a->equilibre = a->equilibre + *h;  // maj du facteur d’équilibre
      a = equilibrageAVL(a);

        if (a->equilibre == 0) {
            *h = 0;            // l'arbre redevient équilibré
        } else {
            *h = 1;            // la hauteur de ce nœud a changé
        }
    }

    return a;
}



Arbre suppMinAVL(Arbre a, int *h, Element *pe) {
    Arbre tmp;

    if (a->fg == NULL) {  
        // On a trouvé la plus petite valeur
        *pe = a->elem;    
        *h = -1;

        tmp = a;
        a = a->fd;      // Le remplaçant est le fils droit
        free(tmp);      // Libération du nœud supprimé

        return a;
    } 
    else {
        // Recherche récursive du minimum dans fg
        a->fg = suppMinAVL(a->fg, h, pe);
        *h = -(*h);
    }

    // Si le sous-arbre a changé de hauteur...
    if (*h != 0) {
        a->equilibre += *h;  // mise à jour du facteur d’équilibre

        if (a->equilibre == 0)
            *h = -1;
        else
            *h = 0;
    }

    return a;
}



int existeFilsDroit(Arbre a) {
    return (a->fd != NULL);
}

Arbre suppressionAVL(Arbre a, Element e, int *h) {
    Arbre tmp;

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
        // e == a->elem → élément trouvé
        if (existeFilsDroit(a)) {
            // Remplacer par le minimum du sous-arbre droit
            a->fd = suppMinAVL(a->fd, h, &a->elem);
        }
        else {
            // Pas de fils droit → remonter le fils gauche
            tmp = a;
            a = a->fg;
            free(tmp);
            *h = -1;
            return a;
        }
    }

    // Mise à jour des hauteurs et équilibrage local
    if (*h != 0) {
        a->equilibre += *h;

        if (a->equilibre == 0)
            *h = -1;
        else
            *h = 0;
    }

    return a;
}


