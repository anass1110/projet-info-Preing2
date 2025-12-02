
typedef struct Arbre {
    Element elmt;
    struct Arbre* fg;    // fils gauche
    struct Arbre* fd;    // fils droit
} Arbre;



Arbre* creerArbre(Element r) {
    Arbre* noeud = malloc(sizeof(Arbre));   // réserver mémoire

    if (noeud == NULL) {
        return NULL;    // sécurité si allocation échoue
    }

    noeud->elmt = r;     // affecter la valeur
    noeud->fg = NULL;    // pas de fils gauche
    noeud->fd = NULL;    // pas de fils droit

    return noeud;
}

Arbre* creerArbre(Element r) {
    Arbre* noeud = malloc(sizeof(Arbre));   // réserver la mémoire

    if (noeud == NULL) {
        return NULL;    // en cas d'échec d’allocation
    }

    noeud->elmt = r;     // elmt(noeud) ← r
    noeud->fg = NULL;    // fg(noeud) ← NULL
    noeud->fd = NULL;    // fd(noeud) ← NULL

    return noeud;        // retourner noeud
}


bool estFeuille(Arbre* a) {
    if (a == NULL) {
        // ERREUR() — ici on peut afficher un message ou gérer autrement
        fprintf(stderr, "Erreur : pointeur NULL dans estFeuille.\n");
        return false; 
    }

    if (a->fg == NULL && a->fd == NULL) {
        return true;    // c’est une feuille
    } else {
        return false;   // pas une feuille
    }
}

int existeFilsDroit(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans existeFilsDroit.\n");
        return 0;   // FAUX
    }

    if (a->fd == NULL) {
        return 0;   // pas de fils droit
    } else {
        return 1;   // fils droit existant
    }
}

int existeFilsGauche(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans existeFilsGauche.\n");
        return 0;   // FAUX
    }

    if (a->fg == NULL) {
        return 0;   // pas de fils gauche
    } else {
        return 1;   // fils gauche existant
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

    a->elmt = r;   // elmt(a) ← r
    return a;      // retourne le pointeur (comme dans ton pseudo-code)
}
int ajouterFilsGauche(Arbre* a, Element r) {
    if (a == NULL) {
        return 0;   // FAUX
    }

    if (!existeFilsGauche(a)) {
        a->fg = creerArbre(r);
        return 1;   // VRAI
    } else {
        return 0;   // FAUX
    }
}
int ajouterFilsDroit(Arbre* a, Element r) {
    if (a == NULL) {
        return 0;   // FAUX
    }

    if (!existeFilsDroit(a)) {
        a->fd = creerArbre(r);
        return 1;   // VRAI
    } else {
        return 0;   // FAUX
    }
}
void supprimerFilsDroit(Arbre* a) {
    if (a == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans supprimerFilsDroit.\n");
        return;
    }

    if (existeFilsDroit(a)) {
        Arbre* droit = a->fd;

        // supprimer récursivement les sous-arbres
        if (existeFilsGauche(droit)) {
            supprimerFilsGauche(droit);
        }
        if (existeFilsDroit(droit)) {
            supprimerFilsDroit(droit);
        }

        // libérer le noeud fils droit
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

        // supprimer récursivement les sous-arbres
        if (existeFilsGauche(gauche)) {
            supprimerFilsGauche(gauche);
        }
        if (existeFilsDroit(gauche)) {
            supprimerFilsDroit(gauche);
        }

        // libérer le noeud fils gauche
        free(a->fg);
        a->fg = NULL;
    }
}

void parcoursPrefixe(Arbre* a) {
    if (a != NULL) {
        traiter(a->elmt);      // traiter(elmt(a))
        parcoursPrefixe(a->fg); // parcours du sous-arbre gauche
        parcoursPrefixe(a->fd); // parcours du sous-arbre droit
    }
}

void parcoursInfixe(Arbre* a) {
    if (a != NULL) {
        parcoursInfixe(a->fg);   // parcours du sous-arbre gauche
        traiter(a->elmt);        // traiter(elmt(a))
        parcoursInfixe(a->fd);   // parcours du sous-arbre droit
    }
}
