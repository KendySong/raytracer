#include <cstdint>
#include <iostream>

#include <SDL/SDL_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "Graphics.hpp"
#include "../Math/Math.hpp"
#include "../Settings.hpp"

Graphics::Graphics(SDL_Window* window, SDL_Renderer* graphics, Camera* camera) : p_window(window), p_graphics(graphics), p_camera(camera)
{
	m_showResolutionX = resolutionX;
	m_showResolutionY = resolutionY;
	m_resolution = Vec2(resolutionX, resolutionY);
	m_aspectRatio = m_resolution.x / m_resolution.y;

	p_frontBuffer = new std::uint32_t[0];
	p_backBuffer = new std::uint32_t[0];
	this->resetFrameBuffer();

	m_lightPos = Vec3(-1, -1, 1);
	m_renderOnce = false;
}

void Graphics::clear()
{
	SDL_RenderSetLogicalSize(p_graphics, resolutionX, resolutionY);
	SDL_SetRenderDrawColor(p_graphics, 0, 0, 0, 255);
	SDL_RenderClear(p_graphics);

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(p_window);
	ImGui::NewFrame();
}

void Graphics::draw()
{
	for (int y = 0; y < resolutionY; y++)
	{
		for (int x = 0; x < resolutionX; x++)
		{
			/*
			//Normalize coordinates
			Vec2 coord(x / m_resolution.x, y / m_resolution.y);
			coord = coord * 2 - 1;
			coord.x *= m_aspectRatio;

			//Create ray and rotate it for simulate the camera
			Ray ray(p_camera->position, Math::normalize(Vec3(coord.x, coord.y, p_camera->position.z)));
			ray.direction = Math::rotateX(ray.direction, p_camera->rotation.x);
			ray.direction = Math::rotateY(ray.direction, p_camera->rotation.y);
		
			Vec3 origin = ray.origin - m_spheres[0].position;

			//ax^2 + bx + c
			//Resolve equation for detect if the ray have an intersection with the sphere
			float a, b, c, discriminant;
			a = Math::dot(ray.direction, ray.direction);
			b = 2 * Math::dot(origin, ray.direction);
			c = Math::dot(origin, origin) - pow(m_spheres[0].radius, 2);
			discriminant = pow(b, 2) - 4 * a * c;

			if (discriminant > 0)
			{
				float t = (-b - sqrt(discriminant)) / (2 * a);

				if (t < 0)
				{
					p_backBuffer[y * resolutionX + x] = this->getColor(0, 170, 255);
					continue;
				}

				//Compute scalar for getting intersection point
				Vec3 hit = ray.at(t);

				//Compute normal vector and light intensity
				Vec3 normal = hit - m_spheres[0].position;
				float intensity = Math::dot(Math::normalize(normal), Math::normalize(m_lightPos - m_spheres[0].position));
				intensity = intensity < 0 ? 0 : intensity > 1 ? 1 : intensity;
				p_backBuffer[y * resolutionX + x] = this->getColor(0, intensity * 255, 0);
			}
			else
			{
				p_backBuffer[y * resolutionX + x] = this->getColor(0, 170, 255);
			}	
			*/

			
			p_closestSphere = nullptr;
			m_closestDist = FLT_MAX;

			//Normalize coordinates
			Vec2 coord(x / m_resolution.x, y / m_resolution.y);
			coord = coord * 2 - 1;
			coord.x *= m_aspectRatio;

			//Create ray and rotate it for simulate the camera
			Ray ray(p_camera->position, Math::normalize(Vec3(coord.x, coord.y, p_camera->position.z)));
			ray.direction = Math::rotateX(ray.direction, p_camera->rotation.x);
			ray.direction = Math::rotateY(ray.direction, p_camera->rotation.y);

			for (Sphere& sphere : m_spheres)
			{
				Vec3 origin = ray.origin - sphere.position;

				//ax^2 + bx + c
				//Resolve equation for detect if the ray have an intersection with the sphere
				float a, b, c, discriminant;
				a = Math::dot(ray.direction, ray.direction);
				b = 2 * Math::dot(origin, ray.direction);
				c = Math::dot(origin, origin) - pow(sphere.radius, 2);
				discriminant = pow(b, 2) - 4 * a * c;

				if (discriminant > 0)
				{
					float t = (-b - sqrt(discriminant)) / (2 * a);

					if (t < 0)
					{
						continue;
					}

					if (t < m_closestDist)
					{
						m_closestDist = t;
						p_closestSphere = &sphere;
					}
				}
				else
				{
					continue;
				}
			}

			if (p_closestSphere == nullptr)
			{
				p_backBuffer[y * resolutionX + x] = this->getColor(0, 170, 255);
				continue;
			}

			//Compute scalar for getting intersection point
			Vec3 hit = ray.at(m_closestDist);

			//Compute normal vector and light intensity
			Vec3 normal = hit - p_closestSphere->position;
			float intensity = Math::dot(Math::normalize(normal), Math::normalize(m_lightPos - p_closestSphere->position));
			intensity = intensity < 0 ? 0 : intensity > 1 ? 1 : intensity;
			p_backBuffer[y * resolutionX + x] = this->getColor(0, intensity * 255, 0);			
		}
	}
}

