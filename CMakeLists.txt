cmake_minimum_required(VERSION 3.10)

# set the project name
project(BlindKwartet)

# add the executable
add_executable(BlindKwartet
    main.cpp
    window.h
    window.cpp
    windowStack.h
    windowStack.cpp
    windowSize.h
    colors.h
    colors.cpp
    textRender.h
    textRender.cpp
    characters.h
    Kwartetter.h
    Kwartetter.cpp
    drawable.h
    MainScreen.h
    MainScreen.cpp
    Player.h
    Player.cpp
    CardBase.h
    CardBase.cpp
    Card.h
    Card.cpp
    Cards.h
    Cards.cpp
    )

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(BlindKwartet glfw pthread ${OPENGL_LIBRARIES})
include_directories (/usr/include/freetype2)
