# Add all source files of our added libraries
file(
    GLOB_RECURSE
    src_files_ext
    ${CMAKE_CURRENT_LIST_DIR}/libs/*.[ch]pp
)

# Allow to sort and explore source directories as a tree structure in Visual Studio
if(MSVC)
    source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${src_files_ext})
endif()