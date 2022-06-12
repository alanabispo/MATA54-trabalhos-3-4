# !!! AVISO !!!
# Este Makefile define alguns scripts para facilitar o uso geral do projeto
# porém o mesmo ainda é compatível com muitos utilitários do clang e
# consequentemente com ferramentas que sejam compatíveis com este build system
# e.g. Clion, QtCreator, etc. Caso REALMENTE não QUEIRA ou NÃO POSSA utilizar
# o cmake, os comandos alternativos (build-alt, test-alt, ...) irão performar
# de maneira muito similar a suas alternativas com o cmake.
# Se é sua primeira vez utilizando cmake recomendo ;)

build-alt:
	gcc -o MATA54-trabalho-3 main_trab3.c
	gcc -o MATA54-trabalho-4 main.c

build: CMakeLists.txt main.c main_trab3.c
	cmake --build ./cmake-build-debug --target MATA54-trabalho-3
	cmake --build ./cmake-build-debug --target MATA54-trabalho-4

test-alt:
	# etah

test: CMakeLists.txt main.c main_trab3.c main_test.c main_trab3_test.c
	# something something