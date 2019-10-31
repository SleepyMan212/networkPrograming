all: testServer testClient

testServer: testServer.c
	gcc -o testServer testServer.c
testClient: testClient.c
	gcc -o testClinet testClient.c

clean:
	rm -rf testClient testServer
