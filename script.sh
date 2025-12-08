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














FIN=$(date +%s%3N)    // pour le temps
DIFF=$((FIN - DEBUT))
echo "Durée totale : ${DIFF} ms"
exit 0
