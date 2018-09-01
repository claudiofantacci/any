set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

set(${LIBRARY_TARGET_NAME}_HDR
        include/libany/any.h
)

add_library(${LIBRARY_TARGET_NAME} INTERFACE)

target_include_directories(${LIBRARY_TARGET_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                            "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

option(DEFINE_ANY_IMPL_FAST_TYPE_INFO_COMPARE
       "Checks only the address of the type infos, otherwise does an actual comparision"
       OFF)
if (DEFINE_ANY_IMPL_FAST_TYPE_INFO_COMPARE)
    target_compile_definitions(${LIBRARY_TARGET_NAME} INTERFACE ANY_IMPL_FAST_TYPE_INFO_COMPARE)
endif()

option(DEFINE_ANY_IMPL_ANY_CAST_MOVEABLE
       "Enable move semnatics for any objects"
       ON)
if (DEFINE_ANY_IMPL_ANY_CAST_MOVEABLE)
    target_compile_definitions(${LIBRARY_TARGET_NAME} INTERFACE ANY_IMPL_ANY_CAST_MOVEABLE)
endif()

install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${LIBRARY_TARGET_NAME}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}")

add_custom_target("HeadersOnly_${PROJECT_NAME}" SOURCES ${${LIBRARY_TARGET_NAME}_HDR})
