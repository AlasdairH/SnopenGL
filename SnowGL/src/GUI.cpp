#include "PCH.h"
#include "GUI.h"

namespace SnowGL
{
	GUI::GUI(SDL_Window *_window)
	{
		m_state = &ApplicationState::getInstance();


		// initialise IMGUI
		ImGui::CreateContext();
		ImGui_ImplSdlGL3_Init(m_window, "#version 430");
		ImGui::StyleColorsDark();

		m_window = _window;

		ImGuiIO& io = ImGui::GetIO();
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);
		io.DisplaySize.x = (float)w;
		io.DisplaySize.y = (float)h;
	}

	GUI::~GUI()
	{
		ImGui::DestroyContext();
	}

	void GUI::onUpdate()
	{
		ImGui_ImplSdlGL3_NewFrame(m_window);

		if (!m_state->isMenuBarHidden)
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit", "Esc")) { m_state->isRunning = false; }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene"))
				{
					if (ImGui::MenuItem("Switch Mode")) 
					{ 
						// switch mode between view and edit
						m_state->switchSceneMode();
					}					
					if (ImGui::MenuItem("Reset Camera")) 
					{ 
						// switch mode between view and edit
						Camera::activeCamera->setFront(glm::vec3(0, 0, -1));
						Camera::activeCamera->transform.setPosition(glm::vec3(0, 2, 0));
					}
					if (ImGui::MenuItem("Options")) { /* Do stuff */ }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		bool *open = NULL;
		// Performance
		ImGui::SetNextWindowPos(ImVec2(0, 50));
		ImGui::Begin("Performance", open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
		
		ImGui::Text("FPS: %f", m_state->framesPerSecond);
		for (int i = 0; i < m_fpsValues.size() - 1; ++i)
		{
			m_fpsValues[i] = m_fpsValues[i + 1]; //move all element to the left except first one
		}
		m_fpsValues[m_fpsValues.size() - 1] = m_state->framesPerSecond;

		ImGui::PlotLines("", m_fpsValues.data(), m_fpsValues.size());
		ImGui::Separator();
		ImGui::Text("Delta Time: %fms", m_state->deltaTime * 1000.0f);

		ImGui::End();

		// OpenGL
		ImGui::SetNextWindowPos(ImVec2(0, 200));
		ImGui::Begin("OpenGL", open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

		ImGui::Text("Bound Texture: %s", m_state->curBoundTexture.c_str());

		ImGui::End();
	}

	void GUI::onRender()
	{
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
	}
}