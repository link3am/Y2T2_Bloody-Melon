#pragma once

#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"
class Bullet
{
public:
	Bullet(VertexArrayObject::sptr BM,
		glm::vec3 location,
		glm::vec3 rotation,
		glm::vec3 scale,
		Texture2D::sptr bulletUV,
		bool isRight
	);

	void Render(Camera::sptr cam);

	void projectile();

	bool IsDeath() {
		return death;
	}
	entt::entity bullet;
	glm::vec3 startPos;
	glm::vec3 endPos;
	bool IsRight = true;
	float t = 0;
	bool death = false;
	Transform::sptr bulletTrans = nullptr;
	VertexArrayObject::sptr bulletMod = nullptr;
	Shader::sptr bulletShader = nullptr;
	Texture2D::sptr bulletTex = nullptr;

	glm::vec4 getHitBox();
};