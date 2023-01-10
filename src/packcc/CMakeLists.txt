project(packcc)

add_executable(${PROJECT_NAME}
    packcc.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Ofast
)
