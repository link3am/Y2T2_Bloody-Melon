#pragma once

#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"
#include <AudioEngine.h>

class Object
{
public:
	Object(std::string MM,
		glm::vec3 location,
		glm::vec3 rotation,
		glm::vec3 scale,
		std::string melonUV
	);

	void Render(Camera::sptr cam);
	glm::vec4 getHitBox();

	Transform::sptr ObjTrans = nullptr;
	VertexArrayObject::sptr ObjModel = nullptr;
	Shader::sptr ObjShader = nullptr;
	Texture2D::sptr ObjTex = nullptr;
};