cmake_minimum_required(VERSION 3.21)
project(MATA54-trabalhos-3-4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "")

set(OUT_FILE "out.txt")

### ----- Bibliotecas ----- ###

# Adiciona bibliotecas
add_library(registro STATIC lib/registro.h lib/registro.c)
add_library(pagina STATIC lib/pagina.h lib/pagina.c)
add_library(arvore_2d STATIC lib/arvore_2d.h lib/arvore_2d.c)
add_library(utils STATIC lib/utils.h lib/utils.c)

### ------- Binários ------ ###

# Adiciona executável para o trabalho 3
add_executable(MATA54-trabalho-3 main_trab3.c)
target_link_libraries(MATA54-trabalho-3 utils registro pagina arvore_2d)

# Adiciona executável para o trabalho 4
add_executable(MATA54-trabalho-4 main.c)
target_link_libraries(MATA54-trabalho-4 utils registro pagina arvore_2d)

### ------- Testes ------- ###
enable_testing()

# Outra alternativa
# file(GLOB TEST "test/*.cc")

# Biblioteca de teste
add_library(test_lib STATIC lib/tests/test_lib.h lib/tests/test_lib.c)

# Executáveis de teste
add_executable(arvore_2d_test lib/arvore_2d_test.c)
target_link_libraries(arvore_2d_test pagina registro utils arvore_2d test_lib)
add_test(NAME arvore_2d_test COMMAND arvore_2d_test)
add_test(NAME arvore_2d_test_generate_stdout COMMAND sh -c "./arvore_2d_test --generate-stdout > ${OUT_FILE}")
add_test(NAME arvore_2d_test_test_generated_stdout COMMAND arvore_2d_test --test-generated-stdout)

# Teste Registro
add_executable(registro_test lib/registro_test.c)
target_link_libraries(registro_test test_lib utils registro pagina arvore_2d)
add_test(NAME registro_test COMMAND registro_test)
add_test(NAME registro_test_generate_stdout COMMAND sh -c "./registro_test --generate-stdout > ${OUT_FILE}")
add_test(NAME registro_test_test_generated_stdout COMMAND registro_test --test-generated-stdout)

# Teste Página
add_executable(pagina_test lib/pagina_test.c)
target_link_libraries(pagina_test test_lib utils registro pagina arvore_2d)
add_test(NAME pagina_test COMMAND pagina_test)
add_test(NAME pagina_test_generate_stdout COMMAND sh -c "./pagina_test --generate-stdout > ${OUT_FILE}")
add_test(NAME pagina_test_test_generated_stdout COMMAND pagina_test --test-generated-stdout)

# Teste Utils
add_executable(utils_test lib/utils_test.c)
target_link_libraries(utils_test test_lib utils registro pagina arvore_2d)
add_test(NAME utils_test COMMAND utils_test)