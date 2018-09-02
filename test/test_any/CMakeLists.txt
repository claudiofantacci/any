set(TEST_TARGET_NAME test_any)

set(${TEST_TARGET_NAME}_SRC
        test_any.cpp
        ../common/CpuTimer.h
)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRC})

target_include_directories(${TEST_TARGET_NAME} PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common>")

target_link_libraries(${TEST_TARGET_NAME} libany)

add_test(NAME ${TEST_TARGET_NAME}
         COMMAND ${TEST_TARGET_NAME}
         WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)
