#include "CPURenderer.h"
#include <iostream>
#include <FreeImage/FreeImage.h>
#include <glm/common.hpp>

#include "AreaLight.h"
#include "GLFWManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "IntersectInfo.h"
#include "Random.h"
#include "Common.h"
#include "LightVisibilityTester.h"
#include "Math.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Tracing.h"

CPURenderer::CPURenderer(const RendererOption& rendererOption)
	: Renderer(rendererOption), renderOpenGL(true)
{
	FreeImage_Initialise();
}

CPURenderer::~CPURenderer()
{
	std::cout << "[Renderer] Release CPURenderer" << std::endl;
	glDeleteTextures(1, &rayTextureID);
	FreeImage_DeInitialise();
	delete frameBuffer;
}

void CPURenderer::Init()
{
	std::cout << "[Renderer] Init CPURenderer" << std::endl;
	GLFWManager::Init(rendererOption.width, rendererOption.height, "CPU Tracer", this);

	{
		// OpenGL
		shaderID = ShaderLoader::Load("vert.glsl", "frag.glsl");
		matrixID = glGetUniformLocation(shaderID, "MVP");
		viewMatrixID = glGetUniformLocation(shaderID, "V");
		modelMatrixID = glGetUniformLocation(shaderID, "M");
		lightID = glGetUniformLocation(shaderID, "LightPosition_worldspace");
		colorID = glGetUniformLocation(shaderID, "Color");
	}

	{
		// RayTracing
		const int bufferSize = rendererOption.width * rendererOption.height * 3;
		frameBuffer = new float[bufferSize];
		std::fill(frameBuffer, frameBuffer + bufferSize, 0.0f);

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &rayTextureID);
		glBindTexture(GL_TEXTURE_2D, rayTextureID);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rendererOption.width, rendererOption.height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		GLfloat vertices[] =
		{
			// positions        // normal          // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
			1.0f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		};

		rayShaderID = ShaderLoader::Load("vertTracingView.glsl", "fragTracingView.glsl");

		glGenVertexArrays(1, &rayVAO);
		glGenBuffers(1, &rayVBO);

		glBindVertexArray(rayVAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, rayVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		}
		glBindVertexArray(0);
	}

	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.WantCaptureKeyboard = true;
		io.WantCaptureMouse = true;
		ImGui_ImplGlfw_InitForOpenGL(GLFWManager::GetWindow(), false);
		ImGui_ImplOpenGL3_Init("#version 430");
		ImGui::StyleColorsDark();
	}

	{
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 1040, 0), 1000, glm::vec3(0.25f, 0.75f, 0.25f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, -1000, 0), 1000)));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(1040, 0, 0), 1000, glm::vec3(0.75f, 0.25f, 0.25f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(-1040, 0, 0), 1000)));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 0, 1040), 1000, glm::vec3(0.25f, 0.25f, 0.75f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 0, -1040), 1000)));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(20, 0, 14), 8, glm::vec3(1.0f, 0.15f, 0.15f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(-14, 0, -20), 8, glm::vec3(0.15f, 1.0f, 1.0f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(14, 0, -20), 8, glm::vec3(0.15f, 0.15f, 1.0f))));
		//shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 5, 0), 8, glm::vec3(1.0f), glm::vec3(5))));
		
		//lights.push_back(std::make_shared<PointLight>(PointLight(glm::vec3(0, 10, 0), glm::vec3(1), 25)));
		std::shared_ptr<Sphere> sphereLightShape = std::make_shared<Sphere>(Sphere(glm::vec3(0, 10, 0), 4, glm::vec3(1.0f), glm::vec3(3.0f)));
		lights.push_back(std::make_shared<AreaLight>(AreaLight(sphereLightShape)));
	}
}

void CPURenderer::SetCamera(const std::shared_ptr<Camera>& camera)
{
	Renderer::SetCamera(camera);
	camera->UpdateScreen(rendererOption.width, rendererOption.height);
	camera->UpdateCamera(0);
	std::cout << "[Renderer] Set Camera To Renderer" << std::endl;
}

