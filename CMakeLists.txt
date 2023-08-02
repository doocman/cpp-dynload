
cmake_minimum_required(VERSION 3.22...3.25)

project(DynamicLoad LANGUAGES CXX)

set(DYNLOAD_DEVELOP OFF CACHE BOOL "Turn ON during library development to get proper compiler warnings and testing")
set(DYNLOAD_USE_EXCEPTIONS ON CACHE BOOL "Standard C++ uses exceptions, thus, this is enabled by default. Turn off if no exceptions are allowed.")
set(DYNLOAD_IMPORT ON CACHE BOOL "Set to true if needing importing part of DYNLOAD")
set(DYNLOAD_EXPORT ON CACHE BOOL "Set to true if needing exporting part of DYNLOAD")
set(DYNLOAD_COMPILE_PLATFORM ON CACHE BOOL "If true, dynl will be automatically compiled for the target platform. Set to false if custom backend is needed.")

set(DYNLOAD_DEV_LINK "")

if (DYNLOAD_DEVELOP)
    set(CMAKE_CXX_STANDARD 11)
    enable_testing()
    include(use_project_options.cmake)
    include(use_google_test.cmake)
    set(DYNLOAD_DEV_LINK PRIVATE dynload_project_options dynload_project_warnings)
    add_subdirectory(tests)
endif ()

if (DYNLOAD_IMPORT)
    add_library(dynl_headers INTERFACE)
    add_library(dynl::dynl_headers ALIAS dynl_headers)
    target_include_directories(dynl_headers INTERFACE
            include
            )
    #set_property(TARGET dynl_headers PROPERTY CXX_STANDARD 20)
    target_compile_features(dynl_headers INTERFACE cxx_std_20)

    add_library(dynl_shared)
    add_library(dynl::dynl_shared ALIAS dynl_shared)
    target_link_libraries(dynl_shared PUBLIC
            dynl::dynl_headers
            ${DYNLOAD_DEV_LINK}
            )
    target_sources(dynl_shared PRIVATE
            src/dynl_shared.cpp
            )
    if (DYNLOAD_USE_EXCEPTIONS)
    else ()
        target_compile_definitions(dynl_shared PRIVATE
                "DYNLOAD_NO_EXCEPTIONS=1"
                )
    endif ()


    add_library(dynl)
    add_library(dynl::dynl ALIAS dynl)
    target_link_libraries(dynl PUBLIC
            dynl::dynl_shared
            ${DYNLOAD_DEV_LINK}
            )
    if (DYNLOAD_COMPILE_PLATFORM)
        if (WIN32)
            target_sources(dynl PRIVATE
                    src/dynl_win32.cpp
                    )
        elseif (UNIX)
            target_sources(dynl PRIVATE
                    src/dynl_unix.cpp
                    )
        else ()
            message(SEND_ERROR "Dynload: no automatic backend available! Use custom defined source file")
        endif ()
    else ()
        message(STATUS "Dynload: no automatic backend. Custom defined source-file(-s) for 'dynl' required")
    endif ()
endif ()
if (DYNLOAD_EXPORT)
    add_library(dynl_export INTERFACE)
    add_library(dynl::dynl_export ALIAS dynl_export)
    target_include_directories(dynl_export INTERFACE
            export_include
            )
endif ()
