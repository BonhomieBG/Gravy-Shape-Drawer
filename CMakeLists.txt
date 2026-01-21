cmake_minimum_required(VERSION 3.16)

project (GravyShapeDrawer VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets)

qt_standard_project_setup()

file(GLOB ROOT_SOURCES "*.cpp")
file(GLOB ROOT_HEADERS "*.h")
file(GLOB GUI_SOURCES "GUI/*.cpp")
file(GLOB GUI_HEADERS "GUI/*.h")

qt_add_executable(GravyShapeDrawer WIN32 ${ROOT_SOURCES} ${ROOT_HEADERS} ${GUI_SOURCES} ${GUI_HEADERS})

target_link_libraries(GravyShapeDrawer PRIVATE Qt6::Widgets)

add_custom_command(TARGET GravyShapeDrawer POST_BUILD
    COMMAND mkdir App
    COMMAND move GravyShapeDrawer.exe App
    COMMAND cd App
    COMMAND windeployqt6 GravyShapeDrawer.exe
)