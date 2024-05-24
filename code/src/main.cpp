#include "raylib.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "rlImGui.h" //// include the API header
#include <cmath>
#include <string>
#include "vector"
#include "rlImGuiColors.h"
#include <thread>

#define WINDOW_TITLE "nSim"
#define USE_SPINLOCK_TIMER true


enum CursorState {
    Uninitialized, Locked, Unlocked
};

CursorState cursorState = Uninitialized;

void updateCursor() {
    if (!IsCursorHidden() && cursorState == Locked) {
        DisableCursor();
    }

    if (cursorState == Uninitialized) {
        DisableCursor();
        cursorState = Locked;
    }

    if (IsKeyPressed(KEY_F1)) {
        if (cursorState == Locked) {
            EnableCursor();
            cursorState = Unlocked;
        } else {
            DisableCursor();
            cursorState = Locked;
        }
    }
}

struct GraphicsDebugger {
    bool showWireframe = false;
    bool showGrid = true;
    int gridSize = 20;
    int maxFPS = 100;
};

GraphicsDebugger graphicsDebugger;

void GraphicsDebuggerUI() {
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
        if (prevMaxFPS != graphicsDebugger.maxFPS) {
            SetTargetFPS(graphicsDebugger.maxFPS);
        }

    }
    ImGui::End();
}

struct Body {
    std::string name;
    float orbitHeight;
    float angle;
    float radius;
    float mass;
    Color color;
    struct {
        std::vector<Vector3> data;
        size_t maxCount = 100;
    } trail;

    void appendTrail(Vector3 point) {
        if (trail.data.size() >= trail.maxCount) {
            trail.data.erase(trail.data.begin());
        }
        trail.data.push_back(point);
    }

    void updateTrailSize(size_t size) {
        trail.maxCount = size;
        trail.data.clear();
        trail.data.reserve(size);
    }

    size_t trailCount() {
        return trail.data.size();
    }
};


Vector3 GetBodyPosition(Body &body) {
    Vector3 pos = {0, 0, 0};
    if (body.orbitHeight > 0) {
        pos.x = body.orbitHeight * cos(body.angle);
        pos.z = body.orbitHeight * sin(body.angle);
    }
    return pos;
}

void DrawBody(Body &body) {
    Vector3 pos = GetBodyPosition(body);

    if (graphicsDebugger.showWireframe)
        DrawSphereWires(pos, body.radius, 10, 10, body.color);
    else
        DrawSphereEx(pos, body.radius, 32, 32, body.color);

    for (size_t i = 1; i < body.trailCount(); i++) {
        DrawLine3D(body.trail.data[i - 1], body.trail.data[i], body.color);
    }

}

void BodyDebuggerUI(Body &body) {

    std::string windowName = "Body Debugger: " + body.name;

    ImGui::Begin(windowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::Text("Orbit Height: %f", body.orbitHeight);
        ImGui::Text("Angle: %f°", body.angle);
        ImGui::Text("Radius: %f", body.radius);
        ImGui::Text("Mass: %f", body.mass);

        ImGui::BeginChild("Trail", {300, 300});

        for (size_t i = 0; i < body.trailCount(); i++) {
            Vector3 &point = body.trail.data[i];
            ImGui::Text("%zu: x: %f, y: %f, z: %f", i, point.x, point.y, point.z);
        }

        ImGui::EndChild();
    }
    ImGui::End();
}

std::vector<Body> bodies;

bool testRayHit(Vector3 cameraPos, Vector3 cameraDir, Vector3 spherePos, float sphereRadius) {
    // 1. Vector from camera position to sphere center
    Vector3 oc = {spherePos.x - cameraPos.x, spherePos.y - cameraPos.y, spherePos.z - cameraPos.z};

    // 2. Calculate projections
    float ocSq = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z;
    float a = cameraDir.x * cameraDir.x + cameraDir.y * cameraDir.y + cameraDir.z * cameraDir.z;
    float b = 2.0f * (oc.x * cameraDir.x + oc.y * cameraDir.y + oc.z * cameraDir.z);

    // 3. Discriminant
    float discriminant = b * b - 4.0f * a * (ocSq - sphereRadius * sphereRadius);

    // 4. Check for solutions (intersections)
    if (discriminant < 0.0f) {
        // No intersection
        return false;
    } else {
        // One or two intersections (we just need to know if there is AT LEAST one)
        return true;
    }
}

