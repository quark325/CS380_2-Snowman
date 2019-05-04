#pragma once

#include <vector>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Mesh.hpp>

class Geometry
{
private:

protected:
	Geometry *parent;
	std::vector<Geometry *> children;
public:
	Geometry();
	~Geometry();
	void GenerateCube(Engine::Mesh* mesh);
	void Quad(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d);
	glm::vec3 ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c);
	void GenerateTriangleMesh(Engine::Mesh* mesh);
	void GenerateLine(Engine::Mesh* mesh);

	void GenerateSphere(Engine::Mesh* mesh, int divide_num);
	void Quad4Sphere(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d, int divide_num);
	
	void GenerateCylinder(Engine::Mesh* mesh, float hrRatio);
	void Circle4Cylinder(Engine::Mesh* mesh, float height, float hrRatio);
	void Side4Cylinder(Engine::Mesh* mesh, float hrRatio);

	void GenerateCone(Engine::Mesh* mesh, float hrRatio);
	void Side4Cone(Engine::Mesh* mesh, float hrRatio);

	void GenerateSnowflake(Engine::Mesh* mesh);
	void MakeVertex(Engine::Mesh* mesh, glm::vec4 pos1, glm::vec4 pos2, glm::vec4 pos3, int iter);

	void GenerateCircle(Engine::Mesh* mesh);

	void AddChild(Geometry *geometry);
};
