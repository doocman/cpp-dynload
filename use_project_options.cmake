
include(FetchContent)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

# Add project_options from https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
set(DYNLOAD_PROJECT_OPTIONS_VERSION "v0.29.0")
FetchContent_Declare(
        _project_options
        URL https://github.com/aminya/project_options/archive/refs/tags/${DYNLOAD_PROJECT_OPTIONS_VERSION}.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# Enable sanitizers and static analyzers when running the tests
set(ENABLE_CLANG_TIDY OFF)
set(ENABLE_CPPCHECK OFF)
set(ENABLE_SANITIZER_ADDRESS OFF)
set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR OFF)
set(ENABLE_COVERAGE ENABLE_COVERAGE)
set(ENABLE_DOXYGEN OFF)
set(ENABLE_CACHE OFF)

project_options(
        PREFIX "dynload"
        ${ENABLE_CACHE}
        ${ENABLE_CPPCHECK}
        ${ENABLE_CLANG_TIDY}
        ENABLE_VS_ANALYSIS
        # ENABLE_CONAN
        # ENABLE_INTERPROCEDURAL_OPTIMIZATION
        # ENABLE_NATIVE_OPTIMIZATION
        ${ENABLE_DOXYGEN}
        ${ENABLE_COVERAGE}
        ${ENABLE_SANITIZER_ADDRESS}
        ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
        # ENABLE_SANITIZER_THREAD
        # ENABLE_SANITIZER_MEMORY
        # ENABLE_COMPILE_COMMANDS_SYMLINK
        # ENABLE_PCH
        # PCH_HEADERS
        # WARNINGS_AS_ERRORS
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_GCC_ANALYZER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
        # LINKER "lld"
        # CONAN_PROFILE ${profile_path}  # passes a profile to conan: see https://docs.conan.io/en/latest/reference/profiles.html
)
