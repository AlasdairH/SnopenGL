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
		ApplicationState &state = ApplicationState::getInstance();

		ImGui_ImplSdlGL3_NewFrame(m_window);

		if (!m_state->isUIHidden)
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Show Tutorial", "")) { m_state->isTutorialOpen = true; }
					if (ImGui::MenuItem("Exit", "Esc")) { m_state->isRunning = false; }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene"))
				{			
					if (ImGui::MenuItem("Toggle Pause"))
					{
						m_state->isPaused = !m_state->isPaused;
					}
					if (ImGui::MenuItem("Switch Camera"))
					{
						// switch mode between view and edit
						state.switchSceneMode();
					}
					if (ImGui::MenuItem("Reset Camera Pos")) 
					{ 
						// switch mode between view and edit
						Camera::activeCamera->transform.setPosition(glm::vec3(0, 2, 0));
					}
					if (ImGui::MenuItem("Orthographic Mode"))
					{
						Camera::activeCamera->setProjectionMode(PROJECTION_ORTHOGRAPHIC);
						LOG(LOG_DEBUG) << "Set camera to PROJECTION_ORTHOGRAPHIC";
					}
					if (ImGui::MenuItem("Perspective Mode"))
					{
						Camera::activeCamera->setProjectionMode(PROJECTION_PERSPECTIVE);
						LOG(LOG_DEBUG) << "Set camera to PROJECTION_PERSPECTIVE";
					}
					ImGui::EndMenu();
				}
				if (m_state->getSceneMode() == MODE_EDIT)
				{
					if (ImGui::BeginMenu("Edit Mode"))
					{
						if (ImGui::MenuItem("Exit")) { m_state->isRunning = false; }
						ImGui::EndMenu();
					}
				}

				ImGui::EndMainMenuBar();
			}

			bool *open = NULL;
			// Performance
			//ImGui::SetNextWindowPos(ImVec2(0, 50));
			ImGui::Begin("Performance", open, ImGuiWindowFlags_AlwaysAutoResize);
		
			ImGui::Text("FPS: %f", m_state->framesPerSecond);
			for (int i = 0; i < m_fpsValues.size() - 1; ++i)
			{
				m_fpsValues[i] = m_fpsValues[i + 1]; //move all element to the left except first one
			}
			m_fpsValues[m_fpsValues.size() - 1] = m_state->framesPerSecond;

			ImGui::PlotLines("", m_fpsValues.data(), (int)m_fpsValues.size());
			ImGui::Separator();
			//ImGui::Text("Delta Time: %fms", m_state->deltaTime * 1000.0f);
			ImGui::Text("Delta Time: %fms", m_state->deltaTime);
			ImGui::Text("Current Frame: %i", m_state->currentFrame);
			ImGui::Text("Simulation Time: %f", m_state->currentTime);

			if (ImGui::Button("Toggle Pause"))
			{
				m_state->isPaused = !m_state->isPaused;
			}

			ImGui::End();

			// OpenGL
			//ImGui::SetNextWindowPos(ImVec2(0, 200));
			ImGui::Begin("OpenGL", open, ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Toggle VSync"))
			{
				SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());
			}

			ImGui::End();

			if (m_selectedParticleSystem != nullptr)
			{
				// Particle System
				//ImGui::SetNextWindowPos(ImVec2(0, 400));
				ImGui::Begin("Particle System", open, ImGuiWindowFlags_AlwaysAutoResize);

				ImGui::Text("Particle count: %i", m_selectedParticleSystem->getSettingsPtr()->getMaxParticles());
				ImGui::Separator();
				
				ImGui::Text("Global Wind");
				ImGui::PushItemWidth(80);
				ImGui::SliderFloat("x", &m_selectedParticleSystem->getSettingsPtr()->globalWind.x, -0.5f, 0.5f, "%.2f");
				ImGui::SameLine();
				ImGui::SliderFloat("y", &m_selectedParticleSystem->getSettingsPtr()->globalWind.y, -0.5f, 0.5f, "%.2f");
				ImGui::SameLine();
				ImGui::SliderFloat("z", &m_selectedParticleSystem->getSettingsPtr()->globalWind.z, -0.5f, 0.5f, "%.2f");

				if (ImGui::Button("Reset"))
				{
					m_selectedParticleSystem->getSettingsPtr()->globalWind = glm::vec3(0);
					m_selectedParticleSystem->applySettingsToShader();
				}

				ImGui::Separator();

				ImGui::SliderFloat("Collision Multiplier", &m_selectedParticleSystem->getSettingsPtr()->collisionMultiplier, 1.0f, 100.0f);

				ImGui::Separator();

				if (ImGui::Button("Apply"))
				{
					m_selectedParticleSystem->applySettingsToShader();
				}

				ImGui::Separator();

				if (ImGui::Button("Show/Hide Partition"))
				{
					m_selectedParticleSystem->getSettingsPtr()->drawPartition = !m_selectedParticleSystem->getSettingsPtr()->drawPartition;
				}

				ImGui::Separator();

				if (ImGui::Button("Start/Stop Particles"))
				{
					state.isRenderingParticles = !state.isRenderingParticles;
				}

				ImGui::End();
			}

			// Tutorial
			if (state.isTutorialOpen)
			{
				// remain paused while tutorial is open
				state.isPaused = true;

				ImGui::SetNextWindowPos(ImVec2(state.windowSize.x / 2 - 100, state.windowSize.y / 2 - 100));
				ImGui::Begin("Tutorial", open, ImGuiWindowFlags_AlwaysAutoResize);

				ImGui::Text("Welcome to SnopenGL!");
				ImGui::Separator();
				ImGui::Text("Controls:");
				ImGui::Text("W, S, A, D - Move Camera in view direction");
				ImGui::Text("Q, E - Move Camera up or down");
				ImGui::Text("Right Mouse Button (hold) - Rotate camera direction");
				ImGui::Text("Space - Hide UI");
				ImGui::Separator();
				ImGui::Text("You can bring this tutorial back from the File menu");

				if (ImGui::Button("Close and Run"))
				{
					state.isTutorialOpen = false;
					state.isPaused = false;
				}

				ImGui::End();
			}
		}
	}

	void GUI::onRender()
	{
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
	}
}