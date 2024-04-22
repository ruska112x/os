all: clean build-gen run-gen build
	@echo "==OD==";
	@od file.hole;
	@echo "=STAT=";
	@stat file.hole;
	@echo "======"

clean:
	@rm -rf a.out gen file.hole

build:
	@g++ -Wall -Wextra main.cpp

build-gen:
	@gcc -Wall -Wextra hole.c -o gen

run-gen:
	./gen
