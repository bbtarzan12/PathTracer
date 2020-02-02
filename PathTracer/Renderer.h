#pragma once
#include <memory>

class Camera;

struct RendererOption
{
	int width, height;
};

class Renderer
{
public:
	Renderer(RendererOption rendererOption);
	virtual ~Renderer() = default;
	virtual void Init() = 0;
	virtual void SetCamera(const std::shared_ptr<Camera>& camera);
	virtual void Start() = 0;
	virtual void Release() = 0;
	virtual void HandleKeyboard(int key, int scancode, int action, int mods) = 0;
	virtual void HandleMouseClick(int button, int action, int mods) = 0;
	virtual void HandleMouseMotion(double xPos, double yPos) = 0;
	virtual void HandleResize(int width, int height) = 0;

protected:
	RendererOption rendererOption;
	std::shared_ptr<Camera> camera;
};