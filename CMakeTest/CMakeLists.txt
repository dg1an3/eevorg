cmake_minimum_required(VERSION 3.10)
project(Tutorial)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)
add_executable(Tutorial WIN32 tutorial.cxx)