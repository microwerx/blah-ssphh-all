#include "pch.hpp"
#include <ssphhapp.hpp>
#include <tool_window.hpp>

ToolWindow::ToolWindow(const std::string& name) : Vf::Window(name) {}

ToolWindow::~ToolWindow() {}

void ToolWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		return;
	}

	ssg_wptr = ssphh_widget_ptr->ssg;
	context_wptr = ssphh_widget_ptr->rendererContext;

	Vf::Window::OnUpdate(timestamp);
}

void ToolWindow::OnRenderDearImGui() {
	auto context = context_wptr.lock();
	auto ssg = ssg_wptr.lock();
	if (!ssg || !context || !beginWindow())
		return;
	Vf::Window::OnRenderDearImGui();

	if (ImGui::TreeNode("POST")) {
		ImGui::SliderFloat("Simple Exposure", &ssg->environment->toneMap.x, -12.0f, 12.0f);
		ImGui::SliderFloat("Gamma Compression", &ssg->environment->toneMap.y, 0.0f, 2.2f);
		ImGui::SliderFloat("Filmic Highlights", &ssg->environment->toneMap.z, 0.0f, 1.0f);
		ImGui::SliderFloat("Filmic Shadows", &ssg->environment->toneMap.w, 0.0f, 1.0f);
		ImGui::SliderFloat("Fade Image", &ssg->environment->CompareBlend.r, 0.0f, 1.0f);
		ImGui::SliderFloat("Fade Color", &ssg->environment->CompareBlend.g, 0.0f, 1.0f);
		ImGui::SliderFloat("Fade Reference", &ssg->environment->CompareBlend.b, 0.0f, 1.0f);
		ImGui::SliderFloat("Compare Reference", &ssg->environment->CompareBlend.a, 0.0f, 1.0f);
		ImGui::ColorEdit3("Fade Color", ssg->environment->FadeColor.ptr());
		ImGui::SliderFloat("Fade Dissolve", &ssg->environment->FadeColor.a, 0.0f, 1.0f);
		ImGui::SliderFloat("Ref Exposure", &ssg->environment->ReferenceOps.r, -12.0f, 12.0f);
		ImGui::SliderFloat("Ref Gamma", &ssg->environment->ReferenceOps.g, 0.0f, 2.2f);
		ImGui::SliderFloat("Ref F.Highlights", &ssg->environment->ReferenceOps.b, 0.0f, 1.0f);
		ImGui::SliderFloat("Ref F.Shadows", &ssg->environment->ReferenceOps.a, 0.0f, 1.0f);
		ImGui::SliderFloat4("Flips", ssg->environment->Flips.ptr(), 0.0f, 1.0f);
		ImGui::TreePop();
	}

	using namespace ImGui;

	static ImVec2 size{ 128, 128 };
	static int texId{ 0 };
	static int arrayId{ 0 };
	ImGui::SliderFloat("Tex Size", &size.x, 1, 256);
	ImGui::SliderInt("Tex", &texId, 0, 100);
	ImGui::SliderInt("Arr", &arrayId, 0, 5);
	size.y = size.x;
	ImGui::Image((ImTextureID)texId, size);
	ImGui::ImageButton((ImTextureID)2, { 64, 64 });

	auto program = context->getProgram("webgl_unwrapped_cubemap_program").lock();
	if (program) {
		int i = 0;
		GLuint texture = 0;
		for (auto& t : context->textureCubes) {
			texture = t.second->getTexture();
			if (i++ == texId)
				break;
		}

		Fluxions::Matrix4f P = Matrix4f::MakeOrtho2D(0, 64*6, 0, 64*3);
		Fluxions::Matrix4f E = Matrix4f::MakeIdentity();
		Fluxions::Matrix4f W = Matrix4f::MakeIdentity();
		program->use();
		program->applyUniform("uCubeTexture", { 0 });
		program->applyUniform("ProjectionMatrix", P);
		program->applyUniform("CameraMatrix", E);
		program->applyUniform("WorldMatrix", W);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glEnable(GL_TEXTURE_CUBE_MAP);
		int vloc = program->getAttribLocation("aPosition");
		int tloc = program->getAttribLocation("aTexCoord");
		FxDrawGL2UnwrappedCubeMap(0, 0, 64, vloc, tloc);

		glLoadIdentity();
		glMultMatrixf(P.const_ptr());
		float z = arrayId > 0 ? 1 : -1;
		constexpr float xsize = 0.1;
		glUseProgram(0);
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord3f(0, 0, z);
		glVertex3f(0, 0, 0);
		glTexCoord3f(1, 0, z);
		glVertex3f(xsize, 0, 0);
		glTexCoord3f(1, 1, z);
		glVertex3f(xsize, xsize, 0);
		glTexCoord3f(0, 1, z);
		glVertex3f(0, xsize, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDisable(GL_TEXTURE_CUBE_MAP);
	}
	endWindow();
}
