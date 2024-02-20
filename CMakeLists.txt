cmake_minimum_required(VERSION 3.12)
project(omega)

if (WIN32 AND MSVC)
    add_compile_options(/W4)
elseif (WIN32)
    add_compile_options(-municode -Wall -Wextra -Wpedantic -lucrtbased)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -fno-stack-protector -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-translation-unit)
endif ()

include_directories("include")
file(GLOB omega_src CONFIGURE_DEPENDS "src/*.c")

add_definitions(-DCMAKE_BUILD -DOMG_STATIC)
add_executable(omgapp "main.c" ${omega_src})
target_link_libraries(omgapp m)
