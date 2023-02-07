#define _USE_MATH_DEFINES
#include <math.h>

#include <windows.h>
#include <ImGui/imgui.h>

#include <SDL/SDL.h>

#include "Camera.hpp"
#include "../Application.hpp"
#include "../Math/Math.hpp"
#include "../Settings.hpp"

Camera::Camera(Vec3 position, float speed, float sensitivity)
{
	this->speed = speed;
	m_sensitivity = sensitivity;
	m_up = Vec3(0, 1, 0);
	m_front = Vec3(0, 0, 1);

	m_rotationLimit = Math::toRadian(90);
	m_firstMovement = true;
	rotation = Vec3(0, M_PI / 2, 0);
	this->position = position;
}

void Camera::processMovement(float deltatime) noexcept
{
	if (GetKeyState('W') & 0x8000)
	{	
		position += m_front * deltatime * speed;
	}

	if (GetKeyState('A') & 0x8000)
	{
		position += Math::normalize(Math::cross(m_front, m_up)) * deltatime * speed;
	}

	if (GetKeyState('S') & 0x8000)
	{
		position -= m_front * deltatime * speed;
	}

	if (GetKeyState('D') & 0x8000)
	{
		position -= Math::normalize(Math::cross(m_front, m_up)) * deltatime * speed;
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		position.y -= deltatime * speed;
	}

	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		position.y += deltatime * speed;
	}

	if (GetKeyState(VK_RBUTTON) & 0x8000)
	{	
		this->processRotation();
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void Camera::processRotation() noexcept
{
	if (rotation.x > m_rotationLimit)
	{
		rotation.x = m_rotationLimit;
	}

	if (rotation.x < -m_rotationLimit)
	{
		rotation.x = -m_rotationLimit;
	}

	if (m_firstMovement)
	{
		m_lastMousePos.x = SCREEN_X / 2;
		m_lastMousePos.y = SCREEN_Y / 2;
		m_firstMovement = false;
	}

	int x, y;
	SDL_GetMouseState(&x, &y);

	Vec2 mousePos(x, y);
	Vec2 offset = mousePos - m_lastMousePos;
	rotation.y -= offset.x * m_sensitivity;
	rotation.x += offset.y * m_sensitivity;

	m_front.x = cos(rotation.y) * cos(rotation.x);
	m_front.y = sin(rotation.x);
	m_front.z = sin(rotation.y) * cos(rotation.x);

	m_front = Math::normalize(m_front);
}

Vec3& Camera::getDirection() noexcept
{
	return m_front;
}

float& Camera::getSensitivity() noexcept
{
	return m_sensitivity;
}