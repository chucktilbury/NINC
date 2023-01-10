project(common)

add_library(${PROJECT_NAME}
    STATIC
        memory.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        $<$<CONFIG:DEBUG>:-g3 >
        $<$<CONFIG:DEBUG>:-Og >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)
