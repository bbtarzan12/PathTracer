#include "Sphere.h"

#include <utility>
#include <vector>
#include <GL/glew.h>
#include <iostream>

#include "Ray.h"
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "Random.h"

Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color, const glm::vec3& emit)
	: Shape(color, emit), center(center), radius(radius), isInited(false), VAO(0), VBO(0), EBO(0)
{
	model = glm::translate(center) * glm::scale(glm::vec3(radius, radius, radius));
	InitOpenGL();
}

bool Sphere::Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const
{
	glm::vec3 oc = ray.origin - center;

	float a = 1;
	float b = 2 * glm::dot(ray.direction, oc);
	float c = glm::dot(oc, oc) - radius * radius;


	float t0 = 0, t1 = 0;

	if (!SolveQuadratic(a, b, c, t0, t1))
	{
		return false;
	}

	if (t0 > ray.maxt || t1 < ray.mint)
	{
		return false;
	}

	tHit = t0;
	if (t0 < ray.mint)
	{
		tHit = t1;
		if (tHit > ray.maxt || tHit <= rayEpsilon)
			return false;
	}

	normal = glm::normalize(oc);
	return true;
}

float Sphere::GetArea() const
{
	return glm::two_pi<float>() * glm::two_pi<float>() * radius * radius;
}

std::tuple<glm::vec3, glm::vec3> Sphere::GetRandomPointOnSurface() const
{
	const auto[r1, r2] = PathTracing::RandomFloat2();
	const glm::vec3 randomPoint = model * glm::vec4(PathTracing::UniformSampleSphere(r1, r2), 1.0f);
	const glm::vec3 randomNormal = glm::normalize(model * glm::vec4(randomPoint - center, 0.0f));
	
	return std::make_tuple(randomPoint, randomNormal);
}

void Sphere::InitOpenGL()
{
	int lats = 40;
	int longs = 40;

	std::vector <GLfloat> vertexBuffer;
	std::vector<GLuint> indices;
	int indicator = 0;
	for (int i = 0; i <= lats; i++)
	{
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = glm::sin(lat0);
		double zr0 = glm::cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = glm::sin(lat1);
		double zr1 = glm::cos(lat1);

		for (int j = 0; j <= longs; j++)
		{
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x = glm::cos(lng);
			double y = glm::sin(lng);

			glm::vec3 vertex1(x * zr0, y * zr0, z0);
			glm::vec3 normal1 = glm::normalize(vertex1);

			vertexBuffer.push_back(vertex1.x);
			vertexBuffer.push_back(vertex1.y);
			vertexBuffer.push_back(vertex1.z);
			vertexBuffer.push_back(normal1.x);
			vertexBuffer.push_back(normal1.y);
			vertexBuffer.push_back(normal1.z);
			indices.push_back(indicator);
			indicator++;

			glm::vec3 vertex2(x * zr1, y * zr1, z1);
			glm::vec3 normal2 = glm::normalize(vertex2);

			vertexBuffer.push_back(vertex2.x);
			vertexBuffer.push_back(vertex2.y);
			vertexBuffer.push_back(vertex2.z);
			vertexBuffer.push_back(normal2.x);
			vertexBuffer.push_back(normal2.y);
			vertexBuffer.push_back(normal2.z);
			indices.push_back(indicator);
			indicator++;
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(GLfloat), &vertexBuffer[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glBindVertexArray(VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	glBindVertexArray(0);

	numsToDraw = indices.size();
	isInited = true;
}

void Sphere::DrawOpenGL()
{
	if (!isInited)
	{
		std::cout << "please call InitOpenGL() before DrawOpenGL()" << std::endl;
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLE_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Sphere::ClearOpenGL()
{
	if (!isInited)
	{
		return;
	}
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
	}
	if (EBO)
	{
		glDeleteBuffers(1, &EBO);
	}
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
	}

	isInited = false;
	VAO = 0;
	EBO = 0;
	VBO = 0;
}

bool Sphere::SolveQuadratic(const float a, const float b, const float c, float& t0, float& t1) const
{
	float det = b * b - 4.0f * a * c;

	if (det <= 0)
		return false;

	det = glm::sqrt(det);

	float q = 0;
	if (b < 0)
	{
		q = -0.5f * (b - det);
	}
	else
	{
		q = -0.5f * (b + det);
	}

	t0 = q / a;
	t1 = c / q;

	if (t0 > t1)
		std::swap(t0, t1);

	return true;
}
