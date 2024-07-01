
cmake_minimum_required(VERSION 3.22...3.25)

project(DynamicLoad LANGUAGES CXX)

set(DYNLOAD_DEVELOP OFF CACHE BOOL "Turn ON during library development to get proper compiler warnings and testing")
set(DYNLOAD_USE_EXCEPTIONS ON CACHE BOOL "Standard C++ uses exceptions, thus, this is enabled by default. Turn off if no exceptions are allowed.")
set(DYNLOAD_IMPORT ON CACHE BOOL "Set to true if needing importing part of DYNLOAD")
set(DYNLOAD_EXPORT ON CACHE BOOL "Set to true if needing exporting part of DYNLOAD")
set(DYNLOAD_COMPILE_PLATFORM ON CACHE BOOL "If true, dynl will be automatically compiled for the target platform. Set to false if custom backend is needed.")
set(DYNLOAD_HEADERONLY OFF CACHE BOOL "Set to true to use dynload as a header-only library")

add_library(dynl_compile_opts INTERFACE)
add_library(dynl::compile_opts ALIAS dynl_compile_opts)
target_compile_options(dynl_compile_opts INTERFACE ${DYNL_COMPILE_FLAGS})

set(DYNLOAD_DEV_LINK dynl_compile_opts)
if (DYNLOAD_DEVELOP)
    set(CMAKE_CXX_STANDARD 11)
    enable_testing()
    include(use_google_test.cmake)
    add_subdirectory(tests)
endif ()

if (DYNLOAD_IMPORT)
    add_library(dynl_headers INTERFACE)
    add_library(dynl::dynl_headers ALIAS dynl_headers)
    target_include_directories(dynl_headers INTERFACE
            include
    )
    target_compile_features(dynl_headers INTERFACE cxx_std_20)

    if (DYNLOAD_USE_EXCEPTIONS)
    else ()
        target_compile_definitions(dynl_shared PRIVATE
                "DYNLOAD_NO_EXCEPTIONS=1"
        )
    endif ()

    if (DYNLOAD_HEADERONLY)
        set(DYNLOAD_LIB_TYPE INTERFACE)
        set(DYNLOAD_PUBLIC_LINK_TYPE INTERFACE)
    else ()
        set(DYNLOAD_LIB_TYPE "")
        set(DYNLOAD_PUBLIC_LINK_TYPE PUBLIC)
    endif ()

    add_library(dynl ${DYNLOAD_LIB_TYPE})
    add_library(dynl::dynl ALIAS dynl)
    target_link_libraries(dynl ${DYNLOAD_PUBLIC_LINK_TYPE}
            dynl::dynl_headers
            ${DYNLOAD_DEV_LINK}
    )
    if (DYNLOAD_COMPILE_PLATFORM)
        if (WIN32)
            set(DYNL_SRC src/dynl_win32.cpp)
            set(DYNL_PLATFORM DYNL_WINDOWS=1)
            set(DYNL_EXTRA_LINK "")
        elseif (UNIX)
            set(DYNL_SRC src/dynl_unix.cpp)
            set(DYNL_PLATFORM DYNL_UNIX=1)
            set(DYNL_EXTRA_LINK dl)
        else ()
            message(SEND_ERROR "Dynload: no automatic backend available! Use custom defined source file")
        endif ()
    else ()
        message(STATUS "Dynload: no automatic backend. Custom defined source-file(-s) for 'dynl' required")
        set(DYNL_SRC "")
        set(DYNL_PLATFORM "")
    endif ()
    if (DYNLOAD_HEADERONLY)
        target_compile_definitions(dynl INTERFACE DYNLOAD_HEADERONLY=1 ${DYNL_PLATFORM})
        target_link_libraries(dynl INTERFACE ${DYNL_EXTRA_LINK})
    else ()
        target_sources(dynl PRIVATE ${DYNL_SRC})
        target_link_libraries(dynl PRIVATE ${DYNL_EXTRA_LINK})
    endif ()
endif ()
if (DYNLOAD_EXPORT)
    add_library(dynl_export INTERFACE)
    add_library(dynl::dynl_export ALIAS dynl_export)
    target_include_directories(dynl_export INTERFACE
            export_include
    )
endif ()
