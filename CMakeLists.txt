cmake_minimum_required(VERSION 3.10)
project(erpc_client_pipe C)

set(CMAKE_C_STANDARD 11)

add_executable(erpc_client_pipe main.c erpc.h erpc.c)