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

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(rasterizer OpenMP::OpenMP_CXX)
endif()
