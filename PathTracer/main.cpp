#include "CPURenderer.h"
#include "PinHoleCamera.h"


int main()
{
	const int width = 1280 / 2;
	const int height = 720 / 2;

	RendererOption renderOption{width, height};

	std::unique_ptr<Renderer> renderer = std::make_unique<CPURenderer>(renderOption);

	const std::shared_ptr<Camera> camera = std::make_shared<PinHoleCamera>(PinHoleCamera({ width, height }, 70.0f, 10.0f, 0.0f, { 0.01f, 1000000.0f }));

	renderer->Init();
	renderer->SetCamera(camera);
	renderer->Start();
	renderer->Release();

	return 0;
}