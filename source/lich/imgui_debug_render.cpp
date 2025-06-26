#include "debug_render.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace lich::debug_render {

void bootstrap(void *window_handle) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::GetStyle().ScaleAllSizes(io.FontGlobalScale = 1.0f);
	ImGui::StyleColorsDark();
    
	ImGui_ImplGlfw_InitForOpenGL(
		static_cast<GLFWwindow *>(window_handle), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void shutdown(void) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void begin(void) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void end(void) {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
