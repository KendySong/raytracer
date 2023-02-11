#include "Material.hpp"

Material::Material(Vec3 albedo, float roughness)
{
    this->albedo = albedo;
    this->roughness = roughness;
}