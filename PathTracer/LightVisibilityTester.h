#pragma once
#include <memory>
#include <vector>

#include "LightVisitor.h"

struct IntersectInfo;
class SceneObject;
class Light;
class Ray;

class LightVisibilityTester : public LightVisitor
{
public:
	LightVisibilityTester(const Ray& ray, IntersectInfo& info, const float& epsilon, const std::vector<std::shared_ptr<SceneObject>>& objects, const std::vector<std::shared_ptr<Light>>& lights);
	
	virtual void Visit(const PointLight& light) override;
	virtual void Visit(const AreaLight& light) override;

	bool operator()() const;

private:
	bool bVisible;
	const Ray& ray;
	IntersectInfo& info;
	const float& epsilon;
	const std::vector<std::shared_ptr<SceneObject>>& objects;
	const std::vector<std::shared_ptr<Light>>& lights;
};
