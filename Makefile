COMP  = gcc
APP = myShell
APP1 = shell
APP2 = prompt

${APP}: ${APP}.c ${APP2}.c
	${COMP} -o ${APP1} ${APP}.c ${APP2}.c
