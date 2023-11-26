cmake_minimum_required(VERSION 3.0.0)
project(metaheuristics VERSION 0.1.0 LANGUAGES C)
add_subdirectory(sa)
add_subdirectory(ga)
add_subdirectory(data)

#add_subdirectory(metaheuristics_lab_demos/simulated_annealing)
add_subdirectory(metaheuristics_lab_demos/genetic_algorithm)
