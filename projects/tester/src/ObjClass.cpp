#include "ObjClass.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>

Object::Object(std::string OM, glm::vec3 location, glm::vec3 rotation, glm::vec3 scale, std::string objUV)
{
	ObjModel = ObjLoader::LoadFromFile(OM);
	ObjTrans = Transform::Create();
	ObjTrans->Transform::SetLocalPosition(location);
	ObjTrans->Transform::SetLocalRotation(rotation);
	ObjTrans->Transform::SetLocalScale(scale);

	ObjShader = Shader::Create();
	ObjShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	ObjShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	ObjShader->Link();

	ObjTex = Texture2D::LoadFromFile(objUV);
}

void Object::Render(Camera::sptr cam)
{
	ObjShader->Bind();
	ObjShader->SetUniform("s_tex", 2);
	ObjTex->Bind(2);

	ObjShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * ObjTrans->LocalTransform());
	ObjShader->SetUniformMatrix("u_Model", ObjTrans->LocalTransform());
	ObjModel->Render();
}

glm::vec4 Object::getHitBox()
{
	return { 
		ObjTrans->GetLocalPosition().x,
		ObjTrans->GetLocalPosition().y,
		ObjTrans->GetLocalScale().x,
		ObjTrans->GetLocalScale().y };
}
