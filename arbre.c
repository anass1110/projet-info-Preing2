#include "arbre.h"
#include <stdlib.h>
#include <string.h>

NoeudReseau* creer_noeud(const char *identifiant)
{
    if (identifiant == NULL) return NULL;
    NoeudReseau *n;

    n = (NoeudReseau*)malloc(sizeof(NoeudReseau));
    if (n == NULL) {
        return NULL;
    }

    strncpy(n->identifiant, identifiant, 255);
    n->identifiant[255] = '\0';

    n->enfants = NULL;

    return n;
}

/* Return 1 si enfant ajouté, 0 si doublon et -1 si erreurv*/
int ajouter_enfant(NoeudReseau *parent, NoeudReseau *enfant, double fuite)
{
    Enfant *courant;
    Enfant *nouveau;

    if (parent == NULL || enfant == NULL) {
        return -1;
    }

    /* Vérification des doublons */
    courant = parent->enfants;
    while (courant != NULL) {
        if (courant->noeud == enfant) {
            return 0; 
        }
        courant = courant->suiv;
    }

    /* Ajout en tête */
    nouveau = (Enfant*)malloc(sizeof(Enfant));
    if (nouveau == NULL) {
        return -1;
    }

    nouveau->noeud = enfant;
    nouveau->fuite = fuite;
    nouveau->suiv = parent->enfants;
    parent->enfants = nouveau;

    return 1;
}

int compter_enfants(NoeudReseau *noeud)
{
    int nb;
    Enfant *courant;

    if (noeud == NULL) {
        return 0;
    }

    nb = 0;
    courant = noeud->enfants;
    while (courant != NULL) {
        nb++;
        courant = courant->suiv;
    }

    return nb;
}

void liberer_noeud(NoeudReseau *noeud)
{
    Enfant *courant;
    Enfant *tmp;

    if (noeud == NULL) {
        return;
    }

    courant = noeud->enfants;
    while (courant != NULL) {
        tmp = courant->suiv;
        free(courant);
        courant = tmp;
    }

    noeud->enfants = NULL;
    free(noeud);
}


