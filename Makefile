main: gcc -Wextra -Wall -g src/game.c src/main.c src/move.c

clean:
	rm -rf *.o
	rm -rf src/a.out
	rm -rf src/main
#rm main

push: git add *; git commit -s -m "push"; git push