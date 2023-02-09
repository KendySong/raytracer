#include "Material.hpp"

Material::Material(Vec3 albedo, float roughness, float metallic)
{
    this->albedo = albedo;
    this->roughness = roughness;
    this->metallic = metallic;
}