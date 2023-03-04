LIB = -lraylib -lopengl32 -lgdi32 -lwinmm
OBJ = Main.o GamePanel.o Card.o

all: $(OBJ)
	g++ -o PixelJack $(OBJ) $(LIB) ../resources/misc/PixelJack.res

Main.o: Main.cpp GamePanel.hpp
	g++ -c Main.cpp

GamePanel.o: GamePanel.cpp GamePanel.hpp
	g++ -c GamePanel.cpp

Card.o: Card.cpp Card.hpp
	g++ -c Card.cpp

clean:
	rm -f PixelJack.exe $(OBJ)