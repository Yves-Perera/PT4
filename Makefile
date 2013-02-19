CC = g++
CFLAGS = -lpthread -lsfml-graphics
EXEC_NAME = JeuDeLaVie
INCLUDES =
LIBS =
OBJ_FILES = gameLife.o Model.o Control.o View.o
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES) 
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) -lpthread -lsfml-graphics


gameLife.o: gameLife.cpp Model.h Control.h View.h
	$(CC) $(CFLAGS) -o $@ -c $<
	
Model.o: Model.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
		
 
Control.o: Control.cpp Model.h
	$(CC) $(CFLAGS) -o $@ -c $<

View.o: View.cpp Model.h
	$(CC) $(CFLAGS) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)
