add_library(lib_geo STATIC
    Octree.cpp
    Octree.h
    TinMesh.cpp
    TinMesh.h
)

target_link_libraries(lib_geo PUBLIC lib_tip)
target_include_directories(lib_geo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
