set(MAIN main)

set(SRC_FILES ./main.cpp)

add_executable(${MAIN} ${SRC_FILES})

target_include_directories(${MAIN} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${MAIN} PUBLIC ctl)
