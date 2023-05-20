build: src/main.c
	gcc -o bin/main.exe src/main.c

run:
	cd bin/ && cmd.exe /C "start main.exe" 