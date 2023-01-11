project(ninca)

add_executable(${PROJECT_NAME}
    parser.c
    ninca.c
)

set(PACKCC packcc CACHE FILEPATH "Specify file path of packcc command.")

add_custom_command(
    OUTPUT parser.c parser.h
    COMMAND ${PACKCC} ARGS -o parser ${CMAKE_CURRENT_SOURCE_DIR}/parser.peg
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.peg
    VERBATIM
)

target_link_libraries(${PROJECT_NAME}
    common
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/../include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    $<$<CONFIG:DEBUG>:-g>
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:PROFILE>:-pg -O0>
)
