#include "Renderer.h"
#include "Camera.h"


Renderer::Renderer(const RendererOption& rendererOption)
	:rendererOption(rendererOption)
{

}

void Renderer::SetCamera(const std::shared_ptr<Camera>& camera)
{
	this->camera = camera;
}