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


class Trail {
public:
	Trail(int size) {
		data = new Vector2[size];
		this->size = size;
		this->top = 0;
	}

	~Trail() {
		delete[] data;
	}

	void push(Vector2 point) {
		if(this->top < this->size) {
			this->data[this->top] = point;
			this->top++;
		} else {
			memcpy(&this->data[0], &this->data[1], (this->size - 1) * sizeof(Vector2));
			this->data[top - 1] = point;
		}
	}

	Vector2* get() {
		return this->data;
	}

	int getTop() {
		return this->top;
	}

	Vector2 getPoint(int index) {
		return this->data[index];
	}

	int getSize() {
		return this->size;
	}


private:
	Vector2* data;
	int size;
	int top;

};

Trail* earthTrail = NULL;

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Texture2D texture = LoadTexture(ASSETS_PATH "/test.png"); // Check README.md for how this works

	rlImGuiSetup(true);

	earthTrail = new Trail(100);

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

		earthTrail->push({ earth_x, earth_y });

		DrawSplineLinear(earthTrail->get(), earthTrail->getTop(), 2.0f, SKYBLUE);

		DrawCircle(earth_x, earth_y, 20, BLUE);


		ImGui::Begin("Target Debug", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		{
			ImGui::Text("Currently Selected: Earth");

			int trailSize = earthTrail->getSize();
			ImGui::SliderInt("Trail Size", &trailSize, 1, 100);

			if (trailSize != earthTrail->getSize()) {
				delete earthTrail;
				earthTrail = new Trail(trailSize);
			}

			if(ImGui::Button("Reset Trail")) {
				delete earthTrail;
				earthTrail = new Trail(100);
			}


			ImGui::Text("Earth Trail");

			ImGui::BeginChild("Scrolling", {0, 300}, ImGuiChildFlags_Border);

			for (int i = 0; i < earthTrail->getTop(); i++) {
				auto point = earthTrail->getPoint(i);
				ImGui::Text("Point %d: (%f, %f)", i, point.x, point.y);
			}

			ImGui::EndChild();

		}
		ImGui::End();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();

	return 0;
}
