

add_executable(
    sample
    main.cpp
)

target_link_libraries(
    sample
    PUBLIC
    ${PROJECT_NAME}
)