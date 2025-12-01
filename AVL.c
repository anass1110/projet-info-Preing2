#include <stdio.h>
#include <stdlib.h>


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



Arbre* equilibrerAVL(Arbre* a) {
    if (a == NULL) return a;

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
