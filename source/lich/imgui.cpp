#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "imgui.hpp"
#include "input.hpp"
#include "log.hpp"

namespace lich {

Imgui_Layer::Imgui_Layer(void *window_handle) :
	Layer{"lich::Imgui_Layer"}, _window_handle{window_handle} {}

void Imgui_Layer::init() {
	_show_demo_window = true;
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::GetStyle().ScaleAllSizes(io.FontGlobalScale = 1.0f);
	ImGui::StyleColorsDark();
    
	ImGui_ImplGlfw_InitForOpenGL(
		static_cast<GLFWwindow *>(_window_handle), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Imgui_Layer::quit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Imgui_Layer::update([[maybe_unused]] Timestep timestep) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (_show_demo_window) ImGui::ShowDemoWindow(&_show_demo_window);
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Imgui_Layer::handle(Event &event) {
	Event_Dispatcher{event}.handle<Key_Press_Event>(
		[this] (const auto &event) -> bool {
			if (event.code == Key_Code::F5) {
				_show_demo_window = !_show_demo_window;
				return true;
			}
			return false;
		});
}

}
