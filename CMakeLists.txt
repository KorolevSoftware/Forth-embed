cmake_minimum_required(VERSION 3.14)
project(test C)



set(SOURCES
  main.c
)
add_subdirectory(src)
add_executable(${PROJECT_NAME} ${SOURCES})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
  )
endif()

target_link_libraries(${PROJECT_NAME}
  Forth-embed
)