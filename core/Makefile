CC = g++
CFLAGS = -std=c++20 -O2 -Wall -Wextra -Wno-unused-result -D_GLIBCXX_DEBUG -D_FORTIFY_SOURCE=2

OBJS = game.o engine.o player.o

run: $(OBJS) run.cpp
	$(CC) $(CFLAGS) $(OBJS) run.cpp -o run

game.o: game.h game.cpp
	$(CC) $(CFLAGS) -c game.cpp

engine.o: game.h engine.h engine.cpp
	$(CC) $(CFLAGS) -c engine.cpp

player.o: game.h player.h player.cpp
	$(CC) $(CFLAGS) -c player.cpp

clean:
	rm -f *.o run
