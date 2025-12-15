#!/bin/bash

DEBUT=$(date +%s%3N) // pour le temps

// truc pour les erreur et afficher le temps meme avec les erreurs
erreur() {
  echo "ERREUR: $*" >&2
  FIN=$(date +%s%3N)
  DIFF=$((FIN - DEBUT))
  echo "Durée totale : ${DIFF} ms"
  exit 1
}


if [ $# -lt 3 ]; then
    erreur "Commande incomplète."
fi

if [ $# -gt 3 ]; then
    erreur "Trop d'arguments."
fi

chemin="$1"
mode="$2"
option="$3"

EXEC="./water_analysis"

if [ ! -x "$EXEC" ]; then
    echo "Compilation du programme C..."
    gcc -Wall -Wextra -o water_analysis water_analysis.c
    if [ $? -ne 0 ]; then
        erreur " compilation du programme C échouée."
        exit 1
    fi
fi

if [ ! -f "$chemin" ]; then
  erreur "Fichier d'entrée '$chemin' introuvable."
fi


if [ "$mode" != "histo" ] && [ "$mode" != "leaks" ]; then
    erreur "Mode invalide : doit être 'histo' ou 'leaks'."
fi


if [ "$mode" = "histo" ]; then

    if [ $# -ne 3 ]; then
        erreur "histo nécessite max ou src ou real."
    fi
 if [[ "$option" != "max" && "$option" != "src" && "$option" != "real" ]]; then
            erreur " option histo invalide."
            exit 1
        fi
"$EXEC" histo "$option" "$chemin"
fi


if [ "$mode" = "leaks" ]; then

if [ "$#" -ne 3 ]; then
            erreur "leaks nécessite un identifiant d’usine."
            exit 1
        fi

        "$EXEC" leaks "$option" "$chemin"
        fi



FIN=$(date +%s%3N)   
DIFF=$((FIN - DEBUT))
echo "Durée totale : ${DIFF} ms"
exit 0
