CC = g++
CFLAGS = -std=c++11 -g -Wall -pthread

c_filetrans:c_main.cpp c_send_file.o c_socket_connect.o
	$(CC) $(CFLAGS) c_send_file.o c_socket_connect.o c_main.cpp -o c_filetrans
c_send_file.o:c_send_file.cpp
	$(CC) $(CFLAGS) -c c_send_file.cpp
c_socket_connect.o:c_socket_connect.cpp
	$(CC) $(CFLAGS) -c c_socket_connect.cpp

clean:
	rm -rf *.o c_filetrans