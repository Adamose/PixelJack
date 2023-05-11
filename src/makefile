LIB = -lraylib -lopengl32 -lgdi32 -lwinmm
OBJ = Main.o GamePanel.o ChipPanel.o Card.o Button.o MessageBoard.o
ARG = -L ../lib -I ../include ../resources/misc/PixelJack.res

all: $(OBJ)
	g++ -o PixelJack $(OBJ) $(LIB) $(ARG)

Main.o: Main.cpp GamePanel.hpp
	g++ -c Main.cpp

GamePanel.o: GamePanel.cpp GamePanel.hpp
	g++ -c GamePanel.cpp

ChipPanel.o: ChipPanel.cpp ChipPanel.hpp
	g++ -c ChipPanel.cpp

Card.o: Card.cpp Card.hpp
	g++ -c Card.cpp

Button.o: Button.cpp Button.hpp
	g++ -c Button.cpp

MessageBoard.o: MessageBoard.cpp MessageBoard.hpp
	g++ -c MessageBoard.cpp

clean:
	rm -f PixelJack.exe $(OBJ)