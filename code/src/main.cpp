#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"	//// include the API header

#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)

#define WINDOW_TITLE "nSim"

const int numPoints = 3;  // Define the number of points
Vector2 points[numPoints] = {  // Correct array declaration
        {0, 0},
        {0, SCREEN_HEIGHT},
        {SCREEN_WIDTH, SCREEN_HEIGHT}
};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Texture2D texture = LoadTexture(ASSETS_PATH"/test.png"); // Check README.md for how this works

    rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme




    while (!WindowShouldClose())
    {
        BeginDrawing();
        rlImGuiBegin();

        DrawCube({-5,0,0}, 2, 2, 2, RED);

        ClearBackground(RAYWHITE);

        const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
        const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;
        DrawTexture(texture, texture_x, texture_y, WHITE);

        const char* text = "OMG! IT WORKS!";
        const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, BLACK);

        bool open = true;
        ImGui::ShowDemoWindow(&open);



        DrawSplineBezierQuadratic(points, numPoints, 4.0f, BLUE);

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
