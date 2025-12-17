
typedef struct Arbre {
    Element elmt;
    struct Arbre* fg;    
    struct Arbre* fd;    
} Arbre;



Arbre* creerArbre(Element r) {
    Arbre* noeud = malloc(sizeof(Arbre));   

    if (noeud == NULL) {
        return NULL;    
    }

    noeud->elmt = r;     
    noeud->fg = NULL;    
    noeud->fd = NULL;    

    return noeud;
}

Arbre* creerArbre(Element r) {
    Arbre* noeud = malloc(sizeof(Arbre));  

    if (noeud == NULL) {
        return NULL;    
    }

    noeud->elmt = r;     
    noeud->fg = NULL;    
    noeud->fd = NULL;    

    return noeud;       
}


bool estFeuille(Arbre* a) {
    if (a == NULL) {
       
        fprintf(stderr, "Erreur : pointeur NULL dans estFeuille.\n");
        return false; 
    }

    if (a->fg == NULL && a->fd == NULL) {
        return true;    
    } else {
        return false;  
    }
}

int existeFilsDroit(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans existeFilsDroit.\n");
        return 0;   
    }

    if (a->fd == NULL) {
        return 0;   
    } else {
        return 1;   
    }
}

int existeFilsGauche(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans existeFilsGauche.\n");
        return 0;   
    }

    if (a->fg == NULL) {
        return 0;   
    } else {
        return 1;   
    }
}
Arbre* filsDroit(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans filsDroit.\n");
        return NULL;
    }

    if (existeFilsDroit(a)) {
        return a->fd;
    } else {
        return NULL;
    }
}
Arbre* filsGauche(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans filsGauche.\n");
        return NULL;
    }

    if (existeFilsGauche(a)) {
        return a->fg;
    } else {
        return NULL;
    }
}


Arbre* modifierElement(Arbre* a, Element r) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans modifierElement.\n");
        return NULL;
    }

    a->elmt = r;  
    return a;      
}
int ajouterFilsGauche(Arbre* a, Element r) {
    if (a == NULL) {
        return 0;   
    }

    if (!existeFilsGauche(a)) {
        a->fg = creerArbre(r);
        return 1; 
    } else {
        return 0;   
    }
}
int ajouterFilsDroit(Arbre* a, Element r) {
    if (a == NULL) {
        return 0;   
    }

    if (!existeFilsDroit(a)) {
        a->fd = creerArbre(r);
        return 1;   
    } else {
        return 0;   
    }
}
void supprimerFilsDroit(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans supprimerFilsDroit.\n");
        return;
    }

    if (existeFilsDroit(a)) {
        Arbre* droit = a->fd;

    
        if (existeFilsGauche(droit)) {
            supprimerFilsGauche(droit);
        }
        if (existeFilsDroit(droit)) {
            supprimerFilsDroit(droit);
        }

       
        free(a->fd);
        a->fd = NULL;
    }
}
void supprimerFilsGauche(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans supprimerFilsGauche.\n");
        return;
    }

    if (existeFilsGauche(a)) {
        Arbre* gauche = a->fg;

      
        if (existeFilsGauche(gauche)) {
            supprimerFilsGauche(gauche);
        }
        if (existeFilsDroit(gauche)) {
            supprimerFilsDroit(gauche);
        }

       
        free(a->fg);
        a->fg = NULL;
    }
}

void parcoursPrefixe(Arbre* a) {
    if (a != NULL) {
        traiter(a->elmt);     
        parcoursPrefixe(a->fg); 
        parcoursPrefixe(a->fd); 
    }
}

void parcoursInfixe(Arbre* a) {
    if (a != NULL) {
        parcoursInfixe(a->fg);  
        traiter(a->elmt);        
        parcoursInfixe(a->fd);   
    }





#include "arbre.h"

#include <stdlib.h>
#include <string.h>

NoeudReseau* creer_noeud(const char *identifiant)
{
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

/*
  Retour :
    1  -> enfant ajouté
    0  -> enfant déjà présent (doublon)
   -1  -> erreur (malloc ou parent/enfant NULL)
*/
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
            return 0; /* déjà présent */
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




    
}
