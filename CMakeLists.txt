cmake_minimum_required(VERSION 3.26)
project(BibbleBytecode VERSION 1.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
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
    $<INSTALL_INTERFACE:include>
)

target_include_directories(BibbleBytecode-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(BibbleBytecode-shared PUBLIC cxx_std_20)
target_compile_features(BibbleBytecode-static PUBLIC cxx_std_20)

set_target_properties(BibbleBytecode-shared PROPERTIES
    EXPORT_NAME Bytecode
    OUTPUT_NAME "bibble-bytecode"
    VERSION 1.0.0
    SOVERSION 1
)
set_target_properties(BibbleBytecode-static PROPERTIES
    EXPORT_NAME BytecodeStatic
    OUTPUT_NAME "bibble-bytecode-aio"
    VERSION 1.0.0
)

configure_package_config_file(
    cmake/BibbleBytecodeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BibbleBytecodeConfig.cmake
    INSTALL_DESTINATION lib/cmake/BibbleBytecode
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/BibbleBytecodeConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    TARGETS BibbleBytecode-shared BibbleBytecode-static
    EXPORT BibbleBytecodeTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(
    DIRECTORY include/
    DESTINATION include
)

install(
    EXPORT BibbleBytecodeTargets
    FILE BibbleBytecodeTargets.cmake
    NAMESPACE Bibble::
    DESTINATION lib/cmake/BibbleBytecode
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/BibbleBytecodeConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/BibbleBytecodeConfigVersion.cmake
    DESTINATION lib/cmake/BibbleBytecode
)