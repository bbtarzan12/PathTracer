#pragma once

#include "Renderer.h"
#include <vector>
#include "Sphere.h"
#include <GL/glew.h>
#include "Ray.h"

struct IntersectInfo;

class CPURenderer : public Renderer
{
public:
	CPURenderer(RendererOption rendererOption);
	virtual ~CPURenderer() override;

	void Init();
	void SetCamera(const std::shared_ptr<Camera>& camera) override;
	void Start() override;
	void Release() override;
	void HandleKeyboard(int key, int scancode, int action, int mods) override;
	void HandleMouseClick(int button, int action, int mods) override;
	void HandleMouseMotion(double xPos, double yPos) override;
	void HandleResize(int width, int height) override;
	void Render(double deltaTime);

private:
	void Update(double deltaTime);

	// -------------------------------------------------------- RayTracing

	glm::vec3 CastRay(const Ray& ray, int maxDepth, int numIndirectSample = 10, float epsilon = 0.005f);

	bool TraceRay(const Ray& ray, IntersectInfo& info, float epsilon);

	// Normal을 Up Vector로 하는 좌표계 만들기
	void BuildLocalCoordinate(const glm::vec3& N, glm::vec3& Nt, glm::vec3& Nb);

	// Local 좌표계에서 다시 World 좌표계로 변환
	glm::vec3 LocalToWorld(const glm::vec3& v, const glm::vec3& N, const glm::vec3& Nt, const glm::vec3& Nb);
	// --------------------------------------------------------

private:
	double deltaTime = 0;


	// Tracing View
	int frame = 1;
	GLuint rayTextureID, rayShaderID, rayVAO, rayVBO;
	float* frameBuffer;

	// OpenGL View
	GLuint shaderID, lightID , matrixID, viewMatrixID, modelMatrixID;

	bool renderOpenGL;


	// Scene
	std::vector<std::shared_ptr<Shape>> shapes;

};