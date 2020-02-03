#include "CPURenderer.h"
#include <iostream>
#include <FreeImage/FreeImage.h>
#include "GLFWManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "IntersectInfo.h"
#include "Random.h"
#include "Common.h"

CPURenderer::CPURenderer(RendererOption rendererOption)
	: Renderer(rendererOption), renderOpenGL(true)
{
	FreeImage_Initialise();
}

CPURenderer::~CPURenderer()
{
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
	}

	{
		// RayTracing
		frameBuffer = new float[rendererOption.width * rendererOption.height * 3];

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
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, -1010, 0), 1000)));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(1040, 0, 0), 1000, glm::vec3(0.75f, 0.25f, 0.25f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(-1040, 0, 0), 1000)));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 0, 1040), 1000, glm::vec3(0.25f, 0.25f, 0.75f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 0, -1040), 1000)));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(20, 0, 14), 8, glm::vec3(1.0f, 0.15f, 0.15f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(-14, 0, -20), 8, glm::vec3(0.15f, 1.0f, 1.0f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(14, 0, -20), 8, glm::vec3(0.15f, 0.15f, 1.0f))));
		shapes.push_back(std::make_shared<Sphere>(Sphere(glm::vec3(0, 1, 0), 10, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(5.0f, 5.0f, 5.0f))));
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
		glm::vec2 input = glm::vec2(height / 2 - yPos, width / 2 - xPos);
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
			glm::vec3 resultColor(0, 0, 0);

			previousColor.r = glm::pow(previousColor.r, PathTracing::GAMMA_DECODING);
			previousColor.g = glm::pow(previousColor.g, PathTracing::GAMMA_DECODING);
			previousColor.b = glm::pow(previousColor.b, PathTracing::GAMMA_DECODING);

			glm::vec3 color = CastRay(ray, 3, 4);

			resultColor = (previousColor * (float)(frame - 1) + color) / (float)frame;
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

glm::vec3 CPURenderer::CastRay(const Ray& ray, int maxDepth, int numIndirectSample, float epsilon)
{
	if (ray.depth > maxDepth)
		return glm::vec3(0, 0, 0);

	glm::vec3 resultColor(0, 0, 0);
	IntersectInfo info;

	if (TraceRay(ray, info, epsilon))
	{
		glm::vec3 hitPoint = ray.origin + ray.direction * info.t;
		glm::vec3 hitNormal = info.normal;

		// Diffuse Material

		// GLobal Indirect Lighting
		glm::vec3 indirectColor(0, 0, 0);
		glm::vec3 Nt, Nb;

		BuildLocalCoordinate(hitNormal, Nt, Nb);
		float pdf = PathTracing::UniformHemispherePDF;
		for (int i = 1; i <= numIndirectSample; i++)
		{
			float r1 = PathTracing::RandomFloat(); // theta
			float r2 = PathTracing::RandomFloat(); // phi
			glm::vec3 sample = PathTracing::UniformSampleHemisphere(r1, r2);
			glm::vec3 sampleWorld = LocalToWorld(sample, hitNormal, Nt, Nb);

			glm::vec3 sampleNormal = glm::normalize(sampleWorld);
			Ray indirectRay(hitPoint + sampleNormal * epsilon, sampleNormal, ray, 0);
			glm::vec3 newIndirectColor = r2 * CastRay(indirectRay, maxDepth, numIndirectSample, epsilon) / pdf; // theta 곱하기랑 pdf 나누기를 꼭 해야한다

			indirectColor = (indirectColor * (float)(i - 1) + newIndirectColor) / (float)i;

			float maxColor = glm::max(glm::max(indirectColor.r, indirectColor.g), indirectColor.b);
			if (PathTracing::RandomFloat() > maxColor)
				break;

			indirectColor *= 1 / maxColor;
		}

		// Object Material Albedo
		resultColor = (info.shape->emit * glm::one_over_pi<float>() + 2.0f * indirectColor) * info.shape->color;
	}

	return resultColor;
}

bool CPURenderer::TraceRay(const Ray& ray, IntersectInfo& info, float epsilon)
{
	info.t = PathTracing::INFINITE<float>;
	for (auto& shape : shapes)
	{
		float tHit = 0;
		glm::vec3 normal;
		if (shape->Intersect(ray, tHit, normal, epsilon) && tHit < info.t)
		{
			info.t = tHit;
			info.shape = shape;
			info.normal = normal;
		}
	}

	return info.shape != nullptr;
}

void CPURenderer::BuildLocalCoordinate(const glm::vec3& N, glm::vec3& Nt, glm::vec3& Nb)
{
	if (glm::abs(N.x) > 0.1f)
	{
		Nt = glm::normalize(glm::cross(glm::vec3(0, 1, 0), N));
	}
	else
	{
		Nt = glm::normalize(glm::cross(glm::vec3(1, 0, 0), N));
	}

	Nb = glm::cross(N, Nt);
}

glm::vec3 CPURenderer::LocalToWorld(const glm::vec3& v, const glm::vec3& N, const glm::vec3& Nt, const glm::vec3& Nb)
{
	return glm::vec3
	{
		v.x * Nt.x + v.y * Nb.x + v.z * N.x,
		v.x * Nt.y + v.y * Nb.y + v.z * N.y,
		v.x * Nt.z + v.y * Nb.z + v.z * N.z
	};
}
