#include "Bullet.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>

Bullet::Bullet(VertexArrayObject::sptr BM,
	glm::vec3 location,
	glm::vec3 rotation,
	glm::vec3 scale,
	Texture2D::sptr bulletUV,
	bool isRight
) {
	//startPos = location;
	
	IsRight = isRight;
	
	if (IsRight) {
		startPos = glm::vec3(location.x + 1.0f, location.y, location.z);
	}
	else {
		startPos = glm::vec3(location.x - 1.0f, location.y, location.z);
	}

	if (IsRight) {
		endPos = glm::vec3(location.x + 5, location.y, location.z);
	}

	else {
		endPos = glm::vec3(location.x - 5, location.y, location.z);
	}
	bulletMod = BM;
	bulletTrans = Transform::Create();
	bulletTrans->Transform::SetLocalPosition(location);
	bulletTrans->Transform::SetLocalRotation(rotation);
	bulletTrans->Transform::SetLocalScale(scale);

	bulletShader = Shader::Create();
	bulletShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	bulletShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	bulletShader->Link();

	bulletTex = bulletUV;
}

void Bullet::Render(Camera::sptr cam) {
	bulletShader->Bind();
	bulletShader->SetUniform("s_tex", 2);
	bulletTex->Bind(2);

	bulletShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * bulletTrans->LocalTransform());
	bulletShader->SetUniformMatrix("u_Model", bulletTrans->LocalTransform());
	bulletMod->Render();
}

void Bullet::projectile()
{
	t += .05;
	if (t > 1) {
		death = true;
	}
	glm::vec3 newPos = (endPos - startPos) * t + startPos;
	bulletTrans->SetLocalPosition(newPos.x, newPos.y, bulletTrans->GetLocalPosition().z);


}

glm::vec4  Bullet::getHitBox()
{
	return { bulletTrans->GetLocalPosition().x, bulletTrans->GetLocalPosition().y, bulletTrans->GetLocalScale().x, bulletTrans->GetLocalScale().y };
}
