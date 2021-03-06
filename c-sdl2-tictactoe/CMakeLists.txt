cmake_minimum_required(VERSION 2.8)

project(tictactoe)

if (WIN32)
    set(CMAKE_CXX_STANDARD 11)
endif()

# Set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Set source and include folders
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src/")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include/")


if (WIN32)

    set(WIN32_PATH "${CMAKE_SOURCE_DIR}/win32")

    # SDL2
    set(SDL2_PATH "${WIN32_PATH}/SDL2")
    set(SDL2_INCLUDE "${SDL2_PATH}/include")
    set(SDL2_LIB "${SDL2_PATH}/lib/libSDL2.dll.a")

    # SDL2 ttf
    set(SDL2_TTF_PATH "${WIN32_PATH}/SDL2_ttf")
    set(SDL2_TTF_INCLUDE "${SDL2_TTF_PATH}/include")
    set(SDL2_TTF_LIB "${SDL2_TTF_PATH}/lib/libSDL2_ttf.dll.a")

    include_directories(${INC_DIR}
                        ${SDL2_INCLUDE}
                        ${SDL2_TTF_INCLUDE})

else()

    # Find packages
    find_package(SDL2 REQUIRED)
    find_package(SDL2_IMAGE REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    # Set include directories
    include_directories(${INC_DIR}
                        ${SDL2_INCLUDE_DIRS}
                        #${SDL2_IMAGE_INCLUDE_DIRS}
                        ${SDL2_TTF_INCLUDE_DIRS})
endif()

# Set include files
set(INCLUDES
    ${INC_DIR}/internal.h
    ${INC_DIR}/tictactoe.h
    ${INC_DIR}/menu.h
    ${INC_DIR}/color.h
    ${INC_DIR}/board.h
    ${INC_DIR}/shape.h
    ${INC_DIR}/engine.h
    ${INC_DIR}/endmenu.h
    ${INC_DIR}/util.h)

# Set source files
set(SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/tictactoe.c
    ${SRC_DIR}/menu.c
    ${SRC_DIR}/color.c
    ${SRC_DIR}/board.c
    ${SRC_DIR}/shape.c
    ${SRC_DIR}/engine.c
    ${SRC_DIR}/endmenu.c
    ${SRC_DIR}/util.c)

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIB} ${SDL2_TTF_LIB})
else()
    #target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} m)
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} m)
endif()
