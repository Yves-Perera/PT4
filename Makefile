CC = g++
ifeq (x,$(firstword $(MAKECMDGOALS)))
	CFLAGS=-lpthread -lsfml-graphics -DGRAPHIQUE
else ifeq (debug,$(firstword $(MAKECMDGOALS)))
	CFLAGS=-lpthread -lsfml-graphics -g
else
	CFLAGS=-lpthread -lsfml-graphics
endif
EXEC_NAME = JeuDeLaVie
INCLUDES =
LIBS =
OBJ_FILES = gameLife.o Model.o Control.o View.o Constantes.o
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

x: $(EXEC_NAME)

debug: $(EXEC_NAME)

clean :
	rm $(OBJ_FILES)
	
mrpropper:
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES) 
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) -lpthread -lsfml-graphics -lsfml-system -lsfml-window


gameLife.o: gameLife.cpp Model.h Control.h View.h Constantes.h
	$(CC) $(CFLAGS) -o $@ -c $<
	
Model.o: Model.cpp Constantes.h
	$(CC) $(CFLAGS) -o $@ -c $<

Constantes.o: Constantes.cpp Constantes.h
	$(CC) $(CFLAGS) -o $@ -c $<		
 
Control.o: Control.cpp Model.h Constantes.h
	$(CC) $(CFLAGS) -o $@ -c $<

View.o: View.cpp Model.h Constantes.h
	$(CC) $(CFLAGS) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)
