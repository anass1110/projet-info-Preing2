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

if [ "$mode" = "histo" ]; then

    if [ $# -ne 3 ]; then
        erreur "histo nécessite max ou src | real."
    fi
case "$option" in
        max)
            echo "identifier / max volume (k.m3.year-1)"
            while IFS=';' read -r id max src real; do
                echo "$id / $max"
            done < "$chemin"
            ;;
        src)
            echo "identifier / source volume (k.m3.year-1)"
            while IFS=';' read -r id max src real; do
                echo "$id / $src"
            done < "$chemin"
            ;;
        real)
            echo "identifier / real volume (k.m3.year-1)"
            while IFS=';' read -r id max src real; do
                echo "$id / $real"
            done < "$chemin"
            ;;
        *)
            erreur "Argument histo invalide."
            ;;
    esac

    exit 0
fi

if [ "$MODE" = "leaks" ]; then

    if [ $# -ne 3 ]; then
        erreur "leaks nécessite un identifiant."
    fi

    f=0
    som=0

    echo "identifier / Leak volume (M.m3.year-1)"

    while IFS=';' read -r id max src real; do
        if [ "$id" = "$option" ]; then
            leak=$((src - real))
            som=$((som + leak))
            f=1
        fi
    done < "$chemin"

    if [ $f -eq 1 ]; then
        echo "$option / $som"
    else
        echo "$option / -1"
    fi

    exit 0
fi



FIN=$(date +%s%3N)    // pour le temps
DIFF=$((FIN - DEBUT))
echo "Durée totale : ${DIFF} ms"
exit 0
