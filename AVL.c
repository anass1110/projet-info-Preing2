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


