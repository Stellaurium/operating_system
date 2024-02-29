all: hanoi-r hanoi-nr

hanoi-r: src/hanoi-main.c src/hanoi-r.c
	gcc -g -o $@ $^

hanoi-nr: src/hanoi-main.c src/hanoi-nr.c
	gcc -g -o $@ $^

clean:
	rm -f hanoi-r hanoi-nr
