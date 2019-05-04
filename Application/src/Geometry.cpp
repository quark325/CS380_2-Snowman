#include <Geometry.hpp>
#include <stdlib.h>

#define PI 3.1415926535

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

glm::vec4 vertices[8] = {
    glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, -1.0f, 1.0f)
};

glm::vec3 Geometry::ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c)
{
    glm::vec3 normal = glm::normalize(glm::cross(a_b.xyz() - a_a.xyz(), a_c.xyz() - a_a.xyz()));
    return normal;
}

// TODO: Fill up Geometry::Quad (Slide No. 9)
void Geometry::Quad(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d)
{
    /* a -- d
     * |    |
     * b -- c
     */
    // Compute normal for quad
    glm::vec4 n = glm::vec4(ComputeNormal(vertices[a_a], vertices[a_b], vertices[a_c]), 0.0f);
    // Triangle 1, Vertex a
    mesh->AddVertexData(vertices[a_a]);
    mesh->AddVertexData(n);
    // Triangle 1, Vertex b
    mesh->AddVertexData(vertices[a_b]);
    mesh->AddVertexData(n);
    // Triangle 1, Vertex c
    mesh->AddVertexData(vertices[a_c]);
    mesh->AddVertexData(n);
    // Triangle 2, Vertex a
    mesh->AddVertexData(vertices[a_a]);
    mesh->AddVertexData(n);
    // Triangle 2, Vertex c
    mesh->AddVertexData(vertices[a_c]);
    mesh->AddVertexData(n);
    // Triangle 2, Vertex d
    mesh->AddVertexData(vertices[a_d]);
    mesh->AddVertexData(n);
}

