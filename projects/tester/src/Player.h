#pragma once

#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"
class Player
{
public:
	Player(std::string MM,
		glm::vec3 location,
		glm::vec3 rotation,
		glm::vec3 scale,
		std::string melonUV
		);

	void Render(Camera::sptr cam);

	glm::vec4 getHitBox();

	bool IsDeath() {
		return death;
	}

	//control
	void glWindow(GLFWwindow* inWindow);

	void control(float dt);
	void phyUpdate(float dt);
	void mapping();
	void blocker();
	glm::vec3 getPlayervec3();
	GLFWwindow* window;
	glm::vec3 pos;
	float groundHight = 0.0f;
	bool isGround = true;

	float acceleration = -50.0f;
	float position = 0.0f;
	//3am end
	
	bool death = false;

	Transform::sptr melonTrans = nullptr;
	VertexArrayObject::sptr melonMod = nullptr;
	Shader::sptr melonShader = nullptr;

	Texture2D::sptr melonTex = nullptr;
};

