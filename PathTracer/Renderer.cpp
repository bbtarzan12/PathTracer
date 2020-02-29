#include "Renderer.h"
#include "Camera.h"


Renderer::Renderer(const RendererOption& rendererOption)
	:rendererOption(rendererOption)
{

}

void Renderer::SetCamera(std::unique_ptr<Camera> camera)
{
	this->camera = std::move(camera);
}