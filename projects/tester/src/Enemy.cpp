#include "Enemy.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>

Enemy::Enemy(std::string EM,
	glm::vec3 location,
	glm::vec3 rotation,
	glm::vec3 scale,
	std::string enemyUV
) {
	enemyMod = ObjLoader::LoadFromFile(EM);
	enemyTrans = Transform::Create();
	enemyTrans->Transform::SetLocalPosition(location);
	enemyTrans->Transform::SetLocalRotation(rotation);
	enemyTrans->Transform::SetLocalScale(scale);



	enemyShader = Shader::Create();
	enemyShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	enemyShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	enemyShader->Link();

	enemyTex = Texture2D::LoadFromFile(enemyUV);
}

void Enemy::Render(Camera::sptr cam) {
	enemyShader->Bind();
	enemyShader->SetUniform("s_tex", 2);
	enemyTex->Bind(2);

	enemyShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * enemyTrans->LocalTransform());
	enemyShader->SetUniformMatrix("u_Model", enemyTrans->LocalTransform());
	enemyMod->Render();
}

void Enemy::AIPatrol()
{
	if (enemyTrans->GetLocalScale().x > 0) {
		t += .005;
	}
	else {
		t -= .005;
	}

	if (t > 1) {
		enemyTrans->SetLocalScale(-1.0f, 1.0f, -1.0f);
	}
	else if (t < 0)
	{
		enemyTrans->SetLocalScale(1.0f, 1.0f, -1.0f);
	}

	glm::vec3 newPos = (PatrolPoint2 - PatrolPoint1)*t+ PatrolPoint1;
	enemyTrans->SetLocalPosition(newPos.x, newPos.y, enemyTrans->GetLocalPosition().z);
	//std::cout << newPos.x << "," << newPos.y << std::endl;
}

void Enemy::setPatrolPoint(glm::vec3 p1, glm::vec3 p2)
{
	PatrolPoint1 = p1;
	PatrolPoint2 = p2;
}

glm::vec4  Enemy::getHitBox()
{
	return {enemyTrans->GetLocalPosition().x, enemyTrans->GetLocalPosition().y, enemyTrans->GetLocalScale().x, enemyTrans->GetLocalScale().y };
}

