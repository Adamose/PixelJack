LIB = -lraylib -lopengl32 -lgdi32 -lwinmm
OBJ = Main.o GamePanel.o ChipPanel.o Card.o

all: $(OBJ)
	g++ -o PixelJack $(OBJ) $(LIB) ../resources/misc/PixelJack.res

Main.o: Main.cpp GamePanel.hpp
	g++ -c Main.cpp

GamePanel.o: GamePanel.cpp GamePanel.hpp
	g++ -c GamePanel.cpp

ChipPanel.o: ChipPanel.cpp ChipPanel.hpp
	g++ -c ChipPanel.cpp

Card.o: Card.cpp Card.hpp
	g++ -c Card.cpp

clean:
	rm -f PixelJack.exe $(OBJ)