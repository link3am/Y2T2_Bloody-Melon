#include "stuff.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>
Stuff::Stuff(std::string obj, glm::vec3 location, glm::vec3 rotation, std::string UV)
{
	stuffMod = ObjLoader::LoadFromFile(obj);
	stuffTrans = Transform::Create();
	stuffTrans->Transform::SetLocalPosition(location);
	stuffTrans->Transform::SetLocalRotation(rotation);
	stuffTrans->Transform::SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

	stuffShader = Shader::Create();
	stuffShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	stuffShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	stuffShader->Link();
	stuffTex = Texture2D::LoadFromFile(UV);
}

void Stuff::Render(Camera::sptr cam)
{

	stuffShader->Bind();
	stuffShader->SetUniform("s_tex", 2);
	stuffTex->Bind(2);

	stuffShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * stuffTrans->LocalTransform());
	stuffShader->SetUniformMatrix("u_Model", stuffTrans->LocalTransform());
	stuffMod->Render();
	
}
