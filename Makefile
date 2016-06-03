CC=g++
CFLAGS=-I. -Wall -std=c++11 -lm -O3 

OBJS= EncodedPackage.o Encoder.o PaddingPackage.o Serializer.o RobustSolitonDistribution.o \

all: test_Encoder test_UDPRawServer test_UDPRawClient test_UDPEncodedServer test_UDPEncodedClient

test_Encoder: test_Encoder.o $(OBJS)
	$(CC) $(CFLAGS) -lboost_system -lpthread $^ -o $@

test_UDPRawServer: UDPServer.o UDPRawServer.o test_UDPRawServer.o $(OBJS)
	$(CC) $(CFLAGS) -lboost_system -lpthread $^ -o $@

test_UDPRawClient: UDPClient.o UDPRawClient.o test_UDPRawClient.o $(OBJS)
	$(CC) $(CFLAGS) -lboost_system -lpthread $^ -o $@

test_UDPEncodedServer: UDPServer.o UDPEncodedServer.o test_UDPEncodedServer.o $(OBJS)
	$(CC) $(CFLAGS) -lboost_system -lpthread $^ -o $@

test_UDPEncodedClient: UDPClient.o UDPEncodedClient.o test_UDPEncodedClient.o $(OBJS)
	$(CC) $(CFLAGS) -lboost_system -lpthread $^ -o $@

test_Encoder.o: test_Encoder.cpp
	$(CC) $(CFLAGS) -c test_Encoder.cpp

test_UDPRawServer.o: test_UDPRawServer.cpp
	$(CC) $(CFLAGS) -c test_UDPRawServer.cpp

test_UDPRawClient.o: test_UDPRawClient.cpp
	$(CC) $(CFLAGS) -c test_UDPRawClient.cpp

test_UDPEncodedServer.o: test_UDPEncodedServer.cpp
	$(CC) $(CFLAGS) -c test_UDPEncodedServer.cpp

test_UDPEncodedClient.o: test_UDPEncodedClient.cpp
	$(CC) $(CFLAGS) -c test_UDPEncodedClient.cpp

EncodedPackage.o: EncodedPackage.cpp
	$(CC) $(CFLAGS) -c EncodedPackage.cpp

Encoder.o: Encoder.cpp
	$(CC) $(CFLAGS) -c Encoder.cpp

PaddingPackage.o: PaddingPackage.cpp
	$(CC) $(CFLAGS) -c PaddingPackage.cpp

Serializer.o: Serializer.cpp
	$(CC) $(CFLAGS) -c Serializer.cpp

RobustSolitonDistribution.o: RobustSolitonDistribution.cpp
	$(CC) $(CFLAGS) -c RobustSolitonDistribution.cpp

UDPServer.o: UDPServer.cpp
	$(CC) $(CFLAGS) -c UDPServer.cpp

UDPRawServer.o:UDPRawServer.cpp
	$(CC) $(CFLAGS) -c UDPRawServer.cpp

UDPEncodedServer.o: UDPEncodedServer.cpp
	$(CC) $(CFLAGS) -c UDPEncodedServer.cpp

UDPClient.o: UDPClient.cpp
	$(CC) $(CFLAGS) -c UDPClient.cpp

UDPRawClient.o: UDPRawClient.cpp
	$(CC) $(CFLAGS) -c UDPRawClient.cpp

UDPEncodedClient.o: UDPEncodedClient.cpp
	$(CC) $(CFLAGS) -c UDPEncodedClient.cpp

.PHONY: clean

clean:
	rm -rf *.o *.h264
	find . -perm -u=rwx -a ! -name "st*" -a ! -regex "\./\..*" -delete
