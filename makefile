compile:
	g++ -o mine -I /usr/include/X11 -L /usr/X11R6/lib -lX11 -L /usr/include/X11/ -lXpm -fpermissive -Wno-narrowing src/minesweeper.cpp

clean:
	rm mine