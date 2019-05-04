// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>

// include application headers
#include <DefaultMaterial.hpp>
#include <PickingMaterial.hpp>
#include <LineMaterial.hpp>
#include <Geometry.hpp>
#include <Animation.hpp>
#include <picking.hpp>
#include <PickableObject.hpp>
#include <Snowman.hpp>

#define _USE_MATH_DEFINES

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;


bool head_spin_mouseR = false;
bool body_spin_mouseR = false;
bool head_spin_mouseL = false;
bool body_spin_mouseL = false;
bool body_spin_Q = false;
bool body_spin_E = false;
bool body_jump = false;
bool move_left = false;
bool move_right = false;
bool move_foward = false;
bool move_back = false;
bool hand_shake = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.5f);
double clickedXpos = 0;
double clickedYpos = 0;
double clickedZpos = 0;

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
    //example code for get x position and y position of mouse click
    if (a_button == GLFW_MOUSE_BUTTON_RIGHT && a_action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(a_window, &xpos, &ypos);
        xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
        ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
		//printf("xpos : %lf, ypos : %lf\n", xpos, ypos);
        int target = pick((int)xpos, (int)ypos, g_framebuffer_width, g_framebuffer_height);
        std::cout << "Picked object index: " << target << std::endl;

		if (target == 1)
		{
			head_spin_mouseR = !head_spin_mouseR;
			head_spin_mouseL = false;
		}
		if (target == 2)
		{
			body_spin_mouseR = !body_spin_mouseR;
			body_spin_mouseL = false;
		}
    }
	
	if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(a_window, &xpos, &ypos);
		xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
		ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
		//printf("xpos : %lf, ypos : %lf\n", xpos, ypos);
		int target = pick((int)xpos, (int)ypos, g_framebuffer_width, g_framebuffer_height);
		std::cout << "Picked object index: " << target << std::endl;

		if (target == 1)
		{
			head_spin_mouseL = !head_spin_mouseL;
			head_spin_mouseR = false;
		}
		if (target == 2)
		{
			body_spin_mouseL = !body_spin_mouseL;
			body_spin_mouseR = false;
		}
	}
}





// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{
}

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
    if (a_action == GLFW_PRESS)
    {
        switch (a_key)
        {
        case GLFW_KEY_H:
            std::cout << "CS380 Homework 2" << std::endl;
            std::cout << "keymaps: WASD, QE, Space" << std::endl;
            std::cout << "h\t\t Help command" << std::endl;
            break;
		case GLFW_KEY_SPACE: //jump
			body_jump = true;
			break;
		case GLFW_KEY_A: //move to left
			move_left = true;
			break;
		case GLFW_KEY_D: //move to right
			move_right = true;
			break;
		case GLFW_KEY_W: //move to foward
			move_foward = true;
			break;
		case GLFW_KEY_S: //move to back
			move_back = true;
			break;
		case GLFW_KEY_Q:
			body_spin_Q = true;
			break;
		case GLFW_KEY_E:
			body_spin_E = true;
			break;
		/*
		case GLFW_KEY_R: //right hand shaking
			hand_shake = true;
			break;
		*/
		default:
            break;
        }
    }
	else if (a_action == GLFW_RELEASE) {
		switch (a_key)
		{
		case GLFW_KEY_SPACE: //jump finish
			body_jump = false;
			break;
		case GLFW_KEY_A: //left moving finish
			move_left = false;
			break;
		case GLFW_KEY_D: //right moving finish
			move_right = false;
			break;
		case GLFW_KEY_W: //move to foward
			move_foward = false;
			break;
		case GLFW_KEY_S: //move to back
			move_back = false;
			break;
		case GLFW_KEY_Q:
			body_spin_Q = false;
			break;
		case GLFW_KEY_E:
			body_spin_E = false;
			break;
		/*
		case GLFW_KEY_R: //right hand shaking
			hand_shake = false;
			break;
		*/
		default:
			break;
		}
	}
}

