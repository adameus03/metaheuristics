cmake_minimum_required(VERSION 3.0.0)
project(ant_colony_optimization VERSION 0.1.0 LANGUAGES C)

add_library(${PROJECT_NAME} aco.c linked_list.c aco_r2_cart_euc.c)
add_library(sub::aco ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/.
)