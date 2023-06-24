COMP  = gcc
APP = myShell

${APP}: myShell.c source.c scanner.c parser.c node.c executor.c prompt.c
	gcc -o shell myShell.c source.c scanner.c parser.c node.c executor.c prompt.c
