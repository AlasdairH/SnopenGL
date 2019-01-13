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
					if (ImGui::MenuItem("Close", "Esc")) { m_state->isRunning = false; }
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


		ImGui::Begin("Performance");
		
		ImGui::Text("FPS: %f", m_state->framesPerSecond);
		ImGui::Text("DT: %f", m_state->deltaTime);

		ImGui::End();
	}

	void GUI::onRender()
	{
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
	}
}