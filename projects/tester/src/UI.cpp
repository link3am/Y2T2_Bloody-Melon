#include "UI.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>

UI::UI(VertexArrayObject::sptr UIM,
	glm::vec3 location,
	glm::vec3 rotation,
	glm::vec3 scale,
	Texture2D::sptr UIUV
){
	UIMod = UIM;
	UITrans = Transform::Create();
	UITrans->Transform::SetLocalPosition(location);
	UITrans->Transform::SetLocalRotation(rotation);
	UITrans->Transform::SetLocalScale(scale);

	UIShader = Shader::Create();
	UIShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	UIShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	UIShader->Link();

	UITex = UIUV;
}

void UI::Render(Camera::sptr cam) {
	UIShader->Bind();
	UIShader->SetUniform("s_tex", 2);
	UITex->Bind(2);

	UIShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * UITrans->LocalTransform());
	UIShader->SetUniformMatrix("u_Model", UITrans->LocalTransform());
	UIMod->Render();
}