void CPURenderer::Start()
{
	std::cout << "[Renderer] Start CPURenderer" << std::endl;

	double lastTime = glfwGetTime();
	while (GLFWManager::WindowShouldClose() == 0)
	{
		glfwPollEvents();

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		if (camera->dirty)
		{
			frame = 1;
			camera->ResetDirty();
		}

		Update(deltaTime);
		Render(deltaTime);
	}
}


void CPURenderer::Release()
{
	GLFWManager::Release();

	for (auto& shape : shapes)
	{
		shape->ClearOpenGL();
	}
}

void CPURenderer::HandleKeyboard(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		frame = 1;
		renderOpenGL = !renderOpenGL;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		const unsigned int& width = rendererOption.width;
		const unsigned int& height = rendererOption.height;
		
		GLubyte *pixels = new GLubyte[3 * width * height];
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
		{
			const unsigned bytesperpixel = FreeImage_GetBPP(image) / 8;
			const unsigned height = FreeImage_GetHeight(image);
			const unsigned pitch = FreeImage_GetPitch(image);
			const unsigned lineSize = FreeImage_GetLine(image);

			BYTE* line = FreeImage_GetBits(image);
			for (unsigned y = 0; y < height; ++y, line += pitch)
			{
				for (BYTE* pixel = line; pixel < line + lineSize; pixel += bytesperpixel)
				{
					std::swap(pixel[0], pixel[2]);
				}
			}
		}

		if (FreeImage_Save(FIF_PNG, image, "result.png"))
		{
			std::cout << "SAVE";
		}

		FreeImage_Unload(image);
		delete pixels;
	}
}

