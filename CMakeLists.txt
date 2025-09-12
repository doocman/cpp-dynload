
cmake_minimum_required(VERSION 3.22...4.2)

set(DYNLOAD_VERSION 0.3)

project(DynamicLoad LANGUAGES CXX VERSION ${DYNLOAD_VERSION})

set(DYNLOAD_DEVELOP ${PROJECT_IS_TOP_LEVEL} CACHE BOOL "Turn ON during library development to get proper compiler warnings and testing")
set(DYNLOAD_USE_EXCEPTIONS ON CACHE BOOL "Standard C++ uses exceptions, thus, this is enabled by default. Turn off if no exceptions are allowed.")
set(DYNLOAD_IMPORT ON CACHE BOOL "Set to true if needing importing part of DYNLOAD")
set(DYNLOAD_EXPORT ON CACHE BOOL "Set to true if needing exporting part of DYNLOAD")
set(DYNLOAD_COMPILE_PLATFORM ON CACHE BOOL "If true, dynl will be automatically compiled for the target platform. Set to false if custom backend is needed.")
set(DYNLOAD_HEADERONLY OFF CACHE BOOL "Set to true to use dynload as a header-only library")

if (DYNLOAD_DEVELOP)
    set(CMAKE_CXX_STANDARD 20)
    enable_testing()
    include(use_google_test.cmake)
    add_subdirectory(tests)

    set(DYNLOAD_INTERNAL_COMPILE_OPTS ${DYNL_COMPILE_FLAGS})
endif ()


if (DYNLOAD_HEADERONLY)
    set(DYNLOAD_LIB_TYPE INTERFACE)
    set(DYNLOAD_PUBLIC_LINK_TYPE INTERFACE)
else ()
    set(DYNLOAD_LIB_TYPE "")
    set(DYNLOAD_PUBLIC_LINK_TYPE PUBLIC)
endif ()

add_library(dynl)
add_library(dynl::dynl ALIAS dynl)
target_compile_options(dynl PUBLIC
        ${DYNLOAD_INTERNAL_COMPILE_OPTS}
)
target_compile_features(dynl PUBLIC cxx_std_20)
add_library(dynl_headers INTERFACE)
add_library(dynl::dynl_headers ALIAS dynl_headers)
target_compile_options(dynl INTERFACE
        ${DYNLOAD_INTERNAL_COMPILE_OPTS}
)
target_compile_features(dynl_headers INTERFACE cxx_std_20)

target_include_directories(dynl PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_include_directories(dynl_headers INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if (NOT DYNLOAD_USE_EXCEPTIONS)
    target_compile_definitions(dynl PUBLIC
            "DYNLOAD_NO_EXCEPTIONS=1"
    )
    target_compile_definitions(dynl_headers INTERFACE
            "DYNLOAD_NO_EXCEPTIONS=1"
    )
endif ()
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
target_compile_definitions(dynl_headers INTERFACE DYNLOAD_HEADERONLY=1 ${DYNL_PLATFORM})
target_link_libraries(dynl_headers INTERFACE ${DYNL_EXTRA_LINK})

target_sources(dynl PRIVATE ${DYNL_SRC})
target_link_libraries(dynl PRIVATE ${DYNL_EXTRA_LINK})

add_library(dynl_export INTERFACE)
add_library(dynl::dynl_export ALIAS dynl_export)
target_include_directories(dynl_export INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/DynloadConfigVersion.cmake"
        VERSION ${DYNLOAD_VERSION}
        COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/DynloadConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/DynloadConfig.cmake"
        INSTALL_DESTINATION lib/cmake/Dynload
)

install(TARGETS dynl dynl_headers dynl_export
        EXPORT dynlTargets
        LIBRARY DESTINATION lib COMPONENT Runtime
        ARCHIVE DESTINATION lib COMPONENT Development
        RUNTIME DESTINATION bin COMPONENT Runtime
        PUBLIC_HEADER DESTINATION include COMPONENT Development
        BUNDLE DESTINATION bin COMPONENT Runtime
)

install(EXPORT dynlTargets DESTINATION lib/cmake/Dynload)
install(FILES "${PROJECT_BINARY_DIR}/DynloadConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/DynloadConfig.cmake"
        DESTINATION lib/cmake/Dynload)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

