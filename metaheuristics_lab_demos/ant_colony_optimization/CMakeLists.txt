cmake_minimum_required(VERSION 3.0.0)
project(ant_colony_optimization_metaheuristics_lab_demos VERSION 0.1.0 LANGUAGES C)

#include(CTest)
#enable_testing()

# target_include_directories(${PROJECT_NAME} PUBLIC ../../sa ../../data testDir)
# target_include_directories(${PROJECT_NAME} PUBLIC testDir)
# add_subdirectory(testDir)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
configure_file(gui.ui ${CMAKE_CURRENT_BINARY_DIR}/gui.ui COPYONLY)
link_directories(${GTK_LIBRARY_DIRS})

add_executable(ant_colony_optimization_metaheuristics_lab_demos main.c fio.c optimizer_fio.c abatch.c avarlogger.c)

target_link_libraries(${PROJECT_NAME}
    sub::aco
    sub::data
)
target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})


#target_include_directories( ${PROJECT_NAME}
#    PUBLIC ${PROJECT_SOURCE_DIR}/..
#)


#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
