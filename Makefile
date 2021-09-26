SHELL = /bin/sh
.POSIX:

# anote version
VERSION = 1.0
PROJ_NAME = tart

#paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

#includes and libs
LIBS=-lncurses

#flags
#no debug:
#CFLAGS= -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os
#debug:
CFLAGS= -g -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os

#compiler and linker
CC=gcc
#cleaning program
RM=rm -rf

C_SRC=${wildcard ./*.c}
H_SRC=${wildcard ./*.h}
OBJ=${subst .c,.o,${subst ./,./build/,${C_SRC}}}

all: options BD_DIR ${PROJ_NAME}

options:
	@echo ${PROJ_NAME} build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo

${PROJ_NAME}: ${OBJ}
	@echo
	@echo "building $@ binary using ${CC} linker"
	@echo "Objects: $^"
	@echo "Libs: ${LIBS}"
	@${CC} $^ -o $@ ${LIBS}
	@echo '------------------------'
	@echo 'finished building binary'

config:
	@echo "config"
	cp ./config.def.h ./$@.h
	@echo

./build/%.o: ./%.c ./%.h
	@echo "[${CC}] $<"
	@ ${CC} $< -c ${CFLAGS} -o $@

./build/main.o: ./main.c ${H_SRC}
	@echo "[${CC}] $<"
	@ ${CC} $< -c ${CFLAGS} -o $@

BD_DIR:
	mkdir -p build

install: ${PROJ_NAME}
	@echo "install"
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${PROJ_NAME} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${PROJ_NAME}
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < ${PROJ_NAME}.1 > ${DESTDIR}${MANPREFIX}/man1/${PROJ_NAME}.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/${PROJ_NAME}.1
	@echo

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROJ_NAME}\
		${DESTDIR}${MANPREFIX}/man1/${PROJ_NAME}.1

clean:
	${RM} ${PROJ_NAME} ./build ./config.h

run:
	./${PROJ_NAME}

.PHONY: clean options all
