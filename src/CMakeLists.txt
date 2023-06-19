add_executable(rasterizer
    main.cpp
    window.cpp
    matrix.cpp
    model.cpp
    scene.cpp
    phong.cpp
    lighting.cpp
    io.cpp
    bmp.cpp
)

target_link_libraries(rasterizer
    GL
    glfw
)

find_package(TBB)
if (TBB_FOUND)
    target_link_libraries(rasterizer TBB::tbb)
endif()
