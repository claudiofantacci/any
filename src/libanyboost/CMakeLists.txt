set(LIBRARY_TARGET_NAME libanyboost)

set(${LIBRARY_TARGET_NAME}_HDR
        include/libanyboost/any_boost.h
)

add_library(${LIBRARY_TARGET_NAME} INTERFACE)

target_include_directories(${LIBRARY_TARGET_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                            "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${LIBRARY_TARGET_NAME}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}")

add_custom_target("HeadersOnly_${LIBRARY_TARGET_NAME}" SOURCES ${${LIBRARY_TARGET_NAME}_HDR})
