add_library(a1 ../src/dummy.cpp)

add_executable(basic_image basic_image.cpp)
target_link_libraries(basic_image a1)

add_executable(raytracing raytracing.cpp)
target_link_libraries(raytracing a1)

add_executable(sphere sphere.cpp)
target_link_libraries(sphere a1)

add_executable(sphere_anti_aliasing sphere_anti_aliasing.cpp)
target_link_libraries(sphere_anti_aliasing a1)

add_executable(basic_tools basic_tools.cpp)
target_link_libraries(basic_tools a1)

add_executable(sphere_iterative_raytracing sphere_iterative_raytracing.cpp)
target_link_libraries(sphere_iterative_raytracing a1)

