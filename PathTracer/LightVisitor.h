#pragma once

class PointLight;
class AreaLight;

class LightVisitor
{
public:
	virtual ~LightVisitor() = default;
	virtual void Visit(const PointLight& light) = 0;
	virtual void Visit(const AreaLight& light) = 0;
	
};
