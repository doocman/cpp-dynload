
enable_testing()

add_executable(dynl.tests.unit_tests.dynl_error src/dynl_error.ut.cpp)
target_link_libraries(dynl.tests.unit_tests.dynl_error PRIVATE
        gmock_main
        dynl::dynl
)
add_executable(dynl.tests.unit_tests.dynl src/dynl.ut.cpp)
target_link_libraries(dynl.tests.unit_tests.dynl PRIVATE
        gmock_main
        dynl::dynl
)

gtest_discover_tests(dynl.tests.unit_tests.dynl_error)
gtest_discover_tests(dynl.tests.unit_tests.dynl)

