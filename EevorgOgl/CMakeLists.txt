cmake_minimum_required(VERSION 3.10)
project(EevorgOgl_CMake)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)
add_executable(
    EevorgOgl_CMake WIN32     
    EevorgOglApp.cpp
    EevorgDoc.cpp 
    Eevorg.cpp    
    EevorgRenderer.cpp
    EevorgView.cpp 
    EevorgViewGl.cpp
    EevorgWnd.rc
    MainFrm.cpp
    stdafx.cpp
)