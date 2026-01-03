# Include the token generation cmake module
include(${PROJECT_SOURCE_DIR}/cmake/GenerateGetTokenSource.cmake)

set(MAIN main)

set(SRC_FILES ./main.cpp)

add_executable(${MAIN} ${SRC_FILES})

target_include_directories(${MAIN} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${MAIN} PUBLIC ctl)

# Create a temporary object library to capture symbols from main
add_library(main_objects OBJECT ${SRC_FILES})
target_include_directories(main_objects PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Generate log_tokens.cpp from undefined symbols in main_objects
extract_symbols_and_generate_source(${MAIN} main_objects)

# Create a library from the generated log_tokens.cpp
add_library(log_tokens STATIC ${CMAKE_CURRENT_BINARY_DIR}/log_tokens.cpp)
target_include_directories(log_tokens PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_dependencies(log_tokens generate_source)

# Link the generated tokens library to main
target_link_libraries(${MAIN} PUBLIC log_tokens)
