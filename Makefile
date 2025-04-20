SHELL = /bin/sh
.POSIX:

VERSION = 1.0
PROJ_NAME = tart

#paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

#includes and libs
LIBS=-lncurses -lm -lpanel

#flags
#no debug:
#CFLAGS= -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os
#debug:
CFLAGS= -g -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os

#compiler and linker
CC=gcc
#cleaning program
RM=rm -rf

C_SRC=${wildcard ./src/*.c}
H_SRC=${wildcard ./src/*.h}
OBJ=${subst .c,.o,${subst ./src/,./build/,${C_SRC}}}

all: options BD_DIR ${PROJ_NAME}

options:
	@echo ${PROJ_NAME} build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "SRC"
	@echo ${C_SRC}
	@echo "H"
	@echo ${H_SRC}
	@echo "OBJ"
	@echo ${OBJ}
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

./build/%.o: ./src/%.c ./src/%.h
	@echo "[${CC}] $<"
	@ ${CC} $< -c ${CFLAGS} -o $@

./build/main.o: ./src/main.c ${H_SRC}
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

run: ${PROJ_NAME}
	./${PROJ_NAME}

tbar: ${PROJ_NAME}
	./${PROJ_NAME} -t bar -l "bar1,bar2,bar3" -v "10,15,20"

tline: ${PROJ_NAME}
	./${PROJ_NAME} -t line -l "line1,line2,line3" -v "5,10,15"

tpie: ${PROJ_NAME}
	./${PROJ_NAME} -t pie -l "mozzarela,pepperoni,margherita" -v "33,33,34"

.PHONY: clean options all
