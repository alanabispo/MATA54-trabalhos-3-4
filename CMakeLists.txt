cmake_minimum_required(VERSION 3.21)
project(MATA54-trabalhos-3-4 C)

set(CMAKE_C_STANDARD 99)

# Adiciona executável para o trabalho 3
add_executable(MATA54-trabalho-3 main_trab3.c lib/registro.c lib/registro.h lib/pagina.c lib/pagina.h lib/arvore_2d.c lib/arvore_2d.h lib/constantes.h lib/utils.h lib/utils.c)

# Adiciona executável para o trabalho 4
add_executable(MATA54-trabalho-4 main.c lib/registro.c lib/registro.h lib/pagina.c lib/pagina.h lib/arvore_2d.c lib/arvore_2d.h lib/constantes.h lib/utils.h lib/utils.c)

# Testes para a biblioteca responsável pelo índice
#add_test()
