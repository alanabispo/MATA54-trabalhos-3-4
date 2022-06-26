cmake_minimum_required(VERSION 3.21)
project(MATA54-trabalhos-3-4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "")

### ----- Bibliotecas ----- ###

# Adiciona bibliotecas
add_library(registro STATIC lib/registro.h lib/registro.c)
add_library(pagina STATIC lib/pagina.h lib/pagina.c)
add_library(arvore_2d STATIC lib/arvore_2d.h lib/arvore_2d.c)
add_library(utils STATIC lib/utils.h lib/utils.c)

### ------- Binários ------ ###

# Adiciona executável para o trabalho 3
add_executable(MATA54-trabalho-3 main_trab3.c)
target_link_libraries(MATA54-trabalho-3 registro pagina arvore_2d utils)

# Adiciona executável para o trabalho 4
add_executable(MATA54-trabalho-4 main.c)
target_link_libraries(MATA54-trabalho-4 registro pagina arvore_2d utils)

### ------- Testes ------- ###
enable_testing()

# Outra alternativa
# file(GLOB TEST "test/*.cc")

# Biblioteca de teste
add_library(test_lib STATIC lib/tests/test_lib.h lib/tests/test_lib.c)

# Executáveis de teste
add_executable(arvore_2d_test lib/arvore_2d_test.c)
target_link_libraries(arvore_2d_test test_lib registro pagina arvore_2d utils)
add_test(NAME arvore_2d_test COMMAND arvore_2d_test)

# Teste Registro
add_executable(registro_test lib/registro_test.c)
target_link_libraries(registro_test test_lib registro pagina arvore_2d utils)
add_test(NAME registro_test COMMAND registro_test)

# Teste Página
add_executable(pagina_test lib/pagina_test.c)
target_link_libraries(pagina_test test_lib registro pagina arvore_2d utils)
add_test(NAME pagina_test COMMAND pagina_test)

# Teste Utils
add_executable(utils_test lib/utils_test.c)
target_link_libraries(utils_test test_lib registro pagina arvore_2d utils)
add_test(NAME utils_test COMMAND utils_test)