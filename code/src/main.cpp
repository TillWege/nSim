#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h" //// include the API header
#include <cmath>
#include <iostream>
#include <stack>

#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)

#define WINDOW_TITLE "nSim"

const int numPoints = 3;      // Define the number of points
Vector2 points[numPoints] = { // Correct array declaration
    {0, 0},
    {0, SCREEN_HEIGHT},
    {SCREEN_WIDTH, SCREEN_HEIGHT}};

const int trailSize = 100;
Vector2 trail[trailSize];

int trailIndex = 0;

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

         const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
         const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;
         DrawTexture(texture, texture_x, texture_y, WHITE);

         const char *text = "OMG! IT WORKS!";
         const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
         DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, BLACK);

         bool open = true;
         ImGui::ShowDemoWindow(&open);

         DrawSplineBezierQuadratic(points, numPoints, 4.0f, BLUE);

        // Sun
        DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, YELLOW);

        // Earth

        float angle = GetTime() * 0.5f;

        float earth_x = SCREEN_WIDTH / 2 + 200 * cos(angle);
        float earth_y = SCREEN_HEIGHT / 2 + 200 * sin(angle);

		trail[trailIndex] = {earth_x, earth_y};
		trailIndex = (trailIndex + 1) % trailSize;

		if(trailIndex == 0)
			std::cout << "Trail reset" << std::endl;



		DrawSplineLinear(trail, trailIndex, 2.0f, SKYBLUE);
		DrawSplineLinear(trail + trailIndex, trailSize - trailIndex, 2.0f, SKYBLUE);


		DrawSplineLinear(trail, trailSize, 2.0f, SKYBLUE);

        DrawCircle(earth_x, earth_y, 20, BLUE);


        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
