# C-WildWater — Projet C & Shell (Pré-ING2)

Projet de traitement d’un fichier CSV volumineux pour :
- générer des histogrammes de volumes par usine (`histo`)
- calculer les fuites pour une usine (`leaks`)

Le script `script.sh` est le point d’entrée : il compile automatiquement le programme C (via `make` si `./wildwater` n’existe pas), exécute le calcul, génère les fichiers `.dat` et les images `.png` (avec gnuplot), puis affiche la durée totale d’exécution.


## 1) Prérequis

### Outils
- `gcc` (C11) + `make`
- `gnuplot` (pour générer les images)

## 2) Compilation

```bash
make
```

Nettoyage compilation :
```bash
make clean
```

Nettoyage complet (binaire + fichiers générés `.dat` / `.png`) :
```bash
make distclean
```

---

## 3) Utilisation

### A) Via le script (recommandé)

```bash
./script.sh <fichier.csv> histo max|src|real|all
./script.sh <fichier.csv> leaks "<ID_usine>"
```

Exemples :
```bash
./script.sh c-wildwater_v0.dat histo max
./script.sh c-wildwater_v0.dat histo all
./script.sh c-wildwater_v0.dat leaks "NM000000T"
```

### B) Exécution directe du programme C

```bash
./wildwater histo <csv> <max|src|real|all> <fichier_sortie>
./wildwater leaks <csv> <identifiant_usine> <fichier_sortie>
```

Exemple :
```bash
./wildwater histo c-wildwater_v0.dat all vol_all.dat
./wildwater leaks c-wildwater_v0.dat NM000000T leaks.dat
```

---

## 4) Histogrammes (histo)

Modes :
- `max` : capacité maximale (volume max)
- `src` : volume capté (source)
- `real` : volume réel (après pertes)
- `all` : histogramme cumulé (bonus)

Fichiers générés par le script :
- `vol_<mode>.dat` : toutes les usines
- `vol_<mode>_low.dat` : 50 plus petites usines (référence = capacité max)
- `vol_<mode>_high.dat` : 10 plus grandes usines (référence = capacité max)
- `vol_<mode>_low.png` / `vol_<mode>_high.png` : images


## 5) Leaks

Commande :
```bash
./script.sh <fichier.csv> leaks "<ID_usine>"
```

Sortie :
- `leaks.dat`


---

## 6) Format des fichiers générés

### `vol_<mode>.dat` (séparateur `;`)

- `max` : `identifier;max volume (M.m3.year-1)`
- `src` : `identifier;source volume (M.m3.year-1)`
- `real` : `identifier;real volume (M.m3.year-1)`
- `all` : `identifier;max volume (M.m3.year-1);source volume (M.m3.year-1);real volume (M.m3.year-1)`

Les fichiers `*_low.dat` et `*_high.dat` sont des sous-ensembles du fichier correspondant.

### `leaks.dat`

`identifier;Leak volume (M.m3.year-1);worst_from;worst_to;worst_loss (M.m3.year-1)`

- `Leak volume` : volume total perdu (valeur absolue)
- `worst_from` / `worst_to` : amont / aval du tronçon qui perd le plus (valeur absolue)
- `worst_loss` : perte maximale (M.m3.year-1) sur ce tronçon

---

## 7) Organisation du code

- `main.c` : analyse des arguments, dispatch des commandes
- `lecture_fichier.c/.h` : lecture robuste du CSV (ligne par ligne)
- `usines_donnes.c/.h` : calculs `histo` (AVL des usines)
- `pertes.c/.h` : calculs `leaks` (réseau aval + recherche du pire tronçon)
- `AVL.c/.h` : arbre AVL (index par identifiant)
- `arbre.c/.h` : structure réseau (nœuds et enfants)
- `Utilitaire.c/.h` : utilitaires (messages d’erreur, helpers)
- `script.sh` : compile + exécute + gnuplot
- `Makefile` : compilation + `clean` / `distclean`

---


- Abbadi Anass
- Ferrand Hugo 
- Casteret Aymeric
6

