#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h" //// include the API header
#include <cmath>
#include <iostream>
#include <stack>
#include "vector"
#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)

#define WINDOW_TITLE "nSim"

const int numPoints = 3;      // Define the number of points
Vector2 points[numPoints] = { // Correct array declaration
	{ 0, 0 },
	{ 0, SCREEN_HEIGHT },
	{ SCREEN_WIDTH, SCREEN_HEIGHT }};


struct Trail {
	Vector2* data;
	int size;
	int top;

	Trail(int size) {
		data = new Vector2[size];
		this->size = size;
		top = 0;
	}

	~Trail() {
		delete[] data;
	}

	void push(Vector2 point) {
		if(top < size) {
			while (top < size) {
				data[top] = point;
				top++;
			}
		} else {
			memcpy(&data[0], &data[1], (size - 1) * sizeof(Vector2));
			data[top - 1] = point;
		}
	}

};

Trail earthTrail = Trail(100);

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Texture2D texture = LoadTexture(ASSETS_PATH "/test.png"); // Check README.md for how this works

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();

		ClearBackground(BLACK);

		const char* text = "Planet Sim";
		const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);

		DrawText(text,
			SCREEN_WIDTH / 2 - text_size.x / 2,
			30,
			20,
			RAYWHITE);

		// Sun
		DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, YELLOW);

		// Earth

		float angle = GetTime() * 0.5f;

		float earth_x = SCREEN_WIDTH / 2 + 200 * cos(angle);
		float earth_y = SCREEN_HEIGHT / 2 + 200 * sin(angle);

		earthTrail.push({ earth_x, earth_y });

		DrawSplineLinear(earthTrail.data, earthTrail.size, 2.0f, SKYBLUE);

		DrawCircle(earth_x, earth_y, 20, BLUE);

		ImGui::Begin("TrailDebug");
		{
			for (int i = 0; i < earthTrail.size; i++) {
				ImGui::Text("Trail[%d]: (%f, %f)", i, earthTrail.data[i].x, earthTrail.data[i].y);
			}
		}
		ImGui::End();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();

	return 0;
}
