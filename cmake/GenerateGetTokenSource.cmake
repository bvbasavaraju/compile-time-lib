function(extract_symbols_and_generate_source TARGET_NAME)

    # Remove Old symbols captured
    # add_custom_target(clean_old_symbols
    #     COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/undefined_symbols_captured.txt
    #     COMMENT "Removing old symbols"
    # )

    # Capture the Symbols from object files
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/undefined_symbols_captured.txt
        COMMAND find ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles -name "*.o" -exec nm --demangle --undefined-only {} + | grep get_token > ${CMAKE_CURRENT_BINARY_DIR}/undefined_symbols_captured.txt
        DEPENDS ${ARGN}
        COMMENT "Extracting the undefined get_token symbols"
    )

    add_custom_target(gather_symbols ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/undefined_symbols_captured.txt
    )

    # Generate Source form Symbol
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/log_tokens.cpp
        COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/generate_get_token_source.py ${CMAKE_CURRENT_BINARY_DIR}/undefined_symbols_captured.txt ${CMAKE_CURRENT_BINARY_DIR}/log_tokens.cpp ${CMAKE_CURRENT_BINARY_DIR}/log_tokens.xml ${CMAKE_CURRENT_BINARY_DIR}/log_tokens.json
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/undefined_symbols_captured.txt ${CMAKE_SOURCE_DIR}/scripts/generate_get_token_source.py
        COMMENT "Generating log_tokens.cpp file"
    )

    add_custom_target(generate_source ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/log_tokens.cpp
    )

    # Setup dependencies
    # add_dependencies(${TARGET_NAME} clean_old_symbols)
    add_dependencies(${TARGET_NAME} gather_symbols)
    add_dependencies(${TARGET_NAME} generate_source)

endfunction()
