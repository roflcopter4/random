# tree/src CMakeLists.txt

add_executable(tree main.c options.c utility.c impl.c game.c display.c quicksort.c)
TARGET_LINK_LIBRARIES(tree ${TREE_LDFLAGS})
