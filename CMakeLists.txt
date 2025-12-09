cmake_minimum_required(VERSION 3.20..3.30)
project("DeadDev Bitmask" LANGUAGES CXX VERSION 1.0.0)
cmake_policy(SET CMP0135 NEW)

message(STATUS "${CMAKE_PROJECT_NAME} v${CMAKE_PROJECT_VERSION}")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(DEADDEV_BITMASK_IS_TOP_LEVEL_PROJECT TRUE)
else(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(DEADDEV_BITMASK_IS_TOP_LEVEL_PROJECT FALSE)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})


option(DEADDEV_BITMASK_BUILD_EXAMPLES "build examples" ${DEADDEV_BITMASK_IS_TOP_LEVEL_PROJECT})
option(DEADDEV_BITMASK_BUILD_TESTS "build tests" ${DEADDEV_BITMASK_IS_TOP_LEVEL_PROJECT})
option(DEADDEV_BITMASK_GENERATE_DOCS "generate documentation using Doxygen" ${DEADDEV_BITMASK_IS_TOP_LEVEL_PROJECT})
option(DEADDEV_BITMASK_INSTALL "install library" ON)

add_library(deaddevbitmask INTERFACE)
target_include_directories(
    deaddevbitmask INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_library(deaddev::bitmask ALIAS deaddevbitmask)

if(DEADDEV_BITMASK_INSTALL)
    include(GNUInstallDirs)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/deaddev/bitmask.hpp DESTINATION include/deaddev/)
endif(DEADDEV_BITMASK_INSTALL)

if(DEADDEV_BITMASK_GENERATE_DOCS)
    find_package(Doxygen)
    if(Doxygen_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        add_custom_target(deaddev_doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Docs"
                VERBATIM)
    else(Doxygen_FOUND)
        message(STATUS "Doxygen not found!")
    endif(Doxygen_FOUND)
endif(DEADDEV_BITMASK_GENERATE_DOCS)


if(DEADDEV_BITMASK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(DEADDEV_BITMASK_BUILD_TESTS)

if(DEADDEV_BITMASK_BUILD_EXAMPLES)
    add_subdirectory(example)
endif(DEADDEV_BITMASK_BUILD_EXAMPLES)

