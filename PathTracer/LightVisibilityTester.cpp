﻿#include "LightVisibilityTester.h"

#include "AreaLight.h"
#include "IntersectInfo.h"
#include "PointLight.h"
#include "Tracing.h"

LightVisibilityTester::LightVisibilityTester(const Ray& ray, IntersectInfo& info, const float& epsilon, const std::vector<std::shared_ptr<Shape>>& shapes, const std::vector<std::shared_ptr<Light>>& lights)
	:bVisible(false), ray(ray), info(info), epsilon(epsilon), shapes(shapes), lights(lights)
{
}

void LightVisibilityTester::Visit(const PointLight& light)
{
	bVisible = !PathTracing::TraceRay(ray, info, epsilon, shapes, lights);
}

void LightVisibilityTester::Visit(const AreaLight& light)
{
	bVisible = PathTracing::TraceRay(ray, info, epsilon, shapes, lights) && info.shape.lock() == light.GetShape();
}

bool LightVisibilityTester::operator()() const
{
	return bVisible;
}