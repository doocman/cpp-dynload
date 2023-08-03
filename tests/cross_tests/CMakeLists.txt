
# This test is meant to compile a shared library using one compiler, and import it in a test with the
# current compiler.

add_executable(dynload.tests.cross_tests src/cross_test_import.cpp)
target_include_directories(dynload.tests.cross_tests PRIVATE include)
target_link_libraries(dynload.tests.cross_tests PRIVATE
        dynl::dynl
        gmock
        )

add_library(dynload.tests.cross_tests.export_this SHARED src/cross_test_export.cpp)
target_include_directories(dynload.tests.cross_tests.export_this PRIVATE include)
target_link_libraries(dynload.tests.cross_tests.export_this
        PRIVATE
        dynl::dynl_export
        )
add_dependencies(dynload.tests.cross_tests dynload.tests.cross_tests.export_this)
add_test(
        NAME dynload.tests.cross_tests.this
        COMMAND dynload.tests.cross_tests "$<TARGET_FILE:dynload.tests.cross_tests.export_this>" "Same as import"
)

set(DYNLOAD_TEST_EXPORTS "" CACHE FILEPATH "Populate these with the compilers you want to test")

set(COMPID 0)
foreach (DL_TEST_EXPORT IN LISTS DYNLOAD_TEST_EXPORTS)

    if(NOT IS_ABSOLUTE "${DL_TEST_EXPORT}")
        set(DL_TEST_EXPORT "${CMAKE_SOURCE_DIR}/${DL_TEST_EXPORT}")
    endif ()
    cmake_path(CONVERT "${DL_TEST_EXPORT}" TO_NATIVE_PATH_LIST DYNLOAD_FILE_NATIVE)

    add_test(
            NAME dynload.tests.cross_tests.${COMPID}
            COMMAND dynload.tests.cross_tests "${DYNLOAD_FILE_NATIVE}" "${COMPID}"
    )

    math(EXPR COMPID "${COMPID} + 1")
    message(STATUS "Added file ${DYNLOAD_FILE_NATIVE} for ID ${COMPID}")
endforeach ()