int main(int argc, char** argv)
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        return -1;
    }

    // Create window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow(g_window_width, g_window_height, "HW2 - Do you want to build a snowman?", NULL, NULL);
    if (!g_window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW library
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
        exit(1);
    }


    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);

    // Initialize framebuffer object and picking textures
    pickingInitialize(g_framebuffer_width, g_framebuffer_width);

    Engine::Camera* main_camera = new Engine::Camera();
    main_camera->SetPosition(cameraPos);

    // TODO: Create mesh and material (in main.cpp) Slide No. 10 (Define cube mesh, material), No. 18 (Define pickable object)
	Geometry geometry = Geometry();

    Engine::Mesh* arm_mesh = new Engine::Mesh();
    geometry.GenerateCylinder(arm_mesh, 0.06f);

	Engine::Mesh* hat_mesh1 = new Engine::Mesh();
	geometry.GenerateCylinder(hat_mesh1, 10.0f);

	Engine::Mesh* hat_mesh2 = new Engine::Mesh();
	geometry.GenerateCylinder(hat_mesh2, 0.6f);

	Engine::Mesh* sphere_mesh = new Engine::Mesh();
	geometry.GenerateSphere(sphere_mesh, 8);

	Engine::Mesh* circle_mesh = new Engine::Mesh();
	geometry.GenerateCircle(circle_mesh);

	Engine::Mesh* cone_mesh = new Engine::Mesh();
	geometry.GenerateCone(cone_mesh, 0.200f);

	Engine::Mesh* snow_mesh = new Engine::Mesh();
	geometry.GenerateSnowflake(snow_mesh);

	Animation* animation = new Animation();

    DefaultMaterial* material = new DefaultMaterial();
    material->CreateMaterial();

    PickingMaterial* picking_material = new PickingMaterial();
    picking_material->CreateMaterial();

	std::vector<PickableObject*> snowmanParts = std::vector<PickableObject*>();

	PickableObject background = PickableObject(circle_mesh, material);
	background.SetPickingMat(picking_material);
	background.SetPosition(glm::vec3(0.0f, -5.0f, -5.0f));
	background.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

	PickableObject backgroundSun = PickableObject(circle_mesh, material);
	backgroundSun.SetPickingMat(picking_material);
	backgroundSun.SetPosition(glm::vec3(3.5f, 3.0f, 0.0f));
	backgroundSun.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	backgroundSun.SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
	//background.SetIndex(1);
	
    PickableObject head = PickableObject(sphere_mesh, material);
    head.SetPickingMat(picking_material);
    head.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    head.SetIndex(1);
	snowmanParts.push_back(&head);

    PickableObject body =PickableObject(sphere_mesh, material);
    body.SetPickingMat(picking_material);
    body.SetPosition(glm::vec3(0.0f, -2.0f, -2.0f));
	body.SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
    body.SetIndex(2);
	snowmanParts.push_back(&body);

	PickableObject leftArm1 = PickableObject(arm_mesh, material);
	leftArm1.SetPickingMat(picking_material);
	leftArm1.SetPosition(glm::vec3(1.5f*cos(glm::radians(30.0f)), 1.5f*sin(glm::radians(30.0f)), 0.0f));
	leftArm1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(75.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	leftArm1.SetScale(glm::vec3(0.6f, 0.3f, 0.6f));
	leftArm1.SetIndex(3);
	snowmanParts.push_back(&leftArm1);

	PickableObject leftArm2 = PickableObject(arm_mesh, material);
	leftArm2.SetPickingMat(picking_material);
	leftArm2.SetPosition(glm::vec3(0.0f, 0.6f, 0.0f));
	leftArm2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(0.0f, 0.0f, +1.0f)));
	leftArm2.SetScale(glm::vec3(0.6f, 0.5f, 0.6f));
	leftArm2.SetIndex(3);
	snowmanParts.push_back(&leftArm2);

	PickableObject rightArm1 = PickableObject(arm_mesh, material);
	rightArm1.SetPickingMat(picking_material);
	rightArm1.SetPosition(glm::vec3(1.5f*cos(glm::radians(150.0f)), 1.5f*sin(glm::radians(150.0f)), 0.0f));
	rightArm1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(75.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	rightArm1.SetScale(glm::vec3(0.6f, 0.3f, 0.6f));
	rightArm1.SetIndex(4);
	snowmanParts.push_back(&rightArm1);

	PickableObject rightArm2 = PickableObject(arm_mesh, material);
	rightArm2.SetPickingMat(picking_material);
	rightArm2.SetPosition(glm::vec3(0.0f, 0.6f, 0.0f));
	rightArm2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, +1.0f)));
	rightArm2.SetScale(glm::vec3(0.6f, 0.5f, 0.6f));
	rightArm2.SetIndex(4);
	snowmanParts.push_back(&rightArm2);

	PickableObject hat1 = PickableObject(hat_mesh1, material);
	hat1.SetPickingMat(picking_material);
	hat1.SetPosition(glm::vec3(0.0f, 0.9f*cos(glm::radians(10.0f)), -0.9f*sin(glm::radians(10.0f))));
	hat1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	hat1.SetScale(glm::vec3(0.06f, 0.06f, 0.06f));
	hat1.SetIndex(5);
	snowmanParts.push_back(&hat1);

	PickableObject hat2 = PickableObject(hat_mesh2, material);
	hat2.SetPickingMat(picking_material);
	hat2.SetPosition(glm::vec3(0.0f, 0.12f, 0.0f));
	hat2.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	hat2.SetIndex(6);
	snowmanParts.push_back(&hat2);

	PickableObject hat3 = PickableObject(hat_mesh2, material);
	hat3.SetPickingMat(picking_material);
	hat3.SetPosition(glm::vec3(0.0f, 0.6f, 0.0f));
	hat3.SetScale(glm::vec3(0.51f, 0.5f/6.0f, 0.51f));
	//hat3.SetIndex(6);
	snowmanParts.push_back(&hat3);

	PickableObject hat4 = PickableObject(hat_mesh2, material);
	hat4.SetPickingMat(picking_material);
	hat4.SetPosition(glm::vec3(0.0f, 0.1f, 0.0f));
	hat4.SetScale(glm::vec3(0.5f, 0.5f/3.0f, 0.5f));
	//hat4.SetIndex(6);
	snowmanParts.push_back(&hat4);

	PickableObject eye1 = PickableObject(sphere_mesh, material);
	eye1.SetPickingMat(picking_material);
	eye1.SetPosition(glm::vec3(sin(glm::radians(60.0f))*sin(glm::radians(20.0f)),
		cos(glm::radians(60.0f)), sin(glm::radians(60.0f))*cos(glm::radians(20.0f))));
	eye1.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	//eye1.SetIndex(7);
	snowmanParts.push_back(&eye1);

	PickableObject eye2 = PickableObject(sphere_mesh, material);
	eye2.SetPickingMat(picking_material);
	eye2.SetPosition(glm::vec3(sin(glm::radians(60.0f))*sin(glm::radians(-20.0f)),
		cos(glm::radians(60.0f)), sin(glm::radians(60.0f))*cos(glm::radians(-20.0f))));
	eye2.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	//eye2.SetIndex(8);
	snowmanParts.push_back(&eye2);

	
	PickableObject nose = PickableObject(cone_mesh, material);
	nose.SetPickingMat(picking_material);
	nose.SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	nose.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	nose.SetScale(glm::vec3(0.33f, 0.33f, 0.33f));
	//nose.SetIndex(9);
	snowmanParts.push_back(&nose);

	PickableObject leg1 = PickableObject(arm_mesh, material);
	leg1.SetPickingMat(picking_material);
	leg1.SetPosition(glm::vec3(1.5f * sin(glm::radians(150.0f))*sin(glm::radians(120.0f)),
		1.5f * cos(glm::radians(150.0f)), 1.5f * sin(glm::radians(150.0f))*cos(glm::radians(120.0f))));
	leg1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(110.0f), glm::vec3(1.0f, 0.0f, -0.3f)));
	leg1.SetScale(glm::vec3(0.6f, 0.8f, 0.6f));
	//nose.SetIndex(9);
	snowmanParts.push_back(&leg1);

	PickableObject leg2 = PickableObject(arm_mesh, material);
	leg2.SetPickingMat(picking_material);
	leg2.SetPosition(glm::vec3(1.5f * sin(glm::radians(150.0f))*sin(glm::radians(-120.0f)),
		1.5f * cos(glm::radians(150.0f)), 1.5f * sin(glm::radians(150.0f))*cos(glm::radians(-120.0f))));
	leg2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(110.0f), glm::vec3(1.0f, 0.0f, 0.3f)));
	leg2.SetScale(glm::vec3(0.6f, 0.8f, 0.6f));
	//nose.SetIndex(9);
	snowmanParts.push_back(&leg2);
	

    // TODO: Add parent (cube2) to cube1 (Slide No. 14)
    //order : body, head, leftArm, rightArm, hat1, hat2, hat3, hat4, eye1, eye2, nose
	//          0    1      2       3         4    5      6    7     8      9     10
	head.AddParent(&body); 
	leftArm1.AddParent(&body);
	leftArm2.AddParent(&leftArm1);
	rightArm1.AddParent(&body);
	rightArm2.AddParent(&rightArm1);
	hat1.AddParent(&head);
	hat2.AddParent(&hat1);
	hat3.AddParent(&hat2);
	hat4.AddParent(&hat3);
	eye1.AddParent(&head);
	eye2.AddParent(&head);
	nose.AddParent(&head);
	leg1.AddParent(&body);
	leg2.AddParent(&body);

	Snowman snowman = Snowman();
	snowman.head = &head;
	snowman.body = &body;
	snowman.leg1 = &leg1;
	snowman.leg2 = &leg2;
	snowman.rightArm1 = &rightArm1;
	snowman.rightArm2 = &rightArm2;

	// Create star objects
	
	for (int i = 0; i < 25; i++)
	{	
		material->UpdateColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Engine::RenderObject* snow = new Engine::RenderObject(snow_mesh, material);
		snow->SetPosition(glm::vec3(-5.0f + 10.0f * ((rand() % 255) / 255.0f), 5.0f * ((rand() % 255) / 255.0f), 0.0f));
		snow->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
		animation->AddObject(snow);
	}

    float prev_time = 0;
	float color_time = 0;
	float backgroundX = 0;
	float backgroundY = 0;
	float backgroundZ = 0;

	/* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
		main_camera->SetPosition(cameraPos);
        float total_time = (float)glfwGetTime();
        float elapsed_time = total_time - prev_time;
		
		color_time += elapsed_time;

		backgroundX = (color_time < 5.0f) ? (127.f - color_time * 127.f / 5.f) / 255.0f : (color_time - 5.0f) / 5.0f * 127.0f / 255.0f;
		backgroundY = (color_time < 5.0f) ? (191.f - color_time * 191.f / 5.f) / 255.0f : (color_time - 5.0f) / 5.0f * 191.0f / 255.0f;
		backgroundZ = (color_time < 5.0f) ? (255.f - color_time * 255.f / 5.f) / 255.0f : (color_time - 5.0f) / 5.0f * 255.0f / 255.0f;

		if (color_time > 10.0f) {
			color_time = 0.f;
		}

        prev_time = total_time;

		if (head_spin_mouseR)
		{
			snowman.rotate_head(elapsed_time);
		}
		if (head_spin_mouseL)
		{
			snowman.rotate_head(-elapsed_time);
		}
		if (body_spin_mouseR)
		{
			snowman.rotate_body(elapsed_time);
		}
		if (body_spin_mouseL)
		{
			snowman.rotate_body(-elapsed_time);
		}
		if (body_jump || snowman.isJumping)
		{
			snowman.jump(elapsed_time);
		}
		if (move_foward) {
			snowman.move4direction(elapsed_time, 0);
		}
		if (move_left) {
			snowman.move4direction(elapsed_time, 1);
		}
		if (move_back) {
			snowman.move4direction(elapsed_time, 2);
		}
		if (move_right) {
			snowman.move4direction(elapsed_time, 3);
		}
		if (move_foward || move_left || move_back || move_right) {
			snowman.legMove(elapsed_time);
		}
		if (hand_shake || snowman.isShaking)
		{
			snowman.handShaking(elapsed_time);
		}
		if (body_spin_Q) {
			snowman.rotate_body(elapsed_time);
		}
		if (body_spin_E) {
			snowman.rotate_body(-elapsed_time);
		}
        // First Pass: Object Selection (Slide No. 20)
        // this is for picking the object using mouse interaction
        // binding framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
        // Background: RGB = 0x000000 => objectID: 0
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render your objects that you want to select using mouse interaction here
		for (int i = 0; i < snowmanParts.size(); i++) {
			PickableObject current = *snowmanParts[i];
			current.RenderPicking(main_camera);
		}
		background.RenderPicking(main_camera);
        
        // Second Pass: Object Rendering (Slide No. 11)
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor((GLclampf) backgroundX, (GLclampf) backgroundY, (GLclampf) backgroundZ, (GLclampf) 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Todo: Render object with main camera in the loop
	
		for (int i = 0; i < snowmanParts.size(); i++) {
			if (i == 0) { // for white part : body, head
				material->UpdateColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (i == 2 || i == 13) { // for brown part : arms, legs
				material->UpdateColor(glm::vec4(140.0f / 256.0f, 76.0f / 256.0f, 0.0f, 1.0f));
			}
			else if (i == 6 || i == 9) { // for black part : hat, eye
				material->UpdateColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			}
			else if (i == 8) { // for red part : hat belt
				material->UpdateColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else if (i == 12) { // for orange part : nose
				material->UpdateColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			}
			PickableObject current = *snowmanParts[i];
			current.Render(main_camera);
		}
		material->UpdateColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		background.Render(main_camera);
		material->UpdateColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		backgroundSun.Render(main_camera);
		
		material->UpdateColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		animation->Animate(main_camera, elapsed_time);
        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Delete resources
    delete main_camera;

    glfwTerminate();
    return 0;

}