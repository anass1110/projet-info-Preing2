
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
}
