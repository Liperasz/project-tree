cmake_minimum_required(VERSION 3.31.6)
project(project_tree C)

set(CMAKE_C_STANDARD 11)

add_executable(project_tree main.c Tree/Tree.c String/String.c Archive/Archive.c Command/Command.c)