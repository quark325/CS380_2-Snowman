#include <Snowman.hpp>
Snowman::Snowman()
{

}

Snowman::~Snowman()
{

}


void Snowman::rotate_body(float deltaTime)
{
	body->SetOrientation(glm::rotate(body->GetOrientation(), glm::radians(90.0f*deltaTime), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::rotate_head(float deltaTime)
{
	head->SetOrientation(glm::rotate(head->GetOrientation(), glm::radians(90.0f*deltaTime), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Snowman::jump(float deltaTime)
{
	isJumping = true;
	float maxHeight = 2.0f;
	float aniSpeed = 2.0f;

	deltaTime *= aniSpeed;
	jumpingTime += deltaTime;
	
	glm::vec3 currentPos = body->GetPosition();
	body->SetPosition(glm::vec3(currentPos.x, currentPos.y+
		maxHeight * (2.0f * deltaTime - deltaTime*(2*jumpingTime-deltaTime)),currentPos.z));
	//y_1 = y_0 + 2 (t_1 - t_0) - (t_1^2 - t_0^2) => in gravity 1
	if (jumpingTime >= 2.0f) {
		jumpingTime = 0.0f;
		isJumping = false;
	}
}

void Snowman::move4direction(float deltaTime, int direction) { //direction 0:front, 1: left, 2:back, 3:right
	float anispeed = 2.0f;
	
	float direction1 = (direction / 2 == 0) ? 1.0f : -1.0f; // ture : front or left, false : back or right
	glm::vec4 moveVec = (int(direction) % 2 == 0) ? 
		glm::vec4(0.0f, 0.0f, direction1 * deltaTime * anispeed, 1.0f):
		glm::vec4(direction1 * deltaTime * anispeed, 0.0f, 0.0f, 1.0f); //true : front or back, false : left or right

	glm::mat4 oriMatrix = body->GetOrientation();
	glm::vec3 currentPos = body->GetPosition();
	glm::vec3 resultPos = glm::vec3(oriMatrix * moveVec);
	body->SetPosition(glm::vec3(currentPos.x+resultPos.x, currentPos.y+resultPos.y, currentPos.z+resultPos.z));
}

void Snowman::legMove(float deltaTime) {
	float aniSpeed = 2.0f;
	deltaTime *= aniSpeed;
	legMoveTime += deltaTime;

	//glm::vec3 currentPos1 = leg1->GetPosition();
	if (legMoveTime <= 0.5f) {
		leg1->SetOrientation(glm::rotate(leg1->GetOrientation(), glm::radians(30.0f*deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)));
		leg2->SetOrientation(glm::rotate(leg2->GetOrientation(), glm::radians(-30.0f*deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else if (legMoveTime <= 1.5f) {
		leg1->SetOrientation(glm::rotate(leg1->GetOrientation(), glm::radians(-30.0f*deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)));
		leg2->SetOrientation(glm::rotate(leg2->GetOrientation(), glm::radians(30.0f*deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else{ //legMoveTime : 1.5f ~ 2.0f
		leg1->SetOrientation(glm::rotate(leg1->GetOrientation(), glm::radians(30.0f*deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)));
		leg2->SetOrientation(glm::rotate(leg2->GetOrientation(), glm::radians(-30.0f*deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)));
	}

	if (legMoveTime >= 2.0f) {
		legMoveTime = 0.0f;
	}
}

void Snowman::handShaking(float deltaTime) {
	isShaking = true;
	float aniSpeed = 30.0f;
	deltaTime *= aniSpeed;
	shakingTime += deltaTime;
	float shakeAngle = 0.0f;

	if (is1stpartsfinish) {//팔 다올라감
		if (shakingTime <= 15.0f) {
			shakeAngle = shakingTime;
		}
		else if (shakingTime <= 45.0f) {
			shakeAngle = 30.0f - shakingTime;
		}
		else {
			shakeAngle = shakingTime - 60.0f;
			if (shakingTime > 60.0f) {
				shakingTime = 0.0f;
			}
		}
		rightArm2->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f+shakeAngle), glm::vec3(0.0f, 0.0f, 1.0f)));
	}
	else {//팔 아직 다 안올라감
		shakeAngle = shakingTime;
		printf("ShakingTime is : %f\n", shakingTime);

		rightArm1->SetOrientation(glm::rotate(body->GetOrientation(), glm::radians(deltaTime), glm::vec3(0.0f, 0.0f, -1.0f)));
		if (shakingTime >= 30.0f) {
			is1stpartsfinish = true;
			shakingTime = 0.0f;
		}
	}
}
