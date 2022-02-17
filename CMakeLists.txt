cmake_minimum_required(VERSION 3.10)

# set the project name
project(BlindKwartet)

set(fontfile "font.hex")
configure_file (${fontfile} ${fontfile} COPYONLY)

set(sources
    window.h
    window.cpp
    windowStack.h
    windowStack.cpp
    windowSize.h
    colors.h
    colors.cpp
    Font.h
    Font.cpp
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
    Kwartet.h
    Kwartet.cpp
    InputInterface.h
)

# add the executable
add_executable(BlindKwartet
    main.cpp
    Input.h
    Input.cpp
    ${sources}
)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(BlindKwartet glfw pthread ${OPENGL_LIBRARIES})
include_directories (/usr/include/freetype2)

ENABLE_TESTING()
ADD_EXECUTABLE( test_kwartetter
    test.cpp
    TestInput.h
    TestInput.cpp
    ${sources}
)
target_link_libraries(test_kwartetter glfw pthread ${OPENGL_LIBRARIES})

ADD_TEST(test test_kwartetter)

