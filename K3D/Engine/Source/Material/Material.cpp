#include "stdafx.h"
#include "Material.h"

void K3D::Material::SetBaseColor(const Vector3 & color)
{
	this->_baseColor = color;
}

void K3D::Material::SetSpecular(const Vector3 & color, float power)
{
	this->_specular = color;
	this->_specularPower = power;
}

void K3D::Material::SetAmbient(const Vector3 & ambient)
{
	this->_ambient = ambient;
}

void K3D::Material::SetRoughness(float roughness)
{
	this->_roughness = roughness;
}

void K3D::Material::Metalness(float metalness)
{
	this->_metalness = metalness;
}

Vector3 & K3D::Material::GetBaseColor()
{
	return _baseColor;
}

Vector3 & K3D::Material::GetAmbient()
{
	return _ambient;
}

float & K3D::Material::GetRoughness()
{
	return _roughness;
}

float & K3D::Material::GetMetalness()
{
	return _metalness;
}
