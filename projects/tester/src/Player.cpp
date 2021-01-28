#include "Player.h"
#include <Graphics\ObjLoader.h>
#include <Graphics\Texture2DData.h>
#include <Graphics\Texture2D.h>

Player::Player(std::string MM,
	glm::vec3 location,
	glm::vec3 rotation,
	glm::vec3 scale,
	std::string melonUV
) {
	melonMod = ObjLoader::LoadFromFile(MM);
	melonTrans = Transform::Create();
	melonTrans->Transform::SetLocalPosition(location);
	melonTrans->Transform::SetLocalRotation(rotation);
	melonTrans->Transform::SetLocalScale(scale);



	melonShader = Shader::Create();
	melonShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	melonShader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	melonShader->Link();

	melonTex = Texture2D::LoadFromFile(melonUV);
}

void Player::Render(Camera::sptr cam) {
	melonShader->Bind();
	melonShader->SetUniform("s_tex", 2);
	melonTex->Bind(2);
	
	melonShader->SetUniformMatrix("u_ModelViewProjection", cam->GetViewProjection() * melonTrans->LocalTransform());
	melonShader->SetUniformMatrix("u_Model", melonTrans->LocalTransform());
	melonMod->Render();
}

glm::vec4  Player::getHitBox()
{
	return { melonTrans->GetLocalPosition().x, melonTrans->GetLocalPosition().y, melonTrans->GetLocalScale().x, melonTrans->GetLocalScale().y };
}


///////3am 
void Player::glWindow(GLFWwindow* inWindow)
{
	window = inWindow;
}
void Player::control(float dt)
{
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		melonTrans->MoveLocalFixed(0.1f, 0.0f, 0.0f);
		melonTrans->SetLocalRotation(0.0f, -10.0f, 0.0f);
		melonTrans->SetLocalScale(1.0f, 1.0f, -1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		melonTrans->MoveLocalFixed(-0.1f, 0.0f, 0.0f);
		melonTrans->SetLocalRotation(0.0f, 10.0f, 0.0f);
		melonTrans->SetLocalScale(-1.0f, 1.0f, -1.0f);

	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		if (isGround == true) {
			position += 15 * dt;
			isGround = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (isGround == true && pos.y >= 0.2)
		{
			melonTrans->MoveLocalFixed(0.0f, -0.2f, 0.0f);
			isGround = false;
			groundHight = 0.0f;
		}
	}
	
}
void Player::phyUpdate(float dt)
{
	mapping();

	control(dt);
	blocker();
	pos = melonTrans->GetLocalPosition();
	//test

	if (pos.y < groundHight)
	{
		pos = melonTrans->GetLocalPosition();
		melonTrans->SetLocalPosition(pos.x, groundHight, pos.z);
		isGround = true;
		//test
		position = 0.0f;
	}
	
	if (pos.y > groundHight+0.1)//gravity
	{
		//test 
		isGround = false;
		position += acceleration * dt * dt;

	}

	melonTrans->MoveLocalFixed(0.0f, position, 0.0f);


	//std::cout << pos.x << std::endl;

	if (pos.y <= -5 || death ==true)
	{
		melonTrans->SetLocalPosition(0.0f, 0.0f, 0.0f);
		isGround = true;
	}
}
void Player::mapping()
{
	if (pos.x > 5.0f && pos.x < 11.0f && pos.y >= 3.9f)
	{
		groundHight = 4.0f;

	}
	else if (pos.x > 1.0f && pos.x < 3.0f && pos.y >= 1.9f)
	{
		groundHight = 2.0f;
	}
	else if (pos.x > 12.0f && pos.x < 18.0f && pos.y >= 2.9f)
	{
		groundHight = 3.0f;

	}
	else if (pos.x > 19.0f && pos.x < 19.8f && pos.y >= 1.4f)
	{
		groundHight = 1.5f;

	}
	else if (pos.x > 21.5f && pos.x < 23.5f && pos.y >= 0.9f)
	{
		groundHight = 1.0f;
	}
	else if (pos.x > 25.6f && pos.x < 26.4f && pos.y >= 1.4f)
	{//can
		groundHight = 1.5f;

	}
	else if (pos.x > 30.7f && pos.x < 32.4f && pos.y >= 0.6f)
	{
		groundHight = 0.7f;

	}

	else if (pos.x > 42.0f || pos.x < -3)
	{
		groundHight = -10.0f;
	}
	else
	{
		groundHight = 0;
	}

}

void Player::blocker()
{
	
	//microwave
	{
		if (pos.x > 12.0f && pos.x < 14.0f && pos.y < 2.9f)
		{
			melonTrans->MoveLocalFixed(-0.1f, 0.0f, 0.0f);
		}
		if (pos.x > 16.0f && pos.x < 18.5f && pos.y < 2.9f)
		{
			melonTrans->MoveLocalFixed(0.1f, 0.0f, 0.0f);
		}
	}
	//pan
	if (pos.x > 30.7f && pos.x < 31.0f && pos.y < 0.6f)
	{
		melonTrans->MoveLocalFixed(-0.1f, 0.0f, 0.0f);
	}
	if (pos.x > 32.2f && pos.x < 32.4f && pos.y < 0.6f)
	{
		melonTrans->MoveLocalFixed(0.1f, 0.0f, 0.0f);
	}
}


glm::vec3 Player::getPlayervec3()
{
	return pos;
}
