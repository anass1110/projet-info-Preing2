#ifndef PERTES_H
#define PERTES_H

typedef struct {
    char amont[256];
    char aval[256];
    double perte_max;
} TronconMax;

/* Calcule les pertes d'eau en aval d'une usine */
int calculer_leaks(const char *csv_path,const char *identifiant_usine,const char *chemin_sortie);

#endif 

