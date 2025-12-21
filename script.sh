#!/bin/bash

debut=$(date +%s)

afficher_fin() {
  fin=$(date +%s)
  duree=$(( (fin - debut) * 1000 ))
  echo "Duree (ms) : $duree"
}

usage() {
  echo "Usage :"
  echo "  ./script.sh <csv> histo max|src|real|all"
  echo "  ./script.sh <csv> leaks \"<identifiant usine>\""
}


if [ $# -ne 3 ]; then
  echo "Erreur : mauvais nombre d'arguments."          # vérification des arguments 
  usage
  afficher_fin
  exit 1
fi

CSV="$1"
CMD="$2"
ARG="$3"

if [ ! -f "$CSV" ]; then
  echo "Erreur : fichier CSV introuvable."
  afficher_fin
  exit 1
fi

# Compilation via make 
if [ ! -x "./wildwater" ]; then
  make
if [ $? -ne 0 ]; then
  echo "Erreur : compilation impossible."
  afficher_fin
  exit 1

   fi
fi



# HISTO 
if [ "$CMD" = "histo" ]; then

  if [ "$ARG" != "max" ] && [ "$ARG" != "src" ] && [ "$ARG" != "real" ] && [ "$ARG" != "all" ]; then
    echo "Erreur : mode histo invalide."
    usage
    afficher_fin
    exit 1
  fi

  OUT="vol_${ARG}.dat"
  ./wildwater histo "$CSV" "$ARG" "$OUT"
if [ $? -ne 0 ]; then
  afficher_fin
  exit 1
fi



  LOW="${OUT%.dat}_low.dat"
  HIGH="${OUT%.dat}_high.dat"
  PNG_LOW="${OUT%.dat}_low.png"
  PNG_HIGH="${OUT%.dat}_high.png"

 
  # low / high
 
  # sélection des IDs selon la capacité max (référence) 
SEL_FILE="$OUT"
TMP_MAX=""

# Pour src/real : on calcule un vol_max juste pour choisir les IDs
if [ "$ARG" = "src" ] || [ "$ARG" = "real" ]; then
  TMP_MAX="vol__select_max.dat"
  ./wildwater histo "$CSV" "max" "$TMP_MAX"
  if [ $? -ne 0 ]; then
    afficher_fin
    exit 1
  fi
  SEL_FILE="$TMP_MAX"
fi

IDS_LOW="$(mktemp)"
IDS_HIGH="$(mktemp)"

tail -n +2 "$SEL_FILE" | sort -t';' -k2,2n | head -n 50 | cut -d';' -f1 > "$IDS_LOW"
tail -n +2 "$SEL_FILE" | sort -t';' -k2,2n | tail -n 10 | cut -d';' -f1 > "$IDS_HIGH"

# On extrait ensuite ces IDs depuis le fichier OUT (src/real/max selon le mode)
head -n 1 "$OUT" > "$LOW"
awk -F';' 'NR==FNR{want[$1]=++n; order[n]=$1; next}
          FNR>1 && ($1 in want){row[$1]=$0}
          END{for(i=1;i<=n;i++) if(order[i] in row) print row[order[i]]}' "$IDS_LOW" "$OUT" >> "$LOW"

head -n 1 "$OUT" > "$HIGH"
awk -F';' 'NR==FNR{want[$1]=++n; order[n]=$1; next}
          FNR>1 && ($1 in want){row[$1]=$0}
          END{for(i=1;i<=n;i++) if(order[i] in row) print row[order[i]]}' "$IDS_HIGH" "$OUT" >> "$HIGH"

rm -f "$IDS_LOW" "$IDS_HIGH"
[ -n "$TMP_MAX" ] && rm -f "$TMP_MAX"


  if command -v gnuplot >/dev/null 2>&1; then

    # BONUS ALL 
    if [ "$ARG" = "all" ]; then

      gnuplot -e "
      set terminal png;
      set datafile separator ';';
      set xlabel 'Usine';
      set ylabel 'Volume (M.m3.year-1)';
      set style data histograms;
      set style histogram rowstacked;
      set style fill solid 1.0 border -1;
      set xtics rotate by -90 font \",8\";
      set boxwidth 0.75 relative;
      set style line 1 lc rgb '#00AA00';
      set style line 2 lc rgb '#AA0000';
      set style line 3 lc rgb '#0000AA';



      set output '${PNG_LOW}';
      set title '50 plus petites usines';
      plot '${LOW}'  using ($2-$3):xtic(1) ls 1 title 'max-src', \
          ''        using ($3-$4)         ls 2 title 'src-real', \
          ''        using ($4)            ls 3 title 'real';





      set output '${PNG_HIGH}';
      set title '10 plus grandes usines';
          plot '${HIGH}'  using ($2-$3):xtic(1) ls 1 title 'max-src', \
          ''        using ($3-$4)         ls 2 title 'src-real', \
          ''        using ($4)            ls 3 title 'real';
      "

      afficher_fin
      exit 0
    fi

    # MODES NORMAUX 
    gnuplot -e "
    set terminal png;
    set datafile separator ';';
    set xlabel 'Usine';
    set ylabel 'Volume (M.m3.year-1)';
    set style fill solid 1.0 border -1;
    set xtics rotate by -90 font \",8\";
    set boxwidth 0.75 relative;
   


    set output '${PNG_LOW}';
    set title '50 plus petites usines';
    plot '${LOW}' using 2:xtic(1) with boxes notitle;

    set output '${PNG_HIGH}';
    set title '10 plus grandes usines';
    plot '${HIGH}' using 2:xtic(1) with boxes notitle;
    "

  else
    echo "Attention : gnuplot non trouve."
  fi

  afficher_fin
  exit 0
fi

# LEAKS 
if [ "$CMD" = "leaks" ]; then
./wildwater leaks "$CSV" "$ARG" leaks.dat
if [ $? -ne 0 ]; then
  afficher_fin
  exit 1

fi

  afficher_fin
  exit 0
fi

usage
afficher_fin
exit 1


