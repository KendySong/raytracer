#pragma once
#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"

class Camera
{
public:
	Camera() = default;
	Camera(Vec3 position, float speed, float sensitivity);
	void processMovement(float deltatime) noexcept;
	void processRotation() noexcept;

	Vec3& getRotation() noexcept;
	Vec3& getDirection() noexcept;
	float& getSensitivity() noexcept;

	Vec3 position;
	float speed;

private:
	Vec3 m_rotation;
	Vec3 m_front;
	Vec3 m_up;
	float m_rotationLimit;
	float m_sensitivity;

	Vec2 m_mousePos;
	Vec2 m_lastMousePos;
	bool m_firstMovement;
};