//
// Created by tillw on 04/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

struct CameraSettings
{
	Camera camera = {
		.position = { 0.0f, 500.0f, 0.0f },
		.target = { 0.0f, 0.0f, 0.0f },
		.up = { 0.0f, 0.0f, 1.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE,
	};
	int zoom = 1;
	int cameraMode = CAMERA_CUSTOM;

	void update()
	{
		if(GetMouseWheelMove() != 0)
		{
			this->zoom = std::max(this->zoom + int(-GetMouseWheelMove()), 1);
		}

		this->camera.position.y = 500.0f * this->zoom;
	}
};

inline CameraSettings cameraSettings;

#endif //CAMERA_H