// TODO: Fill up Geometry::GenerateCube (Slide No. 8)
void Geometry::GenerateCube(Engine::Mesh* mesh)
{
    /*   5 -- 6
     *  /|   /|
     * 1(4)-2 7
     * |    |/
     * 0 -- 3
     */
    mesh->AddAttribute(4); // for position
    mesh->AddAttribute(4); // for normal

    // Add four quad for generating a cube
    Quad(mesh, 1, 0, 3, 2);
    Quad(mesh, 2, 3, 7, 6);
    Quad(mesh, 3, 0, 4, 7);
    Quad(mesh, 6, 5, 1, 2);
    Quad(mesh, 4, 5, 6, 7);
    Quad(mesh, 5, 4, 0, 1);

    mesh->SetNumElements(36);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateTriangleMesh(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4);

    glm::vec3 position_data[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f,1.0f, 0.0f)
    };

    for (int i = 0; i < 3; i++)
    {
        mesh->AddVertexData(position_data[i]);
        mesh->AddVertexData(1.0f);

    }
    mesh->SetNumElements(3);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateLine(Engine::Mesh* mesh)
{
    mesh->SetDrawMode(GL_LINES);
    mesh->AddAttribute(4);
    mesh->AddVertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    mesh->AddVertexData(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    mesh->SetNumElements(2);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateSnowflake(Engine::Mesh* mesh)
{
	mesh->AddAttribute(4);
	mesh->AddAttribute(4);

	float sz = 1.0f;
	glm::vec4 szvec4 = glm::vec4(sz, sz, 1.0f, 1.0f);

	glm::vec4 position_data[] = {
		glm::vec4(1.0f, -1.0f / 3.0f*sqrt(3), 0.01f, 1.0f),
		glm::vec4(0.0f, 2.0f / 3.0f*sqrt(3), 0.01f, 1.0f),
		glm::vec4(-1.0f, -1.0f / 3.0f*sqrt(3), 0.01f, 1.0f)
	};

	for (int i = 0; i < 3; i++) {
		position_data[i] *= szvec4;
	}

	int iter = 4;

	glm::vec4 n = glm::vec4(ComputeNormal(position_data[0], position_data[1], position_data[2]), 0.0f);
	mesh->AddVertexData(position_data[0]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(position_data[1]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(position_data[2]);
	mesh->AddVertexData(n);
	MakeVertex(mesh, position_data[0], position_data[1], position_data[2], iter);
	MakeVertex(mesh, position_data[1], position_data[2], position_data[0], iter);
	MakeVertex(mesh, position_data[2], position_data[0], position_data[1], iter);

	int num = 3;
	for (int i = 0; i < iter; i++) {
		num *= 4;
	}
	mesh->SetNumElements(num*2);
	mesh->CreateMesh();
	mesh->SetDrawMode(GL_TRIANGLES);

	return;
}

void Geometry::MakeVertex(Engine::Mesh* mesh, glm::vec4 pos1, glm::vec4 pos2, glm::vec4 pos3, int iter) {
	int i = 0;

	if (iter > 0)
	{
		//pos112, pos122 => for pos1, pos2
		//pos113, pos223 => for pos1 and pos3 / for pos2 and pos3
		glm::vec4 div3 = glm::vec4(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f);
		glm::vec4 pos112 = (pos1 + pos1 + pos2)*div3;
		glm::vec4 pos122 = (pos1 + pos2 + pos2)*div3;
		glm::vec4 pos113 = (pos1 + pos1 + pos3)*div3;
		glm::vec4 pos223 = (pos2 + pos2 + pos3)*div3;
		glm::vec4 sharp = (pos1 + pos1 + pos2 + pos2 - pos3)*div3;

		glm::vec4 n = glm::vec4(ComputeNormal(pos112, sharp, pos122), 0.0f);
		mesh->AddVertexData(pos112);
		mesh->AddVertexData(n);
		mesh->AddVertexData(sharp);
		mesh->AddVertexData(n);
		mesh->AddVertexData(pos122);
		mesh->AddVertexData(n);


		MakeVertex(mesh, pos1, pos112, pos113, iter - 1);
		MakeVertex(mesh, pos112, sharp, pos122, iter - 1);
		MakeVertex(mesh, sharp, pos122, pos112, iter - 1);
		MakeVertex(mesh, pos122, pos2, pos223, iter - 1);
	}
}

void Geometry::Quad4Sphere(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d, int divide_num)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */
	 // Compute normal for quad
	for (int iLimit = 0; iLimit < 2*divide_num-1; iLimit++) { // a = index a + index c
		int jLimit = (2 * divide_num - 1) - iLimit - 1;
		for (int i = 0; i < iLimit+1; i++) {
			const int invI = iLimit - i;
			const int intervalJ = (jLimit!=0) ? jLimit : 1;
			for (int j = 0; j < jLimit+1; j+= intervalJ) {
				const int invJ = jLimit - j;
				//printf("iLimit : %d, jLimit : %d, i: %d, j: %d\n", iLimit, jLimit, i, j);
				float posX = invI * vertices[a_a].x + i * vertices[a_c].x + invJ * vertices[a_b].x + j * vertices[a_d].x;
				float posY = invI * vertices[a_a].y + i * vertices[a_c].y + invJ * vertices[a_b].y + j * vertices[a_d].y;
				float posZ = invI * vertices[a_a].z + i * vertices[a_c].z + invJ * vertices[a_b].z + j * vertices[a_d].z;

				glm::vec4 pointA = glm::vec4(glm::normalize(glm::vec3(posX + vertices[a_a].x, posY + vertices[a_a].y, posZ + vertices[a_a].z)), 1.0f);
				glm::vec4 pointB = glm::vec4(glm::normalize(glm::vec3(posX + vertices[a_b].x, posY + vertices[a_b].y, posZ + vertices[a_b].z)), 1.0f);
				glm::vec4 pointC = glm::vec4(glm::normalize(glm::vec3(posX + vertices[a_c].x, posY + vertices[a_c].y, posZ + vertices[a_c].z)), 1.0f);
				glm::vec4 pointD = glm::vec4(glm::normalize(glm::vec3(posX + vertices[a_d].x, posY + vertices[a_d].y, posZ + vertices[a_d].z)), 1.0f);

				glm::vec4 n = glm::vec4(ComputeNormal(pointA, pointB, pointC), 0.0f);

				// Triangle 1, Vertex a
				mesh->AddVertexData(pointA);
				mesh->AddVertexData(n);
				// Triangle 1, Vertex b
				mesh->AddVertexData(pointB);
				mesh->AddVertexData(n);
				// Triangle 1, Vertex c
				mesh->AddVertexData(pointC);
				mesh->AddVertexData(n);
				// Triangle 2, Vertex a
				mesh->AddVertexData(pointA);
				mesh->AddVertexData(n);
				// Triangle 2, Vertex c
				mesh->AddVertexData(pointC);
				mesh->AddVertexData(n);
				// Triangle 2, Vertex d
				mesh->AddVertexData(pointD);
				mesh->AddVertexData(n);

			}
		}
	}
}

// TODO: Fill up Geometry::GenerateCube (Slide No. 8)
void Geometry::GenerateSphere(Engine::Mesh* mesh, int divide_num)
{
	/*   5 -- 6
	 *  /|   /|
	 * 1(4)-2 7
	 * |    |/
	 * 0 -- 3
	 */

	int num_element = 36 * (2 * divide_num-1) * (2 * divide_num-1);

	mesh->AddAttribute(4); // for position
	mesh->AddAttribute(4); // for normal

	// Add four quad for generating a cube
	Quad4Sphere(mesh, 1, 0, 3, 2, divide_num);
	Quad4Sphere(mesh, 2, 3, 7, 6, divide_num);
	Quad4Sphere(mesh, 3, 0, 4, 7, divide_num);
	Quad4Sphere(mesh, 6, 5, 1, 2, divide_num);
	Quad4Sphere(mesh, 4, 5, 6, 7, divide_num);
	Quad4Sphere(mesh, 5, 4, 0, 1, divide_num);


	mesh->SetNumElements(num_element);
	mesh->CreateMesh();
	return;
}

void Geometry::GenerateCylinder(Engine::Mesh* mesh, float hrRatio) {
	//make circle with 50 triagles, make side portion with 100 rectangle, h = 2, r = h*hrRatio
	int num_element = 600;

	mesh->AddAttribute(4); // for position
	mesh->AddAttribute(4); // for normal

	const float radius = 2.0f * hrRatio;

	Circle4Cylinder(mesh, 0.0f, radius);
	Circle4Cylinder(mesh, 2.0f, radius);
	Side4Cylinder(mesh, radius);

	mesh->SetNumElements(num_element);
	mesh->CreateMesh();
	return;
}

void Geometry::Circle4Cylinder(Engine::Mesh* mesh, float height, float radius) {
	const float deltaAngle = 2.0 * PI / 50.0f;

	glm::vec4 origin = glm::vec4(0.0f, height, 0.0f, 1.0f);
	glm::vec4 init0 = glm::vec4(1.0f, height, 0.0f, 1.0f);
	glm::vec4 init90 = glm::vec4(0.0f, height, 1.0f, 1.0f);

	glm::vec4 nUp = glm::vec4(ComputeNormal(init0, origin, init90), 0.0f);
	glm::vec4 nDown = glm::vec4(ComputeNormal(init90, origin, init0), 0.0f);
	
	for (int i = 0; i < 50; i++) {
		float initAngle = i * 2.0 * PI / 50.0f;
		glm::vec4 point1 = glm::vec4(radius * cos(initAngle), height, radius * sin(initAngle), 1.0f);
		glm::vec4 point2 = glm::vec4(radius * cos(initAngle+deltaAngle), height, radius * sin(initAngle+deltaAngle),  1.0f);
		if (height > 1.0f) { //upper circle
			mesh->AddVertexData(point1);
			mesh->AddVertexData(nUp);

			mesh->AddVertexData(origin);
			mesh->AddVertexData(nUp);

			mesh->AddVertexData(point2);
			mesh->AddVertexData(nUp);
		}
		else { // lower circle
			mesh->AddVertexData(point2);
			mesh->AddVertexData(nDown);

			mesh->AddVertexData(origin);
			mesh->AddVertexData(nDown);

			mesh->AddVertexData(point1);
			mesh->AddVertexData(nDown);
		}
	}
}

void Geometry::Side4Cylinder(Engine::Mesh* mesh, float radius) {
	/* a -- d
	 * |    |
	 * b -- c
	 */
	const float deltaAngle = 2.0 * PI / 50.0f;
	for (int i = 0; i < 50; i++) {
		float initAngle = i * 2.0 * PI / 50.0f;
		glm::vec4 pointA = glm::vec4(radius * cos(initAngle), 0.0f, radius * sin(initAngle), 1.0f);
		glm::vec4 pointB = glm::vec4(radius * cos(initAngle), 2.0f, radius * sin(initAngle), 1.0f);
		glm::vec4 pointC = glm::vec4(radius * cos(initAngle + deltaAngle), 2.0f, radius * sin(initAngle + deltaAngle), 1.0f);
		glm::vec4 pointD = glm::vec4(radius * cos(initAngle + deltaAngle), 0.0f, radius * sin(initAngle + deltaAngle), 1.0f);

		glm::vec4 n = glm::vec4(ComputeNormal(pointA, pointB, pointC), 0.0f);

		// Triangle 1, Vertex a
		mesh->AddVertexData(pointA);
		mesh->AddVertexData(n);
		// Triangle 1, Vertex b
		mesh->AddVertexData(pointB);
		mesh->AddVertexData(n);
		// Triangle 1, Vertex c
		mesh->AddVertexData(pointC);
		mesh->AddVertexData(n);
		// Triangle 2, Vertex a
		mesh->AddVertexData(pointA);
		mesh->AddVertexData(n);
		// Triangle 2, Vertex c
		mesh->AddVertexData(pointC);
		mesh->AddVertexData(n);
		// Triangle 2, Vertex d
		mesh->AddVertexData(pointD);
		mesh->AddVertexData(n);
	}
}

void Geometry::GenerateCone(Engine::Mesh* mesh, float hrRatio) {
	//make circle with 50 triagles, make side portion with 50 triangle, h = 2, r = h*hrRatio
	int num_element = 300;

	mesh->AddAttribute(4); // for position
	mesh->AddAttribute(4); // for normal

	const float radius = 2.0f * hrRatio;

	Circle4Cylinder(mesh, 0.0f, radius);
	Side4Cone(mesh, radius);

	mesh->SetNumElements(num_element);
	mesh->CreateMesh();
	return;
}

void Geometry::Side4Cone(Engine::Mesh* mesh, float radius) {

	glm::vec4 origin = glm::vec4(0.0f, 2.0f, 0.0f, 1.0f);
	const float deltaAngle = 2.0 * PI / 50.0f;
	for (int i = 0; i < 50; i++) {
		float initAngle = i * 2.0 * PI / 50.0f;
		glm::vec4 pointA = glm::vec4(radius * cos(initAngle), 0.0f, radius * sin(initAngle), 1.0f);
		glm::vec4 pointB = glm::vec4(radius * cos(initAngle + deltaAngle), 0.0f, radius * sin(initAngle + deltaAngle), 1.0f);
		
		glm::vec4 n = glm::vec4(ComputeNormal(origin, pointB, pointA), 0.0f);

		// Triangle 1, Vertex origin
		mesh->AddVertexData(origin);
		mesh->AddVertexData(n);
		// Triangle 1, Vertex a
		mesh->AddVertexData(pointB);
		mesh->AddVertexData(n);
		// Triangle 1, Vertex b
		mesh->AddVertexData(pointA);
		mesh->AddVertexData(n);
	}
}

void Geometry::GenerateCircle(Engine::Mesh* mesh) {
	const float deltaAngle = 2.0 * PI / 50.0f;

	mesh->AddAttribute(4); // for position
	mesh->AddAttribute(4); // for normal

	glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 init0 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 init90 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 nUp = glm::vec4(ComputeNormal(init0, origin, init90), 0.0f);

	for (int i = 0; i < 50; i++) {
		float initAngle = i * 2.0 * PI / 50.0f;
		glm::vec4 point1 = glm::vec4(cos(initAngle), 0.0f, sin(initAngle), 1.0f);
		glm::vec4 point2 = glm::vec4(cos(initAngle + deltaAngle), 0.0f, sin(initAngle + deltaAngle), 1.0f);
		mesh->AddVertexData(point1);
		mesh->AddVertexData(nUp);

		mesh->AddVertexData(origin);
		mesh->AddVertexData(nUp);

		mesh->AddVertexData(point2);
		mesh->AddVertexData(nUp);
	}

	mesh->SetNumElements(300);
	mesh->CreateMesh();
}