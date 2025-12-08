#!/bin/bash

START_MS=$(date +%s%3N)








END_MS=$(date +%s%3N)
DIFF_MS=$((END_MS - START_MS))
echo "Durée totale : ${DIFF_MS} ms"
exit 0
