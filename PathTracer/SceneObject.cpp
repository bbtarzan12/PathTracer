#include "SceneObject.h"

SceneObject::SceneObject(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material)
	:shape(std::move(shape)), material(std::move(material))
{
	
}

std::weak_ptr<Shape> SceneObject::GetShape() const
{
	return shape;
}

std::weak_ptr<Material> SceneObject::GetMaterial() const
{
	return material;
}
