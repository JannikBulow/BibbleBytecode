cmake_minimum_required(VERSION 3.26)
project(BibbleBytecode)

include(GenerateExportHeader)

set(SOURCES
    src/buffer.cpp
    src/reader.cpp
    src/writer.cpp
)

set(HEADERS
    include/BibbleBytecode/module/class.h
    include/BibbleBytecode/module/const_pool.h
    include/BibbleBytecode/module/field.h
    include/BibbleBytecode/module/function.h
    include/BibbleBytecode/module/method.h
    include/BibbleBytecode/module/module.h

    include/BibbleBytecode/_compatability.h
    include/BibbleBytecode/allocator_view.h
    include/BibbleBytecode/buffer.h
    include/BibbleBytecode/reader.h
    include/BibbleBytecode/writer.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(BibbleBytecode-shared SHARED ${SOURCES} ${HEADERS})
add_library(BibbleBytecode-static STATIC ${SOURCES} ${HEADERS})

generate_export_header(BibbleBytecode-shared
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/BibbleBytecode/api.h
    EXPORT_MACRO_NAME BIBBLEBYTECODE_EXPORT
    NO_EXPORT_MACRO_NAME BIBBLEBYTECODE_NO_EXPORT
    STATIC_DEFINE BIBBLEBYTECODE_STATIC_DEFINE
)

target_compile_definitions(BibbleBytecode-static PUBLIC BIBBLEBYTECODE_STATIC_DEFINE)

target_include_directories(BibbleBytecode-shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories(BibbleBytecode-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(BibbleBytecode-shared PUBLIC cxx_std_20)
target_compile_features(BibbleBytecode-static PUBLIC cxx_std_20)