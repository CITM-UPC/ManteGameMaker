#include "WindowScene.h"
#include "App.h"

WindowScene::WindowScene(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowScene::Update() {
	const float window_width = ImGui::GetContentRegionAvail().x;
	const float window_height = ImGui::GetContentRegionAvail().y;

	SceneFBO.Rescale_FrameBuffer(window_width, window_height);
	glViewport(0, 0, window_width, window_height);

	SceneFBO.Bind_FrameBuffer();

	// and we render our triangle as before
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glUseProgram(0);

	// and unbind it again 
	SceneFBO.Unbind_FrameBuffer();

	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::GetWindowDrawList()->AddImage(
		(void*)SceneFBO.GetTextureID(),
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + window_width, pos.y + window_height),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
}