cmake_minimum_required(VERSION 4.1 FATAL_ERROR)

set(PROJECT_NAME MyGame)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Render/Shaders.cpp
	src/Render/Shaders.h
	src/Render/Texture2D.cpp
	src/Render/Texture2D.h
	src/Resources/stb_image.h
	src/Resources/Resources.cpp
	src/Resources/Resources.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)