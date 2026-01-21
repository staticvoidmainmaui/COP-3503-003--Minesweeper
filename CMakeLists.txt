cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project3)

## If you want to link SFML statically, uncomment the line below
set(SFML_STATIC_LIBRARIES TRUE)

## Specify the path to your SFML directory (adjust if needed)
set(SFML_DIR "C:/Users/alejm/Downloads/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")

## Find SFML components
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

## Add your source files
add_executable(project3
        main.cpp
        WelcomeWindow.cpp
        LeaderboardWindow.cpp
        GameWindow.cpp
)

## Link SFML libraries
target_link_libraries(project3
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)
