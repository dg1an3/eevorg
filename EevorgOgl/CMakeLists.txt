cmake_minimum_required(VERSION 3.31)

# Set project name and versions
project(EevorgOgl VERSION 1.0.0 LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set up for MFC
set(CMAKE_MFC_FLAG 1)
add_definitions(-D_AFXDLL)

# Find OpenGL package
find_package(OpenGL REQUIRED)

set(SOURCES
    EevorgOglApp.cpp
    EevorgDoc.cpp 
    Eevorg.cpp    
    EevorgRenderer.cpp
    EevorgView.cpp 
    EevorgViewGl.cpp
    MainFrm.cpp
    stdafx.cpp
)

set(HEADERS
    Eevorg.h
    EevorgDoc.h
    EevorgRenderer.h
    EevorgView.h
    EevorgViewGl.h
    valimage.h
    stdafx.h
)

set(RESOURCES
    EevorgWnd.rc
)

add_executable(
    ${PROJECT_NAME} WIN32
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENGL_LIBRARIES}
)

# Verify OpenGL library paths (prints during configuration)
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
message(STATUS "OpenGL include dir: ${OPENGL_INCLUDE_DIR}")
