#pragma once

#include "Renderer.h"
#include <vector>
#include <GL/glew.h>
#include "Ray.h"

struct IntersectInfo;

class Shape;
class Light;

class CPURenderer : public Renderer
{
public:
	CPURenderer(RendererOption rendererOption);
	virtual ~CPURenderer() override;

	void Init() override;
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

	glm::vec3 CastRay(const Ray& ray, int maxDepth, float epsilon = 0.005f);

	bool TraceRay(const Ray& ray, IntersectInfo& info, float epsilon);
	// --------------------------------------------------------

private:
	double deltaTime = 0;


	// Tracing View
	int frame = 1;
	GLuint rayTextureID, rayShaderID, rayVAO, rayVBO;
	float* frameBuffer;

	// OpenGL View
	GLuint shaderID, lightID, colorID, matrixID, viewMatrixID, modelMatrixID;

	bool renderOpenGL;


	// Scene
	std::vector<std::shared_ptr<Shape>> shapes;
	std::vector<std::shared_ptr<Light>> lights;

};