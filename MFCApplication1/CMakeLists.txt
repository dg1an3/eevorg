cmake_minimum_required(VERSION 3.10)
project(MFCApp1)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)
add_executable(
    MFCApp1 WIN32 
    MFCApplication1.cpp 
    MFCApplication1Doc.cpp 
    MFCApplication1View.cpp 
    MFCApplication1.rc
    MainFrm.cpp
    ClassView.cpp 
    FileView.cpp 
    OutputWnd.cpp 
    PropertiesWnd.cpp 
    ViewTree.cpp
    pch.cpp
)