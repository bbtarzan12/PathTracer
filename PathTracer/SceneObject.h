#pragma once
#include <memory>


class Shape;
class Material;

class SceneObject
{
public:
	SceneObject(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material);

	std::weak_ptr<Shape> GetShape() const;
	std::weak_ptr<Material> GetMaterial() const;
	
private:
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Material> material;
	
};
