#ifndef ARBRE_H
#define ARBRE_H

typedef struct NoeudReseau NoeudReseau;

typedef struct Enfant {
    NoeudReseau *noeud;
    double fuite;            /* en pourcentage */
    struct Enfant *suiv;
} Enfant;

struct NoeudReseau {
    char identifiant[256];
    Enfant *enfants;
};

NoeudReseau* creer_noeud(const char *identifiant);
int ajouter_enfant(NoeudReseau *parent, NoeudReseau *enfant, double fuite);
int compter_enfants(NoeudReseau *noeud);
void liberer_noeud(NoeudReseau *noeud);

#endif
