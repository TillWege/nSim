//
// Created by tillw on 04/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "cmath"

#define DEFAULT_DISTANCE 2500.0f

#define MOUSE_SPEED_HOR 0.1f
#define MOUSE_SPEED_VER 0.1f

struct SpericalCoord
{
	float r = 5000.0f;
	float theta = 90.0f;
	float phi = 90.0f;
};

struct CameraSettings
{
	Camera camera = {
		.position = { 0.0f, 2500.0f, 0.0f },
		.target = { 0.0f, 0.0f, 0.0f },
		.up = { 0.0f, 1.0f, 0.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE,
	};
	int zoom = 1;
	int cameraMode = CAMERA_CUSTOM;
	SpericalCoord coord;

	void update(Vector3 focusPos)
	{
		if(GetMouseWheelMove() != 0)
		{
			auto move = -GetMouseWheelMove();

			if(move > 0)
				this->coord.r *= 1.1f;
			else
				this->coord.r *= 0.9f;

		}

		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			auto delta = GetMouseDelta();

			auto deltaPhi = delta.x * MOUSE_SPEED_HOR;
			auto deltaTheta = -delta.y * MOUSE_SPEED_VER;

			this->coord.theta += deltaTheta;
			this->coord.phi += deltaPhi;

		}

		this->coord.phi = std::fmod(this->coord.phi, 360.0f);

		if(this->coord.theta > 180.0f)
			this->coord.theta = 180.0f;
		else if(this->coord.theta < 0.1f)
			this->coord.theta = 0.1f;


		auto offsetX = this->coord.r * std::sin(this->coord.theta * DEG2RAD) * std::cos(this->coord.phi * DEG2RAD);
		auto offsetZ = this->coord.r * std::sin(this->coord.theta * DEG2RAD) * std::sin(this->coord.phi * DEG2RAD);

		auto offsetY = this->coord.r * std::cos(this->coord.theta * DEG2RAD);


		this->camera.position.x = focusPos.x + offsetX;
		this->camera.position.y = focusPos.y + offsetY;
		this->camera.position.z = focusPos.z + offsetZ;

		this->camera.target = focusPos;
	}
};

inline CameraSettings cameraSettings;

#endif //CAMERA_H
