#include "raylib.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "rlImGui.h" //// include the API header
#include <cmath>
#include <string>
#include "vector"
#include "rlImGuiColors.h"

#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)

#define WINDOW_TITLE "nSim"

const int numPoints = 3;      // Define the number of points
Vector2 points[numPoints] = { // Correct array declaration
	{ 0, 0 },
	{ 0, SCREEN_HEIGHT },
	{ SCREEN_WIDTH, SCREEN_HEIGHT }};

enum CursorState
{
	Uninitialized, Locked, Unlocked
};

CursorState cursorState = Uninitialized;

void updateCursor()
{
	if (!IsCursorHidden() && cursorState == Locked)
	{
		DisableCursor();
	}

	if (cursorState == Uninitialized)
	{
		DisableCursor();
		cursorState = Locked;
	}

	if (IsKeyPressed(KEY_F1))
	{
		if (cursorState == Locked)
		{
			EnableCursor();
			cursorState = Unlocked;
		}
		else
		{
			DisableCursor();
			cursorState = Locked;
		}
	}
}

struct GraphicsDebugger
{
	bool showWireframe = false;
	bool showGrid = true;
	int gridSize = 20;
	int maxFPS = 100;
};

GraphicsDebugger graphicsDebugger;

void GraphicsDebuggerUI()
{
	ImGui::Begin("Graphics Debugger", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	{

		ImGui::Checkbox("Show Wireframe", &graphicsDebugger.showWireframe);
		ImGui::Checkbox("Show Grid", &graphicsDebugger.showGrid);
		ImGui::SliderInt("Grid Size", &graphicsDebugger.gridSize, 1, 100);
		ImGui::Separator();
		ImGui::Text("Fps: %d", GetFPS());
		ImGui::Text("Frame Time: %f", GetFrameTime());
		int prevMaxFPS = graphicsDebugger.maxFPS;
		ImGui::SliderInt("Max FPS", &graphicsDebugger.maxFPS, 1, 1000);
		if (prevMaxFPS != graphicsDebugger.maxFPS)
		{
			SetTargetFPS(graphicsDebugger.maxFPS);
		}

	}
	ImGui::End();
}

struct Body
{
	std::string name;
	float orbitHeight;
	float angle;
	float radius;
	float mass;
	Color color;
	struct
	{
		std::vector<Vector3> data;
		size_t maxCount = 100;
	} trail;

	void appendTrail(Vector3 point)
	{
		if (trail.data.size() >= trail.maxCount)
		{
			trail.data.erase(trail.data.begin());
		}
		trail.data.push_back(point);
	}

	void updateTrailSize(size_t size)
	{
		trail.maxCount = size;
		trail.data.clear();
		trail.data.reserve(size);
	}

	size_t trailCount()
	{
		return trail.data.size();
	}
};

void DrawBody(Body& body)
{
	Vector3 pos = { 0, 0, 0 };
	if (body.orbitHeight > 0)
	{
		pos.x = body.orbitHeight * cos(body.angle);
		pos.z = body.orbitHeight * sin(body.angle);
	}

	body.appendTrail(pos);

	if (graphicsDebugger.showWireframe)
		DrawSphereWires(pos, body.radius, 10, 10, body.color);
	else
		DrawSphere(pos, body.radius, body.color);

	for (size_t i = 1; i < body.trailCount(); i++)
	{
		DrawLine3D(body.trail.data[i - 1], body.trail.data[i], body.color);
	}

}

void BodyDebuggerUI(Body& body)
{

	std::string windowName = "Body Debugger: " + body.name;

	ImGui::Begin(windowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Text("Orbit Height: %f", body.orbitHeight);
		ImGui::Text("Angle: %f°", body.angle);
		ImGui::Text("Radius: %f", body.radius);
		ImGui::Text("Mass: %f", body.mass);

		ImGui::BeginChild("Trail", { 300, 300 });

		for (size_t i = 0; i < body.trailCount(); i++)
		{
			Vector3& point = body.trail.data[i];
			ImGui::Text("%zu: x: %f, y: %f, z: %f", i, point.x, point.y, point.z);
		}

		ImGui::EndChild();
	}
	ImGui::End();
}

std::vector<Body> bodies;

struct bodySettings
{
    std::string name;
    float orbitHeight;
    float radius;
    float mass;
    ImVec4 color = {1, 1, 1, 1};
};

bodySettings tempBody;

void NewBodyDebuggerUI()
{

    ImGui::SetNextWindowSize({300, 180});
    ImGui::Begin("New Body");

    ImGui::InputText("Name", &tempBody.name);
    ImGui::InputFloat("Orbit Height", &tempBody.orbitHeight);
    ImGui::InputFloat("Radius", &tempBody.radius);
    ImGui::InputFloat("Mass", &tempBody.mass);
    ImGui::ColorEdit4("Color", &tempBody.color.x);

    if (ImGui::Button("Create"))
    {
        Body newBody = {
            tempBody.name,
            tempBody.orbitHeight,
            0.0f,
            tempBody.radius,
            tempBody.mass,
            rlImGuiColors::Convert(tempBody.color),
        };
        tempBody = {};
        bodies.push_back(newBody);
    }
    ImGui::End();
}


int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	//Texture2D texture = LoadTexture(ASSETS_PATH "/test.png"); // Check README.md for how this works
	rlImGuiSetup(true);


	Camera camera = { 0 };
	camera.projection = CAMERA_PERSPECTIVE;
	camera.fovy = 45.0f;
	camera.position = { 0.0f, 10.0f, 10.0f };
	camera.target = { 0.0f, 0.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };

	SetExitKey(0);

	SetWindowFocused();

	Body sun = { "Sun", 0.0f, 0.0f, 2.0f, 10.0f, YELLOW};
	bodies.push_back(sun);

	Body earth = { "Earth", 7.0f, 0.0f, 0.5f, 0.1f, BLUE};
	bodies.push_back(earth);

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
			RAYWHITE
		);

		for (Body& body : bodies)
		{
			body.angle += GetFrameTime() * 0.5f;
			BodyDebuggerUI(body);
//			earthTrail->push({body.orbitHeight * cos(body.angle), 0, body.orbitHeight * sin(body.angle)});
		}


		BeginMode3D(camera);

		UpdateCamera(&camera, CAMERA_ORBITAL);


		if (graphicsDebugger.showGrid)
			DrawGrid(graphicsDebugger.gridSize, 1.0f);

		for (Body& body : bodies)
		{
			DrawBody(body);
			BodyDebuggerUI(body);
		}


        if(IsMouseButtonPressed(0))
        {
            auto pos = GetMouseRay(GetMousePosition(), camera);
            TraceLog(LOG_INFO, "Mouse ray POS: %f, %f, %f", pos.position.x, pos.position.y, pos.position.z);
            TraceLog(LOG_INFO, "Mouse ray DIR: %f, %f, %f", pos.direction.x, pos.direction.y, pos.direction.z);

        }

		EndMode3D();
		GraphicsDebuggerUI();
        NewBodyDebuggerUI();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();

	return 0;
}


