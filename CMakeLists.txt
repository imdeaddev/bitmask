cmake_minimum_required(VERSION 3.23..3.30)
project("deaddev-bitmask" LANGUAGES CXX VERSION 1.0.1)
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

add_library(bitmask INTERFACE)
target_include_directories(
    bitmask INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_library(deaddev::bitmask ALIAS bitmask)

if(DEADDEV_BITMASK_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
    install(TARGETS bitmask EXPORT ${PROJECT_NAME}-targets)
    install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE deaddev::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    configure_package_config_file(
        cmake/deaddev-bitmask-config.cmake.in
        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    install(DIRECTORY include/deaddev
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.hpp")
endif(DEADDEV_BITMASK_INSTALL)

if(DEADDEV_BITMASK_GENERATE_DOCS)
    find_package(Doxygen)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    if(Doxygen_FOUND)
        add_custom_target(deaddev_doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
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

find_package(deaddev-bitmask)
