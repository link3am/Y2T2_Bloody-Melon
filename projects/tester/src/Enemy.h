#pragma once
#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"

class Enemy
{
public:
	Enemy(std::string EM,
		glm::vec3 location,
		glm::vec3 rotation,
		glm::vec3 scale,
		std::string enemyUV
	);

	void Render(Camera::sptr cam);

	void AIPatrol();

	void setPatrolPoint(glm::vec3 p1, glm::vec3 p2);

	bool IsDeath() {
		return death;
	}

	glm::vec4 getHitBox();
	entt::entity enemy;

	float t = 0;

	bool death = false;

	glm::vec3 PatrolPoint1 = glm::vec3 (0.0, 0.0, 0.0);
	glm::vec3 PatrolPoint2 = glm::vec3 (0.0, 0.0, 0.0);
	Transform::sptr enemyTrans = nullptr;
	VertexArrayObject::sptr enemyMod = nullptr;
	Shader::sptr enemyShader = nullptr;

	Texture2D::sptr enemyTex = nullptr;
};

