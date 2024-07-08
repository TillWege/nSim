#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include <cmath>
#include <string>
#include "vector"
#include "Body.h"
#include "Loader.h"
#include "rlgl.h"
#include <thread>
#include "Camera.h"
#include "Consts.h"


struct GraphicsDebugger
{
	bool showWireframe = false;
	bool showSelector = true;
	bool showGrid = false;
	int gridSize = 20;
	int maxFPS = 100;
};

GraphicsDebugger graphicsDebugger;

void GraphicsDebuggerUI()
{
	ImGui::Begin("Graphics Debugger", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{

		ImGui::Checkbox("Show Wireframe", &graphicsDebugger.showWireframe);
		ImGui::Checkbox("Show Selectors", &graphicsDebugger.showSelector);
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

void DrawBody(Body& body)
{
    auto pos = body.getDisplayPosition();
	if (graphicsDebugger.showWireframe)
		DrawSphereWires(pos, body.getEffectiveRadius(), 16, 16, body.color);
	else
		DrawSphereEx(pos, body.getEffectiveRadius(), 16, 16, body.color);

	for (size_t i = 1; i < body.trailCount(); i++)
	{
		DrawLine3D(body.trail[i - 1], body.trail[i], body.color);
	}

	DrawLine3D(body.trail[body.trailCount() - 1], pos, body.color);

	if(!body.isPlanet and graphicsDebugger.showSelector)
	{
		Color selectorColor = body.color;
		selectorColor.a = 100;
		DrawSphereEx(pos, body.getEffectiveRadius() * 10, 16, 16, selectorColor);
	}
}

void BodyDebuggerUI(Body& body)
{

	std::string windowName = "Body Debugger: " + body.name;

	ImGui::Begin(windowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Text("Radius: %f", body.radius);
		ImGui::Text("Mass: %f", body.mass);

		ImGui::Text("Position: x: %f, y: %f, z: %f", body.position.x, body.position.y, body.position.z);
		ImGui::Text("Velocity: x: %f, y: %f, z: %f", body.velocity.x, body.velocity.y, body.velocity.z);

		ImGui::BeginChild("Trail", { 300, 300 });

		for (size_t i = 0; i < body.trailCount(); i++)
		{
			Vector3& point = body.trail[i];
			ImGui::Text("%zu: x: %f, y: %f, z: %f", i, point.x, point.y, point.z);
		}

		ImGui::EndChild();
	}
	ImGui::End();
}

std::vector<Body> bodies;

bool testRayHit(Vector3 cameraPos, Vector3 cameraDir, Vector3 spherePos, float sphereRadius)
{
	// 1. Vector from camera position to sphere center
	Vector3 oc = { spherePos.x - cameraPos.x, spherePos.y - cameraPos.y, spherePos.z - cameraPos.z };

	// 2. Calculate projections
	float ocSq = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z;
	float a = cameraDir.x * cameraDir.x + cameraDir.y * cameraDir.y + cameraDir.z * cameraDir.z;
	float b = 2.0f * (oc.x * cameraDir.x + oc.y * cameraDir.y + oc.z * cameraDir.z);

	// 3. Discriminant
	float discriminant = b * b - 4.0f * a * (ocSq - sphereRadius * sphereRadius);

	// 4. Check for solutions (intersections)
	if (discriminant < 0.0f)
	{
		// No intersection
		return false;
	}
	else
	{
		// One or two intersections (we just need to know if there is AT LEAST one)
		return true;
	}
}

float distanceToSphere(Vector3 cameraPos, Vector3 cameraDir, Vector3 spherePos, float sphereRadius)
{
	// 1. Vector from camera position to sphere center
	Vector3 oc = { spherePos.x - cameraPos.x, spherePos.y - cameraPos.y, spherePos.z - cameraPos.z };

	// 2. Calculate projections and distance to sphere center
	float ocSq = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z;
	float a = cameraDir.x * cameraDir.x + cameraDir.y * cameraDir.y + cameraDir.z * cameraDir.z;
	float b = 2.0f * (oc.x * cameraDir.x + oc.y * cameraDir.y + oc.z * cameraDir.z);

	// 3. Discriminant (same as in testRayHit)
	float discriminant = b * b - 4.0f * a * (ocSq - sphereRadius * sphereRadius);

	// 4. Check for solutions (intersections)
	if (discriminant < 0.0f)
	{
		// No intersection, return distance to sphere center minus radius
		return std::sqrt(ocSq) - sphereRadius;
	}
	else
	{
		// Intersection exists. Calculate distance to the closest intersection
		float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

		// Ensure t is positive (intersection in front of camera)
		float t = (t1 > 0.0f) ? t1 : t2;
		if (t < 0.0f)
		{
			// Both intersections behind camera, return distance to sphere center minus radius
			return std::sqrt(ocSq) - sphereRadius;
		}

		// Calculate intersection point and distance to it
		Vector3 intersectionPoint = {
			cameraPos.x + t * cameraDir.x,
			cameraPos.y + t * cameraDir.y,
			cameraPos.z + t * cameraDir.z
		};
		float distance = std::sqrt(
			(intersectionPoint.x - cameraPos.x) * (intersectionPoint.x - cameraPos.x) +
				(intersectionPoint.y - cameraPos.y) * (intersectionPoint.y - cameraPos.y) +
				(intersectionPoint.z - cameraPos.z) * (intersectionPoint.z - cameraPos.z)
		);

		return distance;
	}
}

struct bodySettings
{
	std::string name;
	float orbitHeight = 0.0f;
	float radius = 1.0f;
	float mass = 1.0f;
	ImVec4 color = { 1, 1, 1, 1 };
};

//void NewBodyDebuggerUI()
//{
//
//	ImGui::SetNextWindowSize({ 300, 180 });
//	ImGui::Begin("New Body");
//
//	static bodySettings tempBody;
//
//	ImGui::InputText("Name", &tempBody.name);
//	ImGui::InputFloat("Orbit Height", &tempBody.orbitHeight);
//	ImGui::InputFloat("Radius", &tempBody.radius);
//	ImGui::InputFloat("Mass", &tempBody.mass);
//	ImGui::ColorEdit4("Color", &tempBody.color.x);
//
//	if (ImGui::Button("Create"))
//	{
//		Body newBody = {
//			tempBody.name,
//			tempBody.orbitHeight,
//			0.0f,
//			tempBody.radius,
//			rlImGuiColors::Convert(tempBody.color),
//		};
//		tempBody = {};
//		bodies.push_back(newBody);
//	}
//	ImGui::End();
//}

void CameraSettingsDebuggerUI()
{
	ImGui::Begin("Camera Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Text("Camera Mode");
		if (ImGui::RadioButton("Orbital", &cameraSettings.cameraMode, CAMERA_ORBITAL))
		{
			cameraSettings.cameraMode = CAMERA_ORBITAL;
			cameraSettings.camera.target = { 0.0f, 0.0f, 0.0f };
		}

		if (ImGui::RadioButton("First Person", &cameraSettings.cameraMode, CAMERA_FIRST_PERSON))
			cameraSettings.cameraMode = CAMERA_FIRST_PERSON;

		if (ImGui::RadioButton("Free", &cameraSettings.cameraMode, CAMERA_FREE))
			cameraSettings.cameraMode = CAMERA_FREE;

		ImGui::Separator();
		ImGui::Begin("Camera Settings");
		ImGui::InputFloat3("Position", &cameraSettings.camera.position.x);
		ImGui::InputFloat3("Target", &cameraSettings.camera.target.x);
		ImGui::InputFloat3("Up", &cameraSettings.camera.up.x);

		ImGui::DragFloat3("Sphere Coord", &cameraSettings.coord.r);

		ImGui::SliderFloat("Fovy", &cameraSettings.camera.fovy, 30.0f, 90.0f);

		ImGui::End();

	}
	ImGui::End();
}

struct PerformanceStats
{
	int fps;
	int64_t secondsPassed = 0;
};

PerformanceStats performanceStats;

struct SimulationSettings
{
	int timeStep = TIME_STEP;
	bool paused = false;
};

SimulationSettings simulationSettings;

void SimulationSettingsDebuggerUI()
{

	performanceStats.fps = GetFPS();

	ImGui::Begin("Simulation Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Text("Seconds per sim-tick");
		ImGui::SliderInt("##simtick", &simulationSettings.timeStep, 1.0f, 10.0f);
		ImGui::SameLine();
		if(ImGui::Button("+1", { 50, 0 }))
			simulationSettings.timeStep += 1.0f;

		ImGui::SameLine();
		if(ImGui::Button("-1", { 50, 0 }))
			simulationSettings.timeStep -= 1.0f;

		ImGui::Checkbox("Paused", &simulationSettings.paused);

		ImGui::Text("Seconds passed in Simulation: %lld", performanceStats.secondsPassed);
		ImGui::Text("Minutes passed in Simulation: %lld", performanceStats.secondsPassed / 60);
		ImGui::Text("Hours passed in Simulation: %lld", performanceStats.secondsPassed / 3600);
		ImGui::Text("Days passed in Simulation: %lld", performanceStats.secondsPassed / 86400);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("Simulation Running");
		ImGui::PopStyleColor();
	}
	ImGui::End();
}

bool simRunning = true;

void simulate()
{
	while (simRunning)
	{

		if (!simulationSettings.paused)
		{
			// Skip the first body (sun)
			for (int i = 1; i < bodies.size(); i++)
			{
				for (int j = 0; j < bodies.size(); j++)
				{
                    Body& body = bodies[i];
                    Body& body2 = bodies[j];
					if (&body != &body2)
					{
                        SciVec3 direction = body2.position - body.position;

						double distance = direction.length();

                        double forceMagnitude = GRAV_CONST * ((body.mass * body2.mass) / (distance * distance));

						if (isinf(forceMagnitude) || isnan(forceMagnitude))
							TraceLog(LOG_ERROR, "Force magnitude is inf or nan");

                        SciVec3 forceDirection = direction.normalized();
                        SciVec3 force = forceDirection * forceMagnitude;

						SciVec3 acceleration = force / body.mass;

                        body.velocity += acceleration * simulationSettings.timeStep;
					}
				}
			}

            for (Body& body : bodies)
            {
                body.position += body.velocity * simulationSettings.timeStep;

				int stepSize = body.isPlanet ? PLANET_TRAIL_STEP : SATELLITE_TRAIL_STEP;

				if(performanceStats.secondsPassed % stepSize <= TIME_STEP)
				{
					body.appendTrail(body.getDisplayPosition());
				}

            }

			performanceStats.secondsPassed += simulationSettings.timeStep;
		}

	}
}

int focusIndex = 0;

void FocusSelectDebugUI()
{
	ImGui::Begin("Focus Select");
	{
		ImGui::Text("Select a body to focus on");
		for (int i = 0; i < bodies.size(); i++)
		{
			if (ImGui::Button(bodies[i].name.c_str()))
			{
				focusIndex = i;
			}
		}
	}
	ImGui::End();
}

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(1600, 900, WINDOW_TITLE);
	SetTargetFPS(60);

	rlImGuiSetup(true);
	SetTraceLogLevel(LOG_DEBUG);

	SetWindowFocused();

	loadPlanets(bodies);

	loadSatellites(bodies);

	std::thread simThread;

	simThread = std::thread(simulate);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		rlImGuiBegin();
		ClearBackground(BLACK);
		rlSetClipPlanes(0.01f, cameraSettings.coord.r * 2.0f);

		const char* text = "Planet Sim";
		const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);

		DrawText(text,
			GetScreenWidth() / 2 - text_size.x / 2,
			30,
			20,
			RAYWHITE
		);

		BeginMode3D(cameraSettings.camera);

		if (graphicsDebugger.showGrid)
			DrawGrid(graphicsDebugger.gridSize, cameraSettings.zoom * 100.f);


		simulationSettings.paused = true;
		for (Body& body : bodies)
		{
			DrawBody(body);
			//BodyDebuggerUI(body);
		}
		auto focusPos = bodies[focusIndex].getDisplayPosition();
		cameraSettings.update(focusPos);

		simulationSettings.paused = false;



		if (IsMouseButtonPressed(0))
		{
			auto pos = GetMouseRay(GetMousePosition(), cameraSettings.camera);
			//TraceLog(LOG_INFO, "Mouse ray POS: %f, %f, %f", pos.position.x, pos.position.y, pos.position.z);
			//TraceLog(LOG_INFO, "Mouse ray DIR: %f, %f, %f", pos.direction.x, pos.direction.y, pos.direction.z);

			std::vector<std::tuple<Body, float>> hitBodies;
			for (Body& body : bodies)
			{
				auto radius = body.getEffectiveRadius();
				if(!body.isPlanet)
					radius *= 10;
				if (testRayHit(pos.position, pos.direction, body.getDisplayPosition(), radius))
				{
					float dist = distanceToSphere(pos.position, pos.direction, body.getDisplayPosition(), radius);
					hitBodies.emplace_back(body, dist);
				}
			}

			for (auto& [body, dist] : hitBodies)
			{
				TraceLog(LOG_INFO, "Hit body: %s, distance: %f", body.name.c_str(), dist);
			}
		}





		EndMode3D();
		GraphicsDebuggerUI();
		//NewBodyDebuggerUI();
		CameraSettingsDebuggerUI();
		SimulationSettingsDebuggerUI();
		FocusSelectDebugUI();

		rlImGuiEnd();
		EndDrawing();
	}



	simRunning = false;
	simThread.join();

	rlImGuiShutdown();
	CloseWindow();

	return 0;
}


