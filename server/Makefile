CC = g++
CFLAGS = -std=c++11 -g -Wall -pthread -D DEBUG

s_filetrans:s_main.cpp threadpool.o s_recv_file.o s_socket_connect.o s_socket_listen.o s_send_file.o
	$(CC) $(CFLAGS) threadpool.o s_recv_file.o s_socket_connect.o s_socket_listen.o s_send_file.o s_main.cpp -o s_filetrans
threadpool.o:threadpool.cpp
	$(CC) $(CFLAGS) -c threadpool.cpp
s_recv_file.o:s_recv_file.cpp
	$(CC) $(CFLAGS) -c s_recv_file.cpp
s_send_file.o:s_send_file.cpp
	$(CC) $(CFLAGS) -c s_send_file.cpp
s_socket_connect.o:s_socket_connect.cpp
	$(CC) $(CFLAGS) -c s_socket_connect.cpp
s_socket_listen.o:s_socket_listen.cpp
	$(CC) $(CFLAGS) -c s_socket_listen.cpp


clean:
	rm -rf *.o s_filetrans
