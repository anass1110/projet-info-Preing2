#!/bin/bash

DEBUT=$(date +%s%3N) \\ pour le temps

















FIN=$(date +%s%3N)    \\ pour le temps
DIFF=$((FIN - DEBUT))
echo "Durée totale : ${DIFF} ms"
exit 0
