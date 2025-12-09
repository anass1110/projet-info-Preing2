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

if [ ! -f "$chemin" ]; then
  erreur "Fichier d'entrée '$chemin' introuvable."
fi






FIN=$(date +%s%3N)    // pour le temps
DIFF=$((FIN - DEBUT))
echo "Durée totale : ${DIFF} ms"
exit 0
