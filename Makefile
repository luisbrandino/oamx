all:
	gcc src/main.c src/mbc.c -o oamx.exe

test:
	gcc tests/test_mbc.c src/memory.c src/mbc.c -o test_oamx.exe