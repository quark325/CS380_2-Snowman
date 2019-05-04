#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PickableObject.hpp>

class Snowman
{
private:
	
public:
    Snowman();
    ~Snowman();
	
	PickableObject* body;
	PickableObject* head;
	PickableObject* leg1;
	PickableObject* leg2;
	PickableObject* leftArm1;
	PickableObject* leftArm2;
	PickableObject* rightArm1;
	PickableObject* rightArm2;

	bool isJumping = false;
	float jumpingTime = 0.0f;
	void jump(float deltaTime);

	void rotate_head(float deltaTime);
	void rotate_body(float deltaTime);
	
	void move4direction(float deltaTime, int direction);

	float legMoveTime = 0.0f;
	void legMove(float deltaTime);

	bool isShaking = false;
	float shakingTime = 0.0f;
	bool is1stpartsfinish = false;
	void handShaking(float delatTime);
};