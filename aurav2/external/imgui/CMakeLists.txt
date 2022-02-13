cmake_minimum_required(VERSION 3.5)
project(ImGui)

add_library(ImGui STATIC
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_demo.cpp
	imgui/imgui.cpp

	imgui/backends/imgui_impl_android.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	)

target_include_directories(ImGui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_compile_definitions(ImGui PRIVATE -DIMGUI_IMPL_OPENGL_ES2)
target_link_libraries(ImGui GLESv2)
target_link_libraries(ImGui android)