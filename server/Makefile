CC = g++
CFLAGS = -std=c++11 -g -Wall -pthread

s_filetrans:s_main.cpp threadpool.o
	$(CC) $(CFLAGS) threadpool.o s_main.cpp -o s_filetrans
threadpool.o:threadpool.cpp
	$(CC) $(CFLAGS) -c threadpool.cpp

clean:
	rm -rf *.o s_filetrans