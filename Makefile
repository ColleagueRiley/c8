buildLinux:
	@gcc source/main.c -I include -lX11 -o c8

buildWindows:
	@gcc source/main.c -I include -lgdi32 -lshell32 -o c8.exe
	
test:	
	./c8 test.rop

debug:
	make buildLinux
	make test