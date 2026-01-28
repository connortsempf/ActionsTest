#######################
#### Project Setup ####
#######################

## CMake Version Requirement ##
cmake_minimum_required(VERSION 3.16)

## Project Definition ##
project(TestApp VERSION 1.0.0 LANGUAGES CXX)

## C++ Language Version ##
set(CMAKE_CXX_STANDARD 20)

## Require C++ Language Version ##
set(CMAKE_CXX_STANDARD_REQUIRED ON)




####################
#### Qt Package ####
####################

## Enable Qt MOC ##
set(CMAKE_AUTOMOC ON)

## Enable Qt UIC ##
set(CMAKE_AUTOUIC ON)

## Enable Qt RCC ##
set(CMAKE_AUTORCC ON)

## MinGW RC Compiler Setup for Windows RC File in Build ##
if(MINGW)
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

## Get the Qt Package ##
option(QT6_DIR "Path to Local Qt Compiler Installation" "")
if(QT6_DIR)
    find_package(Qt6 REQUIRED COMPONENTS Widgets PATHS "${QT6_DIR}" NO_DEFAULT_PATH)
else()
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    if(NOT Qt6_FOUND)
        message(FATAL_ERROR "Qt6 not found!

        Please install Qt6 development libraries:

        Windows:
        Download from https://www.qt.io/download

        macOS:
        brew install qt6

        Linux (Ubuntu/Debian):
        sudo apt install qt6-base-dev qt6-tools-dev

        Or set QT6_DIR to your Qt compiler-specific libraries directory:
        cmake .. -DQT6_DIR=\"/path/to/your/Qt/package/6.10.x/compiler/\"")
    endif()
endif()




######################
#### Build Source ####
######################

## Add Source Files ##
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/main.cpp")

## Set the Output Binary Files Directory ##
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Set the Output Binary Files Directory for Multi-Config Outputs ##
foreach(CONFIG Release Debug RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_SOURCE_DIR}/bin/${CONFIG}")
endforeach()

## Generate OS-Specific 2048 Application Executable ##
add_executable(${PROJECT_NAME} ${SOURCES})

## Link to Qt Framework ##
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)

## Output Binaries to Bin Directory ##
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt REQUIRED)
    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #     COMMAND "${WINDEPLOYQT_EXECUTABLE}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe"
    #     COMMENT "Deploying Qt Dependencies to Output Directory..."
    # )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Deploying Qt Dependencies to Output Directory..."
    )
endif()




###############################
#### Deploy to OS Platform ####
###############################

## Windows Application Build ##
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/platform/windows/TestApp.rc")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()

## macOS Application Build ##
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.connorsempf.TestApp"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE "TestApp.icns"
    )
    set_source_files_properties("${CMAKE_SOURCE_DIR}/platform/macOS/TestApp.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/platform/macOS/TestApp.icns")
endif()

## Linux Application Build ##
if(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/TestApp.desktop" DESTINATION share/applications)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/TestApp.png" DESTINATION share/pixmaps)
endif()
