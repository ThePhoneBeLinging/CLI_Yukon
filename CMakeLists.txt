cmake_minimum_required(VERSION 3.26)
project(CLI_Yukon C)

set(CMAKE_C_STANDARD 23)

add_executable(CLI_Yukon main.c
        Card.h
        Card.c
        Utility.h
        Utility.c
        StateMachine.h
)
