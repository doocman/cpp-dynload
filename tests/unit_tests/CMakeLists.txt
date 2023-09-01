
enable_testing()

add_executable(dynl.tests.unit_tests.dynl_error src/dynl_error.ut.cpp)
target_link_libraries(dynl.tests.unit_tests.dynl_error PRIVATE
        gmock_main
        dynl::dynl
)

