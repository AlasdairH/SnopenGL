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
						CONSOLE_MESSAGE("Set camera to PROJECTION_ORTHOGRAPHIC")
					}
					if (ImGui::MenuItem("Perspective Mode"))
					{
						Camera::activeCamera->setProjectionMode(PROJECTION_PERSPECTIVE);
						CONSOLE_MESSAGE("Set camera to PROJECTION_PERSPECTIVE")
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
				else
				{
					if (ImGui::BeginMenu("View Mode"))
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



			ImGui::End();

			// OpenGL
			//ImGui::SetNextWindowPos(ImVec2(0, 200));
			ImGui::Begin("OpenGL", open, ImGuiWindowFlags_AlwaysAutoResize);

			float pointSize = m_selectedParticleSystem->getPointSize();
			ImGui::SliderFloat("Point Size", &pointSize, 1.0f, 10.0f);
			m_selectedParticleSystem->setPointSize(pointSize);

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

				ImGui::Text("Start Colour");
				glm::vec4 &colStart = m_selectedParticleSystem->getSettingsPtr()->colourStart;
				float colStartF4[4] = { colStart.x, colStart.y, colStart.z, colStart.w };
				ImGui::PushItemWidth(80);
				ImGui::ColorEdit4("Start Colour", colStartF4);
				colStart = glm::vec4(colStartF4[0], colStartF4[1], colStartF4[2], colStartF4[3]);

				ImGui::Text("End Colour");
				glm::vec4 &colEnd = m_selectedParticleSystem->getSettingsPtr()->colourEnd;
				float colEndF4[4] = { colEnd.x, colEnd.y, colEnd.z, colEnd.w };
				ImGui::PushItemWidth(80);
				ImGui::ColorEdit4("End Colour", colEndF4);
				colEnd = glm::vec4(colEndF4[0], colEndF4[1], colEndF4[2], colEndF4[3]);

				ImGui::Separator();

				ImGui::Text("Collision Colour");
				glm::vec4 &colCollision = m_selectedParticleSystem->getSettingsPtr()->collisionDebugColour;
				float colCollisionF4[4] = { colCollision.x, colCollision.y, colCollision.z, colCollision.w };
				ImGui::PushItemWidth(80);
				ImGui::ColorEdit4("Collision Colour", colCollisionF4);
				colCollision = glm::vec4(colCollisionF4[0], colCollisionF4[1], colCollisionF4[2], colCollisionF4[3]);

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