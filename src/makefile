LIB = -lraylib -lopengl32 -lgdi32 -lwinmm
OBJ = Main.o GamePanel.o ChipPanel.o Card.o Button.o MessageBoard.o
CPP = Main.cpp GamePanel.cpp ChipPanel.cpp Card.cpp Button.cpp MessageBoard.cpp
ARG = -L ../lib -I ../include ../resources/misc/PixelJack.res

all: $(OBJ)
	g++ -o PixelJack $(OBJ) $(LIB) $(ARG)

web:
	emcc -o index.html $(CPP) -Os ../lib/web/libraylib.a -I ../include -s USE_GLFW=3 -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=2 -s ALLOW_MEMORY_GROWTH=1 --preload-file ../resources -DPLATFORM_WEB --shell-file ../lib/web/shell.html
	mkdir web-build
	mv index.data index.html index.js index.wasm index.worker.js ./web-build

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