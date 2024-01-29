SHELL := /bin/bash

CC = g++-10

default:
	$(CC) -std=c++2a -O3 main.cc

debug:
	$(CC) -std=c++2a -g main.cc

run: default
	time ./a.out

clean:
	rm -f a.out
