# !!! AVISO !!!
# Este Makefile define alguns scripts para facilitar o uso geral do projeto
# porém o mesmo ainda é compatível com muitos utilitários do clang e
# consequentemente com ferramentas que sejam compatíveis com este build system
# e.g. Clion, QtCreator, etc. Caso REALMENTE não QUEIRA ou NÃO POSSA utilizar
# o cmake, os comandos alternativos (build-alt, test-alt, ...) irão performar
# de maneira muito similar a suas alternativas com o cmake.
# Se é sua primeira vez utilizando cmake recomendo ;)

CMAKE_BUILD_FOLDER = ./cmake-build-debug
ALT_BUILD_FOLDER = ./gcc-build-debug
LOG_FILE = tmp_log.txt
OUT_FILE = out.txt

# Inicializa o trabalho 4
start: build MATA54-trabalho-4
	./MATA54-trabalho-4

# Inicializa o trabalho 3
start-trabalho3: build MATA54-trabalho-3
	./MATA54-trabalho-3

# Limpa binários e arquivos de build
clean:
	rm -rf $(CMAKE_BUILD_FOLDER)
	rm -rf $(ALT_BUILD_FOLDER)
	rm -rf $(LOG_FILE)
	rm -rf $(OUT_FILE)
	rm -rf MATA54-trabalho-3 MATA54-trabalho-4
	rm -rf ./*.dat
	rm -rf ./trabalho.zip

# Prepara o projeto
prepare: clean
	mkdir -p $(CMAKE_BUILD_FOLDER) && \
	mkdir -p $(ALT_BUILD_FOLDER) && \
	cd $(CMAKE_BUILD_FOLDER) && \
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja ../

# Constrói as bibliotecas para o projeto
build-lib-alt: lib/registro.c lib/utils.c lib/pagina.c lib/arvore_2d.c
	# Compila libs
	gcc -c lib/arvore_2d.c -o $(ALT_BUILD_FOLDER)/arvore_2d.o
	gcc -c lib/pagina.c -o $(ALT_BUILD_FOLDER)/pagina.o
	gcc -c lib/registro.c -o $(ALT_BUILD_FOLDER)/registro.o
	gcc -c lib/utils.c -o $(ALT_BUILD_FOLDER)/utils.o
	# Gera arquivos para linkage static
	ar rcs $(ALT_BUILD_FOLDER)/libarvore_2d.a $(ALT_BUILD_FOLDER)/arvore_2d.o
	ar rcs $(ALT_BUILD_FOLDER)/libpagina.a $(ALT_BUILD_FOLDER)/pagina.o
	ar rcs $(ALT_BUILD_FOLDER)/libregistro.a $(ALT_BUILD_FOLDER)/registro.o
	ar rcs $(ALT_BUILD_FOLDER)/libutils.a $(ALT_BUILD_FOLDER)/utils.o

# Constrói de maneira alternativa sem a necessidade do cmake
# ALERTA: Testes removidos por não serem obrigatórios mas altamente recomendados
# COMANDO: make test-alt build-alt
build-alt: build-lib-alt main.c main_trab3.c
	gcc -o MATA54-trabalho-3 main_trab3.c -L$(ALT_BUILD_FOLDER) -larvore_2d -lpagina -lregistro -lutils
	gcc -o MATA54-trabalho-4 main.c -L$(ALT_BUILD_FOLDER) -larvore_2d -lpagina -lregistro -lutils

# Constrói testes com cmake
build-test: lib/registro_test.c lib/utils_test.c lib/pagina_test.c lib/arvore_2d_test.c
	cmake --build $(CMAKE_BUILD_FOLDER)

# Constrói testes com gcc
build-test-alt: build-lib-alt lib/registro_test.c lib/utils_test.c lib/pagina_test.c lib/arvore_2d_test.c
	# Compila lib auxiliar para testes
	gcc -c lib/tests/test_lib.c -o $(ALT_BUILD_FOLDER)/test_lib.o
	ar rcs $(ALT_BUILD_FOLDER)/libtest_lib.a $(ALT_BUILD_FOLDER)/test_lib.o
	# Compila testes
	gcc lib/arvore_2d_test.c -o $(ALT_BUILD_FOLDER)/arvore_2d_test.o -L$(ALT_BUILD_FOLDER) -larvore_2d -lpagina -lregistro -lutils -ltest_lib
	gcc lib/pagina_test.c -o $(ALT_BUILD_FOLDER)/pagina_test.o -L$(ALT_BUILD_FOLDER) -larvore_2d -lpagina -lregistro -lutils -ltest_lib
	gcc lib/registro_test.c -o $(ALT_BUILD_FOLDER)/registro_test.o -L$(ALT_BUILD_FOLDER) -larvore_2d -lpagina -lregistro -lutils -ltest_lib
	gcc lib/utils_test.c -o $(ALT_BUILD_FOLDER)/utils_test.o -L$(ALT_BUILD_FOLDER) -larvore_2d -lpagina -lregistro -lutils -ltest_lib

# Constrói utilizando cmake
build: test
	cmake --build $(CMAKE_BUILD_FOLDER) --target MATA54-trabalho-3 && cp $(CMAKE_BUILD_FOLDER)/MATA54-trabalho-3 .
	cmake --build $(CMAKE_BUILD_FOLDER) --target MATA54-trabalho-4 && cp $(CMAKE_BUILD_FOLDER)/MATA54-trabalho-4 .

# Realiza uma construção limpa
clean-build: clean build

# Realiza uma construção limpa utilizando o build alternativo
clean-build-alt: clean build-alt

# Função para imprimir mensagem de erro nos testes
define print_error
	echo "FAILED" && cat "tmp_log.txt"
endef

# Realiza testes unitários utilizando um script simples de teste
test-alt: build-lib-alt build-test-alt
	@echo "Executando testes..."
	@# Executa Testes
	@printf "1/10 Test #1: arvore_2d --- " && \
	( ( $(ALT_BUILD_FOLDER)/arvore_2d_test.o 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "2/10 Test #3: arvore_2d_test --generate-stdout --- " && \
	( ( $(ALT_BUILD_FOLDER)/arvore_2d_test.o --generate-stdout > $(OUT_FILE) 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "3/10 Test #4: arvore_2d_test --test-generated-stdout --- " && \
	( ( $(ALT_BUILD_FOLDER)/arvore_2d_test.o --test-generated-stdout 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "4/10 Test #2: pagina_test --- " && \
	( ( $(ALT_BUILD_FOLDER)/pagina_test.o 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "5/10 Test #3: pagina_test --generate-stdout --- " && \
	( ( $(ALT_BUILD_FOLDER)/pagina_test.o --generate-stdout > $(OUT_FILE) 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "6/10 Test #4: pagina_test --test-generated-stdout --- " && \
	( ( $(ALT_BUILD_FOLDER)/pagina_test.o --test-generated-stdout 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "7/10 Test #5: registro_test --- " && \
	( ( $(ALT_BUILD_FOLDER)/registro_test.o 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "8/10 Test #3: registro_test --generate-stdout --- " && \
	( ( $(ALT_BUILD_FOLDER)/registro_test.o --generate-stdout > $(OUT_FILE) 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "9/10 Test #4: registro_test --test-generated-stdout --- " && \
	( ( $(ALT_BUILD_FOLDER)/registro_test.o --test-generated-stdout 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	printf "10/10 Test #6: utils_test --- " && \
	( ( $(ALT_BUILD_FOLDER)/utils_test.o 2> $(LOG_FILE) && echo "PASSED") || ( $(call print_error) && exit 1 ) ) && \
	echo "Todos os testes passaram!" && rm -rf $(LOG_FILE)

# Realiza testes utilizando a suite CMakeTest
test: build-test
	ctest --extra-verbose --output-on-failure --test-dir $(CMAKE_BUILD_FOLDER)

# Compacta o trabalho para entrega
package:
	git archive -o trabalho.zip HEAD

# Ajuda - Adicionar sempre que houver novas dependências
help:
	@echo "Targets validos para este Makefile:"
	@echo "... start (default)"
	@echo "... start-trabalho3 (default)"
	@echo "... clean"
	@echo "... prepare"
	@echo "... build-alt"
	@echo "... build"
	@echo "... clean-build-alt"
	@echo "... clean-build"
	@echo "... test-alt"
	@echo "... test"
	@echo "... package"