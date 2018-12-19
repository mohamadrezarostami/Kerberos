Kerberos : des.o des3.o rsa.o sha256.o as_server.o tgs_server.o service_server.o ticket.o user.o utils.o client.o main.o
	g++ -g des.o des3.o rsa.o sha256.o as_server.o tgs_server.o service_server.o ticket.o user.o utils.o client.o main.o -o Kerberos
main.o : main.cpp
	g++ -g -c main.cpp
des.o : des.cpp des.h
	g++ -g -c des.cpp
des3.o : des3.cpp des3.h
	g++ -g -c des3.cpp
rsa.o : rsa.cpp rsa.h
	g++ -g -c rsa.cpp
sha256.o : sha256.cpp sha256.h
	g++ -g -c sha256.cpp
as_server.o : as_server.cpp as_server.h
	g++ -g -c as_server.cpp
tgs_server.o : tgs_server.cpp tgs_server.h
	g++ -g -c tgs_server.cpp
service_server.o : service_server.cpp service_server.h
	g++ -g -c service_server.cpp
ticket.o : ticket.cpp ticket.h
	g++ -g -c ticket.cpp
user.o : user.cpp user.h
	g++ -g -c user.cpp
utils.o : utils.cpp utils.h
	g++ -g -c utils.cpp
client.o : client.cpp client.h
	g++ -g -c client.cpp
clean :
	rm *.o