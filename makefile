.PHONY: all clean
all: client server

Chat.cpp Chat.h:
	slice2cpp Chat.ice

client: Client.cpp Chat.cpp ChatI.cpp
	g++ -I. Client.cpp Chat.cpp ChatI.cpp -lIce -lpthread -o client

server: Server.cpp Chat.cpp ChatI.cpp
	g++ -I. Server.cpp Chat.cpp ChatI.cpp -lIce -lpthread -o server

clean:
	-rm Chat.cpp Chat.h client server