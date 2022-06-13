# !!! AVISO !!!
# Este Makefile define alguns scripts para facilitar o uso geral do projeto
# porém o mesmo ainda é compatível com muitos utilitários do clang e
# consequentemente com ferramentas que sejam compatíveis com este build system
# e.g. Clion, QtCreator, etc. Caso REALMENTE não QUEIRA ou NÃO POSSA utilizar
# o cmake, os comandos alternativos (build-alt, test-alt, ...) irão performar
# de maneira muito similar a suas alternativas com o cmake.
# Se é sua primeira vez utilizando cmake recomendo ;)

CMAKE_BUILD_FOLDER = ./cmake-build-debug

# Inicializa o trabalho 4
start: build MATA54-trabalho-4
	./MATA54-trabalho-4

# Inicializa o trabalho 3
start-trabalho3: build MATA54-trabalho-3
	./MATA54-trabalho-3

# Limpa binários e arquivos de build
clean:
	rm -rf $(CMAKE_BUILD_FOLDER)
	rm -rf MATA54-trabalho-3 MATA54-trabalho-4
	rm -rf ./*.dat

# Prepara o projeto
prepare: clean
	mkdir -p $(CMAKE_BUILD_FOLDER) && \
	cd $(CMAKE_BUILD_FOLDER) && \
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja ../

# Constrói de maneira alternativa sem a necessidade do cmake
build-alt: main.c main_trab3.c
	gcc -o MATA54-trabalho-3 main_trab3.c
	gcc -o MATA54-trabalho-4 main.c

# Constrói utilizando cmake
build: CMakeLists.txt main.c main_trab3.c
	cmake --build $(CMAKE_BUILD_FOLDER) --target MATA54-trabalho-3 && cp $(CMAKE_BUILD_FOLDER)/MATA54-trabalho-3 .
	cmake --build $(CMAKE_BUILD_FOLDER) --target MATA54-trabalho-4 && cp $(CMAKE_BUILD_FOLDER)/MATA54-trabalho-4 .

# Realiza uma construção limpa
clean-build: clean build

# Realiza uma construção limpa utilizando o build alternativo
clean-build-alt: clean build-alt

# Realiza testes unitários utilizando um script simples de teste
test-alt: main.c main_trab3.c
	# etah

# Realiza testes utilizando a suite CMakeTest
test: CMakeLists.txt main.c main_trab3.c
	# something something

# Ajuda - Adicionar sempre que houver novas dependências
help:
	@echo "Targets validos para este Makefile:"
	@echo "... start (default)"
	@echo "... start-trabalho3 (default)"
	@echo "... clean"
	@echo "... prepare"
	@echo "... build-alt"
	@echo "... build"
	@echo "... clean-build"
	@echo "... clean-build-alt"
	@echo "... test-alt"
	@echo "... test"