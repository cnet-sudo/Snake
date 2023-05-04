cmake_minimum_required(VERSION 3.21)
project(Sneke VERSION 0.0.1 LANGUAGES CXX)

add_executable(Sneke MyNcurses.cpp)

target_link_libraries(Sneke PRIVATE pdcurses)

target_compile_features(Sneke PRIVATE cxx_std_20)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Sneke POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Sneke> $<TARGET_FILE_DIR:Sneke> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS Sneke)

# Директория с СMakeLists.txt
# cmake -S . -B build
# cmake --build build --config Release
# cmake --build build --config Debug
