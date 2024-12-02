option(SIMULATOR "Build simulator binary" OFF)
if (SIMULATOR)
    cmake_minimum_required(VERSION 3.10)
    project(lvgl)
    set(CMAKE_C_STANDARD 11)#C11
    set(CMAKE_CXX_STANDARD 17)#C17
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

    file(GLOB_RECURSE INCLUDES "azan_clock.h" "lv_drivers/*.h" "lvgl/*.h" )
    file(GLOB_RECURSE SOURCES "azan_clock.c" "ui/*.c" "fonts/*.c" "lv_drivers/*.c" "lvgl/*.c")
    add_subdirectory(ui)

    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
    SET(CMAKE_CXX_FLAGS "-O3")

    find_package(SDL2 REQUIRED SDL2)
    include_directories(${SDL2_INCLUDE_DIRS})
    add_executable(main main_simulator.c mouse_cursor_icon.c ${SOURCES} ${INCLUDES})
    add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
    target_link_libraries(main PRIVATE SDL2 )
    add_compile_definitions(SIMULATOR)
    add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)
else()

    file(READ "../main/ui/filelist.txt" file_content)
    string(REGEX REPLACE "\n" ";" UI_SOURCES "${file_content}")
    list(TRANSFORM UI_SOURCES PREPEND "ui/")

    idf_component_register(
        SRCS "waveshare_rgb_lcd_port.c" "lvgl_port.c" "main.c" "azan_clock.c" ${UI_SOURCES}
        INCLUDE_DIRS ".")

    idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
    target_compile_options(${lvgl_lib} PRIVATE -Wno-format)
endif()