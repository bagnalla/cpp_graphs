SHELL := /bin/bash

CC = g++

default:
	$(CC) -std=c++23 -O3 main.cc

debug:
	$(CC) -std=c++23 -g main.cc

test:
	$(CC) -std=c++23 -g test.cc

run: default
	time ./a.out

clean:
	rm -f a.out
