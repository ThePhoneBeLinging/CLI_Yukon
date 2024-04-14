cmake_minimum_required(VERSION 3.28)
project(CLI_Yukon C)

set(CMAKE_C_STANDARD 23)

add_executable(CLI_Yukon main.c
        Card.h
        Card.c
        main.h
        Utility.h
        Utility.c
)