float distanceToSphere(Vector3 cameraPos, Vector3 cameraDir, Vector3 spherePos, float sphereRadius) {
    // 1. Vector from camera position to sphere center
    Vector3 oc = {spherePos.x - cameraPos.x, spherePos.y - cameraPos.y, spherePos.z - cameraPos.z};

    // 2. Calculate projections and distance to sphere center
    float ocSq = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z;
    float a = cameraDir.x * cameraDir.x + cameraDir.y * cameraDir.y + cameraDir.z * cameraDir.z;
    float b = 2.0f * (oc.x * cameraDir.x + oc.y * cameraDir.y + oc.z * cameraDir.z);

    // 3. Discriminant (same as in testRayHit)
    float discriminant = b * b - 4.0f * a * (ocSq - sphereRadius * sphereRadius);

    // 4. Check for solutions (intersections)
    if (discriminant < 0.0f) {
        // No intersection, return distance to sphere center minus radius
        return std::sqrt(ocSq) - sphereRadius;
    } else {
        // Intersection exists. Calculate distance to closest intersection
        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

        // Ensure t is positive (intersection in front of camera)
        float t = (t1 > 0.0f) ? t1 : t2;
        if (t < 0.0f) {
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

struct bodySettings {
    std::string name;
    float orbitHeight;
    float radius;
    float mass;
    ImVec4 color = {1, 1, 1, 1};
};

bodySettings tempBody;

void NewBodyDebuggerUI() {

    ImGui::SetNextWindowSize({300, 180});
    ImGui::Begin("New Body");

    ImGui::InputText("Name", &tempBody.name);
    ImGui::InputFloat("Orbit Height", &tempBody.orbitHeight);
    ImGui::InputFloat("Radius", &tempBody.radius);
    ImGui::InputFloat("Mass", &tempBody.mass);
    ImGui::ColorEdit4("Color", &tempBody.color.x);

    if (ImGui::Button("Create")) {
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

struct CameraSettings {
    Camera camera = {
            .position = {0.0f, 10.0f, 10.0f},
            .target = {0.0f, 0.0f, 0.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE,
    };
    int cameraMode = CAMERA_ORBITAL;
};

CameraSettings cameraSettings;

void CameraSettingsDebuggerUI() {
    ImGui::Begin("Camera Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::Text("Camera Mode");
        if (ImGui::RadioButton("Orbital", &cameraSettings.cameraMode, CAMERA_ORBITAL)) {
            cameraSettings.cameraMode = CAMERA_ORBITAL;
            cameraSettings.camera.target = {0.0f, 0.0f, 0.0f};
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
        ImGui::SliderFloat("Fovy", &cameraSettings.camera.fovy, 30.0f, 90.0f);

        ImGui::End();

    }
    ImGui::End();
}

struct SimulationSettings {
    float timeScale = 1.0f;
    float timeStep = 1.0 / 60.0f;
    bool paused = false;
};

SimulationSettings simulationSettings;

void SimulationSettingsDebuggerUI() {
    ImGui::Begin("Simulation Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::SliderFloat("Time Scale", &simulationSettings.timeScale, 0.0f, 10.0f);
        ImGui::SliderFloat("Time Step", &simulationSettings.timeStep, 0.001f, 1.0f);
        ImGui::Checkbox("Paused", &simulationSettings.paused);
    }
    ImGui::End();
}

bool simRunning = true;

void simulate()
{
    while (simRunning)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        if (!simulationSettings.paused) {
            for (Body &body: bodies) {
                body.angle += 0.5f * simulationSettings.timeScale * simulationSettings.timeStep;

                Vector3 pos = GetBodyPosition(body);
                body.appendTrail(pos);
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();

        auto sim_duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
        TraceLog(LOG_INFO, "Simulation time: %d ns", sim_duration_ns);

        auto timestep_ns = static_cast<long long>(simulationSettings.timeStep * 1000000000.0f / simulationSettings.timeScale);

        if (sim_duration_ns < timestep_ns)
        {
            auto delta_timestep = timestep_ns - sim_duration_ns;
            auto sleepDur = std::chrono::nanoseconds(delta_timestep);
            if(USE_SPINLOCK_TIMER)
                while (std::chrono::high_resolution_clock::now() - startTime < sleepDur);
            else
                std::this_thread::sleep_for(sleepDur);
        }

        auto endTime2 = std::chrono::high_resolution_clock::now();
        auto duration_ns2 = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime).count();
        TraceLog(LOG_INFO, "Total time: %d ns", duration_ns2);

        auto error = duration_ns2 - timestep_ns;
        TraceLog(LOG_INFO, "Δt: %d ns", error);
    }
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT  | FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, WINDOW_TITLE);
    SetTargetFPS(60);



    rlImGuiSetup(true);

    SetExitKey(0);

    SetWindowFocused();

    Body sun = {"Sun", 0.0f, 0.0f, 2.0f, 10.0f, YELLOW};
    bodies.push_back(sun);

    Body earth = {"Earth", 7.0f, 0.0f, 0.5f, 0.1f, BLUE};
    bodies.push_back(earth);

    std::thread simThread;

    simThread = std::thread(simulate);

    while (!WindowShouldClose()) {
        BeginDrawing();
        rlImGuiBegin();
        ClearBackground(BLACK);

        const char *text = "Planet Sim";
        const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);

        DrawText(text,
                 GetScreenWidth() / 2 - text_size.x / 2,
                 30,
                 20,
                 RAYWHITE
        );

        BeginMode3D(cameraSettings.camera);

        if (!simulationSettings.paused)
            UpdateCamera(&cameraSettings.camera, cameraSettings.cameraMode);


        if (graphicsDebugger.showGrid)
            DrawGrid(graphicsDebugger.gridSize, 1.0f);

        for (Body &body: bodies) {
            DrawBody(body);
            BodyDebuggerUI(body);
        }


        if (IsMouseButtonPressed(0)) {
            auto pos = GetMouseRay(GetMousePosition(), cameraSettings.camera);
            TraceLog(LOG_INFO, "Mouse ray POS: %f, %f, %f", pos.position.x, pos.position.y, pos.position.z);
            TraceLog(LOG_INFO, "Mouse ray DIR: %f, %f, %f", pos.direction.x, pos.direction.y, pos.direction.z);

            std::vector<std::tuple<Body, float>> hitBodies;
            for (Body &body: bodies) {
                if (testRayHit(pos.position, pos.direction, GetBodyPosition(body), body.radius)) {
                    float dist = distanceToSphere(pos.position, pos.direction, GetBodyPosition(body), body.radius);
                    hitBodies.emplace_back(body, dist);
                }
            }

            for (auto &[body, dist]: hitBodies) {
                TraceLog(LOG_INFO, "Hit body: %s, distance: %f", body.name.c_str(), dist);
            }

        }

        EndMode3D();
        GraphicsDebuggerUI();
        NewBodyDebuggerUI();
        CameraSettingsDebuggerUI();
        SimulationSettingsDebuggerUI();

        rlImGuiEnd();
        EndDrawing();
    }

    simRunning = false;
    simThread.join();

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}