void Graphics::drawGui()
{
	SDL_RenderSetLogicalSize(p_graphics, SCREEN_X, SCREEN_Y);
	ImGui::Begin("Render");
		ImGui::Text("ms : %f", m_timeToRender);
		ImGui::Checkbox("Render frame per frame", &m_renderOnce);

		if (!m_renderOnce)
		{
			this->drawSwap();
		}
		else
		{
			if (ImGui::Button("Render frame"))
			{
				this->drawSwap();
			}
		}
		ImGui::Separator();

		ImGui::TextUnformatted("Camera");	
		ImGui::DragFloat3("Position", &p_camera->position.x);
		ImGui::DragFloat3("Rotation", &p_camera->rotation.x);
		ImGui::DragFloat3("Direction", &p_camera->getDirection().x);
		ImGui::SliderFloat("Speed", &p_camera->speed, 1, 50);

		ImGui::Separator();

		ImGui::TextUnformatted("Light");
		ImGui::PushID(0);
		ImGui::DragFloat3("Position", &m_lightPos.x);	
		ImGui::PopID();

		ImGui::Separator();

		ImGui::InputInt2("Resolution", &m_showResolutionX);	
		if (ImGui::Button("Apply resolution"))
		{
			resolutionX = m_showResolutionX;
			resolutionY = m_showResolutionY;
			m_resolution.x = resolutionX;
			m_resolution.y = resolutionY;
			m_aspectRatio = m_resolution.x / m_resolution.y;
			this->resetFrameBuffer();
		}

		ImGui::Separator();

		ImGui::TextUnformatted("Help");
		ImGui::TextUnformatted("Control the camera : right click");
		ImGui::TextUnformatted("Move : wasd");

	ImGui::End();

	ImGui::Begin("Scene");
	for (size_t i = 0; i < m_spheres.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Sphere[%i]", i);
		ImGui::DragFloat3("Position", &m_spheres[i].position.x);
		ImGui::SliderFloat("Radius", &m_spheres[i].radius, 0, 10);
		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::End();
}

void Graphics::render()
{
	SDL_UpdateTexture(p_frontTex, NULL, p_frontBuffer, resolutionX * sizeof std::uint32_t);
	SDL_RenderCopy(p_graphics, p_frontTex, NULL, NULL);

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(p_graphics);
}

std::uint32_t Graphics::getColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	std::uint32_t red = (0x000000FF & r) << 16;
	std::uint32_t green = (0x000000FF & g) << 8;
	std::uint32_t blue = (0x000000FF & b);
	std::uint32_t result = 0xFF000000 | red | green | blue;
	return result;
}

SDL_Color Graphics::getColor(std::uint32_t colorARGB)
{
	SDL_Color color;
	color.a = (colorARGB & 0xFF000000) >> 24;
	color.r = (colorARGB & 0x00FF0000) >> 16;
	color.g = (colorARGB & 0x0000FF00) >> 8;
	color.b = (colorARGB & 0x00FF00FF);
	return color;
}

SDL_Renderer* Graphics::getRenderer() noexcept
{
	return p_graphics;
}

void Graphics::setSpheres(std::vector<Sphere>& spheres)
{
	m_spheres = spheres;
}

void Graphics::resetFrameBuffer()
{
	delete p_frontBuffer;
	delete p_backBuffer;
	p_frontBuffer = new std::uint32_t[resolutionX * resolutionY];
	p_backBuffer = new std::uint32_t[resolutionX * resolutionY];
	memset(p_frontBuffer, 0, resolutionX * resolutionY * sizeof std::uint32_t);
	memset(p_backBuffer, 0, resolutionX * resolutionY * sizeof std::uint32_t);
	p_frontTex = SDL_CreateTexture(p_graphics, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, resolutionX, resolutionY);
	p_backTex = SDL_CreateTexture(p_graphics, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, resolutionX, resolutionY);
}

void Graphics::drawSwap()
{
	//Draw sphere and swap buffers
	m_frameChrono.restart();
	this->draw();
	m_timeToRender = m_frameChrono.getElapsedTime() * 1000;

	std::uint32_t* tempBuffer = p_frontBuffer;
	p_frontBuffer = p_backBuffer;
	p_backBuffer = tempBuffer;
}