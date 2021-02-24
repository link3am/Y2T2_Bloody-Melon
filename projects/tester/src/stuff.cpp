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
	stuffShader->LoadShaderPartFromFile("shaders/lighting_frag.glsl", GL_FRAGMENT_SHADER);
	stuffShader->Link();
	stuffTex = Texture2D::LoadFromFile(UV);



}

void Stuff::Render(Camera::sptr cam, glm::vec3 lightpos, glm::vec3 camerapos)
{

	stuffShader->Bind();
	stuffShader->SetUniform("s_tex", 2);
	stuffTex->Bind(2);

	//light
		///lighting
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	float     lightAmbientPow = 0.05f;
	float     lightSpecularPow = 1.0f;
	glm::vec3 ambientCol = glm::vec3(1.0f);
	float     ambientPow = 0.1f;
	float     lightLinearFalloff = 0.09f;
	float     lightQuadraticFalloff = 0.0f;

	stuffShader->SetUniform("u_LightPos", lightPos);
	stuffShader->SetUniform("u_LightCol", lightCol);
	stuffShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	stuffShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	stuffShader->SetUniform("u_AmbientCol", ambientCol);
	stuffShader->SetUniform("u_AmbientStrength", ambientPow);
	stuffShader->SetUniform("u_LightAttenuationConstant", 1.0f);
	stuffShader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
	stuffShader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);

	stuffShader->SetUniform("u_LightPos", lightpos);
	stuffShader->SetUniform("u_CamPos", camerapos);
	//
	stuffShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * stuffTrans->LocalTransform());
	stuffShader->SetUniformMatrix("u_Model", stuffTrans->LocalTransform());
	stuffMod->Render();
	
}
