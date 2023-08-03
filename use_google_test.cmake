
include(FetchContent)

set(DYNLOAD_GTEST_VERSION "v1.14.0")

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/${DYNLOAD_GTEST_VERSION}.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

