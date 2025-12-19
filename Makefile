CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJ = main.o usines_donnes.o pertes.o lecture_fichier.o arbre.o AVL.o Utilitaire.o
EXEC = wildwater

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c usines_donnes.h pertes.h Utilitaire.h
	$(CC) $(CFLAGS) -c main.c

usines_donnes.o: usines_donnes.c usines_donnes.h lecture_fichier.h Utilitaire.h AVL.h
	$(CC) $(CFLAGS) -c usines_donnes.c

pertes.o: pertes.c pertes.h lecture_fichier.h Utilitaire.h AVL.h arbre.h
	$(CC) $(CFLAGS) -c pertes.c

lecture_fichier.o: lecture_fichier.c lecture_fichier.h
	$(CC) $(CFLAGS) -c lecture_fichier.c

arbre.o: arbre.c arbre.h
	$(CC) $(CFLAGS) -c arbre.c

AVL.o: AVL.c AVL.h
	$(CC) $(CFLAGS) -c AVL.c

Utilitaire.o: Utilitaire.c Utilitaire.h
	$(CC) $(CFLAGS) -c Utilitaire.c

clean:
	rm -f $(OBJ) $(EXEC) *.o

distclean: clean
	rm -f *.dat *.png
