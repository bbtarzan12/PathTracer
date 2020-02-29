#pragma once
#include <memory>

#include "Material.h"
#include "Shape.h"

class SceneObject
{
public:
	SceneObject(std::unique_ptr<Shape> shape, std::unique_ptr<Material> material);

	const Shape* GetShape() const;
	const Material* GetMaterial() const;
	
private:
	std::unique_ptr<Shape> shape;
	std::unique_ptr<Material> material;
	
};
