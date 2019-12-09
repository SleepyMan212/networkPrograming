all: hw

hw: Server.cpp Http.cpp Manage.cpp User.cpp
	# g++ -o hw hw.cpp
	g++ -c Server.cpp Http.cpp User.cpp Manage.cpp
	g++ -g -std=c++14 -o hw hw.cpp Server.o Http.o User.o Manage.o

clean:
	rm -f hw *.o