void CPURenderer::HandleMouseClick(int button, int action, int mods)
{
	if (GLFWManager::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		GLFWManager::SetCursorToPos(rendererOption.width / 2, rendererOption.height / 2);
		glfwSetInputMode(GLFWManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
		glfwSetInputMode(GLFWManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void CPURenderer::HandleMouseMotion(double xPos, double yPos)
{
	if (GLFWManager::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		int& width = rendererOption.width;
		int& height = rendererOption.height;

		GLFWManager::SetCursorToPos(width / 2, height / 2);
		glm::vec2 input = glm::vec2(width / 2 - xPos, height / 2 - yPos);
		camera->UpdateCamera(deltaTime, glm::vec2(0), input);
		GLFWManager::SetCursorToPos(width / 2, height / 2);
	}
}

void CPURenderer::HandleResize(int width, int height)
{
	rendererOption.width = width;
	rendererOption.height = height;

	camera->UpdateScreen(width, height);
	glViewport(0, 0, width, height);

	glBindTexture(GL_TEXTURE_2D, rayTextureID);
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); 
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	delete frameBuffer;
	frameBuffer = new float[width * height * 3];
	frame = 1;
}

void CPURenderer::Render(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const unsigned int& width = rendererOption.width;
	const unsigned int& height = rendererOption.height;

	if (renderOpenGL)
	{
		for (auto& shape : shapes)
		{
			glUseProgram(shaderID);
			glm::mat4 mvp = camera->proj * camera->view * shape->model;

			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &camera->view[0][0]);
			glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &shape->model[0][0]);
			glUniform3f(lightID, 0, 0, 0);
			glUniform3f(colorID, shape->color.r, shape->color.g, shape->color.b);

			shape->DrawOpenGL();

			glUseProgram(0);
		}
	}
	else
	{
#pragma omp parallel for
		for(int i = 0 ; i < width * height; i++)
		{
			int x = i % width;
			int y = i / width;

			Ray ray = camera->GenerateRay(x, y);

			glm::vec3 previousColor(frameBuffer[i * 3], frameBuffer[i * 3 + 1], frameBuffer[i * 3 + 2]);

			previousColor.r = glm::pow(previousColor.r, PathTracing::GAMMA_DECODING);
			previousColor.g = glm::pow(previousColor.g, PathTracing::GAMMA_DECODING);
			previousColor.b = glm::pow(previousColor.b, PathTracing::GAMMA_DECODING);

			glm::vec3 color = CastRay(ray, 5);

			const glm::vec3 resultColor = (previousColor * static_cast<float>(frame - 1) + color) / static_cast<float>(frame);
			frameBuffer[i * 3] = glm::pow(resultColor.r, PathTracing::GAMMA_COMPRESSION);
			frameBuffer[i * 3 + 1] = glm::pow(resultColor.g, PathTracing::GAMMA_COMPRESSION);
			frameBuffer[i * 3 + 2] = glm::pow(resultColor.b, PathTracing::GAMMA_COMPRESSION);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rayTextureID);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, frameBuffer);
		}
		glUseProgram(rayShaderID);

		glBindVertexArray(rayVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glUseProgram(0);

		frame++;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwMakeContextCurrent(GLFWManager::GetWindow());
	glfwSwapBuffers(GLFWManager::GetWindow());
}

void CPURenderer::Update(double deltaTime)
{
	glm::vec2 input = glm::vec2(GLFWManager::IsKeyDown(GLFW_KEY_W) ? 1 : GLFWManager::IsKeyDown(GLFW_KEY_S) ? -1 : 0, GLFWManager::IsKeyDown(GLFW_KEY_D) ? 1 : GLFWManager::IsKeyDown(GLFW_KEY_A) ? -1 : 0);
	camera->UpdateCamera(deltaTime, input);

	if (GLFWManager::IsKeyDown(GLFW_KEY_Z))
	{
		camera->HandleInput(deltaTime, GLFW_KEY_Z);
	}

	if (GLFWManager::IsKeyDown(GLFW_KEY_X))
	{
		camera->HandleInput(deltaTime, GLFW_KEY_X);
	}

	if (GLFWManager::IsKeyDown(GLFW_KEY_C))
	{
		camera->HandleInput(deltaTime, GLFW_KEY_C);
	}

	if (GLFWManager::IsKeyDown(GLFW_KEY_V))
	{
		camera->HandleInput(deltaTime, GLFW_KEY_V);
	}
}

glm::vec3 CPURenderer::CastRay(Ray& ray, int maxDepth, float epsilon)
{
	glm::vec3 L(0); // Lo
	glm::vec3 pathWeight(1.0f); // BSDF * Cos / PDF;
	for(int depth = 1; depth <= maxDepth; depth++)
	{
		IntersectInfo info{};
		if (!PathTracing::TraceRay(ray, info, epsilon, shapes, lights))
			break;
		
		const std::shared_ptr<Shape>& shape = info.shape.lock();
		if (shape == nullptr)
			break;

		if(depth == 1)
		{
			L += shape->emit;
		}

		const glm::vec3& hitWorldPoint = ray.origin + ray.direction * info.t;
		const glm::vec3& hitWorldNormal = info.normal;
		const glm::vec3& albedoColor = shape->color;
		const glm::vec3& f = albedoColor * glm::one_over_pi<float>();

		for (auto& light : lights)
		{
			float lightPdf, distance;
			glm::vec3 lightDir;
			glm::vec3 lightRadiance = light->Sample(hitWorldPoint, lightDir, lightPdf, distance);

			IntersectInfo lightInfo;
			Ray lightRay(hitWorldPoint, lightDir, 0, distance);

			LightVisibilityTester visibilityTester(lightRay, lightInfo, epsilon, shapes, lights);
			light->Accept(visibilityTester);

			if (!visibilityTester())
			{
				continue;
			}
			
			L += pathWeight * f * lightRadiance * glm::max(0.0f, glm::dot(hitWorldNormal, lightDir)) / lightPdf;
		}

		const float pdf = PathTracing::UniformHemispherePDF; // 새로운 Ray의 확률

		const auto[normal, tangent, biTangent] = Math::WorldToTangent(hitWorldNormal);
		const auto[cosTheta, r2] = PathTracing::RandomFloat2();
		glm::vec3 sampleTangent = PathTracing::UniformSampleHemisphere(cosTheta, r2);
		glm::vec3 sampleWorld = Math::TangentToWorld(sampleTangent, normal, tangent, biTangent);
		glm::vec3 sampleNormal = glm::normalize(sampleWorld);

		pathWeight = f * cosTheta / pdf;

		if(depth > 3)
		{
			float p = glm::min(0.5f, glm::max(pathWeight.x, glm::max(pathWeight.y, pathWeight.z)));

			if (p <= PathTracing::RandomFloat())
				break;

			pathWeight /= p;
		}

		ray = Ray(hitWorldPoint, sampleNormal, ray, 0);
	}

	return L;
}