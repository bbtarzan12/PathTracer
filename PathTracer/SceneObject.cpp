#include "SceneObject.h"

SceneObject::SceneObject(std::unique_ptr<Shape> shape, std::unique_ptr<Material> material)
	:shape(std::move(shape)), material(std::move(material))
{
	
}

const Shape* SceneObject::GetShape() const
{
	return shape.get();
}

const Material* SceneObject::GetMaterial() const
{
	return material.get();
}
