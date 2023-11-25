cmake_minimum_required(VERSION 3.0.0)
project(genetic_algorithm VERSION 0.1.0 LANGUAGES C)

add_library(${PROJECT_NAME} ga.c ga_bin_r_basic.c)
add_library(sub::ga ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)