#include "FairegroundPCH.h"
#include "Faireground/EditorLayer.h"

#include "Renaissance/Graphics/Texture.h"

#include "imgui.h"

namespace Renaissance
{
	void EditorLayer::OnAttached()
	{
		Window& window = Application::Get().GetWindow();

		mSceneCamera = Graphics::Camera::MakeOrthographic((float)window.GetWidth(), (float)window.GetHeight(), 1.0f, 0.1f, 500.0f);
		mSceneCamera->SetLocation(Math::Vector3(0.0f, 0.0f, 0.5f));
		mCameraController = MakeShared<CameraController>(mSceneCamera);

		mAwesomeFace = Graphics::Texture2D::Create("../Renaissance/assets/textures/awesomeface.png");
		mGrass = Graphics::Texture2D::Create("../Renaissance/assets/textures/grass.png");
		mContainer = Graphics::Texture2D::Create("../Renaissance/assets/textures/container.jpg");

		Graphics::FrameBuffer::Specification spec;
		spec.Width = Application::Get().GetWindow().GetWidth();
		spec.Height = Application::Get().GetWindow().GetHeight();

		Graphics::FrameBufferLayout sceneBufferLayout = { 
			{ Graphics::FrameBufferAttachmentType::Color, true }
		};
		mSceneBuffer = Graphics::FrameBuffer::Create(spec, sceneBufferLayout);
	}

	void EditorLayer::OnDetached()
	{

	}

	void EditorLayer::OnUIRender()
	{
		static bool fullscreenViewport = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Renaissance Editor", &mShowEditor, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);
		
		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			REN_CORE_WARN("Dear ImGui docking disabled.");
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Fullscreen Viewport", nullptr, fullscreenViewport))
				{
					fullscreenViewport = !fullscreenViewport;
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", nullptr, false, true))
				{
					Renaissance::Application::Get().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (!fullscreenViewport)
		{
			ImGui::Begin("Viewport");
			uint32_t sceneBufferColorId = mSceneBuffer->GetAttachmentRendererId(Graphics::FrameBufferAttachmentType::Color);
			ImGui::Image((void*)(uint64_t)sceneBufferColorId, ImGui::GetWindowSize(), ImVec2(0.0f ,1.0f), ImVec2(1.0f, 0.0f));
			ImGui::End();
		}

		ImGui::End();
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		mCameraController->OnUpdate(deltaTime);

		{
			Graphics::Renderer::Get().BeginScene(mSceneCamera);
			
			mSceneBuffer->Bind();
			{
				Graphics::SpriteBatch spriteBatch;
				spriteBatch.Draw(Vector2(0.0f, 0.0f), Vector2(1.0f), mAwesomeFace, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(-0.2f, 0.4f), Vector2(0.5f), mGrass, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(-0.5f, -0.2f), Vector2(0.2f), mAwesomeFace, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(0.6f, 0.1f), Vector2(0.2f), mContainer, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(0.6f, 0.1f), Vector2(0.2f), Vector4(1.0f, 0.2f, 0.4f, 1.0f));
			}
			mSceneBuffer->Unbind();

			Graphics::Renderer::Get().EndScene();
		}
	}

	void EditorLayer::OnEvent(Events::Event& e)
	{

	}
}
