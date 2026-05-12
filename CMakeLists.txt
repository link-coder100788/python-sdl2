cmake_minimum_required(VERSION 4.3)
project(python_sdl2)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_PYTHON_MODULE "Build the python module." ON)
option(USE_HOMEBREW_OPENAL "Use OpenAL-soft header layout." OFF)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(SDL2 REQUIRED)
find_package(OpenAL REQUIRED)

message(STATUS "Python executable: ${Python3_EXECUTABLE}")
message(STATUS "Python include: ${Python3_INCLUDE_DIRS}")
message(STATUS "OpenAL: ${OPENAL_INCLUDE_DIR}")

set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM dir: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(reflect_tool
        tools/reflector.cpp
)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

find_library(CLANG_LIB NAMES clang libclang HINTS ${LLVM_LIBRARY_DIRS})

target_link_libraries(reflect_tool PRIVATE ${CLANG_LIB})

set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
        OUTPUT
        ${GENERATED_DIR}/generated_reflect.c
        ${GENERATED_DIR}/generated_reflect.h

        COMMAND reflect_tool
        ${CMAKE_SOURCE_DIR}/src/core/PyCore.h
        ${GENERATED_DIR}

        DEPENDS
        reflect_tool
        ${CMAKE_SOURCE_DIR}/src/core/PyCore.h
)

add_library(python_sdl2 MODULE
        src/bindings/python/python_module.c
        src/core/PyCore.cpp
        src/core/PyCore.h

        ${GENERATED_DIR}/generated_reflect.c
)

if(USE_HOMEBREW_OPENAL)
    target_compile_definitions(python_sdl2 PRIVATE -DUSING_OPENAL_SOFT)
    target_include_directories(python_sdl2 PRIVATE "${OPENAL_INCLUDE_DIR}/AL")
endif()

execute_process(
        COMMAND ${Python3_EXECUTABLE} -c
        "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))"
        OUTPUT_VARIABLE PY_EXT_SUFFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Manual Python suffix: ${PY_EXT_SUFFIX}")

set_target_properties(python_sdl2 PROPERTIES
        PREFIX ""
        SUFFIX "${PY_EXT_SUFFIX}"
)

target_include_directories(python_sdl2 PRIVATE
        ${CMAKE_SOURCE_DIR}/src/core
        ${GENERATED_DIR}
        ${Python3_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(python_sdl2 PRIVATE
        ${Python3_LIBRARIES}
        SDL2::SDL2
        ${OPENAL_LIBRARY}
)

target_compile_options(python_sdl2 PRIVATE -Wno-deprecated-declarations)
target_compile_options(python_sdl2 PRIVATE -Werror)

find_package(Ruby REQUIRED)

message(STATUS "Ruby Executable: ${Ruby_EXECUTABLE}")
message(STATUS "Ruby Version: ${Ruby_VERSION}")
message(STATUS "Ruby Include: ${Ruby_INCLUDE_DIRS}")

add_library(ruby_sdl2 MODULE
        src/bindings/ruby/ruby_module.c
        src/core/RbCore.cpp
        src/core/RbCore.h
)

if(USE_HOMEBREW_OPENAL)
    target_compile_definitions(ruby_sdl2 PRIVATE -DUSING_OPENAL_SOFT)
    target_include_directories(ruby_sdl2 PRIVATE "${OPENAL_INCLUDE_DIR}/AL")
endif()

set_target_properties(ruby_sdl2 PROPERTIES
        PREFIX ""
        OUTPUT_NAME "ruby_sdl2"
        SUFFIX ".bundle"
)

target_include_directories(ruby_sdl2 PRIVATE
        ${CMAKE_SOURCE_DIR}/src/core
        ${Ruby_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(ruby_sdl2 PRIVATE
        ${Ruby_LIBRARIES}
        SDL2::SDL2
        ${OPENAL_LIBRARY}
)

target_compile_options(ruby_sdl2 PRIVATE -Wno-deprecated-declarations)
target_compile_options(ruby_sdl2 PRIVATE -Werror)

execute_process(
        COMMAND /usr/libexec/java_home
        OUTPUT_VARIABLE JAVA_HOME
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT JAVA_HOME)
    message(FATAL_ERROR "JAVA_HOME not found")
endif()

set(JNI_INCLUDE_DIRS
        ${JAVA_HOME}/include
        ${JAVA_HOME}/include/darwin
)

message(STATUS "JNI PATH: ${JNI_INCLUDE_DIRS}")

add_library(java_sdl2 SHARED
        src/bindings/java/native/JCore.cpp
        src/bindings/java/native/JCore.h
        src/bindings/java/native/java_module.c
        src/bindings/java/native/dev_linkcoder100788_java_sdl2_JavaSDL2.h
        src/bindings/java/native/dev_linkcoder100788_java_sdl2_JavaSDL2_Window.h
        src/bindings/java/native/dev_linkcoder100788_java_sdl2_JavaSDL2_Sprite.h
        src/bindings/java/native/dev_linkcoder100788_java_sdl2_JavaSDL2_OpenALPlayer.h
)

if(USE_HOMEBREW_OPENAL)
    target_compile_definitions(java_sdl2 PRIVATE -DUSING_OPENAL_SOFT)
    target_include_directories(java_sdl2 PRIVATE "${OPENAL_INCLUDE_DIR}/AL")
endif()

set(JAVA_SRC
        ${CMAKE_SOURCE_DIR}/src/bindings/java/dev/linkcoder100788/java_sdl2/JavaSDL2.java
)

set(NATIVE_DIR
        ${CMAKE_SOURCE_DIR}/src/bindings/java/native
)

add_custom_command(
        OUTPUT ${NATIVE_DIR}/dev_linkcoder100788_java_1sdl2_JavaSDL2.h
        COMMAND javac
        -h ${NATIVE_DIR}
        ${JAVA_SRC}
        DEPENDS ${JAVA_SRC}
        COMMENT "Generating JNI headers"
)

add_custom_target(jni_headers
        DEPENDS ${NATIVE_DIR}/dev_linkcoder100788_java_1sdl2_JavaSDL2.h
)

target_include_directories(java_sdl2 PRIVATE
        ${JNI_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(java_sdl2 PRIVATE
        SDL2::SDL2
        ${OPENAL_LIBRARY}
)

target_compile_options(java_sdl2 PRIVATE -Wno-deprecated-declarations)
target_compile_options(java_sdl2 PRIVATE -Werror)

set(JAR_OUTPUT
        ${CMAKE_BINARY_DIR}/JavaSDL2.jar
)

set(JAVA_BUILD_DIR
        ${CMAKE_BINARY_DIR}/java_classes
)

add_custom_target(java_compile ALL
        COMMAND javac
        -d ${JAVA_BUILD_DIR}
        ${JAVA_SRC}
        BYPRODUCTS
        ${JAVA_BUILD_DIR}/dev/linkcoder100788/java_sdl2/JavaSDL2.class
        ${JAVA_BUILD_DIR}/dev/linkcoder100788/java_sdl2/JavaSDL2$Window.class
)

add_custom_target(java_jar ALL
        COMMAND jar cf ${JAR_OUTPUT} -C ${JAVA_BUILD_DIR} .
        DEPENDS java_compile
)