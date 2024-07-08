//
// Created by tillw on 04/07/2024.
//

#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "raylib.h"
#include "Consts.h"

#define PLANET_ASSETS_PATH ASSETS_PATH"/planets.csv"

#define PLANET_NAME_INDEX 0
#define PLANET_MASS_INDEX 1
#define PLANET_DIAMETER_INDEX 2
#define PLANET_ROTATION_PERIOD_INDEX 6
#define PLANET_DISTANCE_INDEX 8
#define PLANET_SPEED_INDEX 12

#define MASS_FACTOR 1e24
#define DIAMETER_FACTOR 1e3 // von km in m umrechnen
#define ROTATION_FACTOR 60 * 60
#define DISTANCE_FACTOR 1e9 // von 1e6km in m umrechnen
#define SPEED_FACTOR 1e3 // von km/s in m/s umrechnen

#define SATELLITE_DATA_ASSETS_PATH ASSETS_PATH"/satellites_data.csv"
#define SATELLITE_ORBIT_ASSETS_PATH ASSETS_PATH"/satellites_orbit.csv"

std::vector<std::vector<std::string>> loadCSV(const std::string& filename) {
	std::vector<std::vector<std::string>> data;
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Error: Unable to open file " << filename << std::endl;
		return data; // Return an empty vector on error
	}

	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> row;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ',')) {
			row.push_back(cell);
		}

		data.push_back(row);
	}

	return data;
}

void loadPlanets(std::vector<Body> &bodies)
{
	auto planetData = loadCSV(PLANET_ASSETS_PATH);
	TraceLog(LOG_DEBUG, "Loaded %d planets", planetData.size());

	// Skip the first row
	planetData.erase(planetData.begin());

	Body sun = Body(true);

	sun.name = "Sun";
	sun.mass = MASS_SUN;
	sun.radius = RADIUS_SUN;
	sun.color = YELLOW;

	bodies.push_back(sun);

	for (auto& row : planetData)
	{
		Body body = Body(true);
		body.name = row[PLANET_NAME_INDEX];
		body.mass = std::stof(row[PLANET_MASS_INDEX]) * MASS_FACTOR;
		body.radius = (std::stof(row[PLANET_DIAMETER_INDEX]) / 2.0f) * 1000.f;
		body.position = { 0, 0,0 };

        double dist = std::stod(row[PLANET_DISTANCE_INDEX]) * DISTANCE_FACTOR;

		int phaseAng = GetRandomValue(0, 360);

        double phaseRad = double(phaseAng) * (PI / 180);

		body.position.z = dist * cos(phaseRad);
		body.position.x = dist * sin(phaseRad);
		body.position.y = 0;

		double speed = std::stod(row[PLANET_SPEED_INDEX]) * SPEED_FACTOR;

		int rotAng = phaseAng + 90;
        double rotRad = double(rotAng) * (PI / 180);

		body.velocity.z = speed * cos(rotRad);
		body.velocity.x = speed * sin(rotRad);
		body.velocity.y = 0;


		if(body.name == "Mercury")
			body.color = GRAY;
		else if (body.name == "Venus")
			body.color = ORANGE;
		else if (body.name == "Earth")
			body.color = BLUE;
		else if (body.name == "Mars")
			body.color = MAROON;
		else if (body.name == "Jupiter")
			body.color = BROWN;
		else if (body.name == "Saturn")
			body.color = BEIGE;
		else if (body.name == "Uranus")
			body.color = SKYBLUE;
		else if (body.name == "Neptune")
			body.color = DARKBLUE;
		else if (body.name == "Pluto")
			body.color = DARKGRAY;
		else
			body.color = WHITE;

		bodies.push_back(body);
	}
}

void loadSatellites(std::vector<Body> &bodies)
{
	auto satelliteData = loadCSV(SATELLITE_DATA_ASSETS_PATH);
	auto satelliteOrbitData = loadCSV(SATELLITE_ORBIT_ASSETS_PATH);

	satelliteData.erase(satelliteData.begin());
	satelliteOrbitData.erase(satelliteOrbitData.begin());

	for(auto& row : satelliteData)
	{
		std::string parent = row[0];
		std::string name = row[1];


		auto gmStr = row[3];

		// Kerberos and Styx have a '<' in front of their gm value
		if(gmStr.starts_with('<'))
			gmStr = gmStr.substr(1);

		// Nereid has 0 as gm value
		if(gmStr == "0.00000")
			continue;

		double gm = std::stod(gmStr);

		double radius = std::stod(row[6]);

		std::vector<std::string>* orbit = nullptr;

		for(auto& orbitRow: satelliteOrbitData)
		{
			std::string orbitSat = orbitRow[1];

			if(orbitSat == name)
			{
				orbit = &orbitRow;
				break;
			}
		}

		if(orbit == nullptr)
		{
			TraceLog(LOG_WARNING, "No orbit data found for %s", name.c_str());
			continue;
		}

		Body satellite = Body(false);

		satellite.name = name;
		satellite.mass = (gm * GM_FACTOR) / GRAV_CONST ;
		satellite.radius = radius * 1000.0f;

//		auto parent_body = std::find_if(bodies.begin(), bodies.end(), [&](const Body& body) {
//			return body.name == parent;
//		});

		int parentIndex = -1;

		for(int i = 0; i < bodies.size(); i++)
		{
			if(bodies[i].name == parent)
			{
				parentIndex = i;
				break;
			}
		}

		if(parentIndex == -1)
		{
			TraceLog(LOG_WARNING, "Parent body %s not found for satellite %s", parent.c_str(), name.c_str());
			continue;
		}

		bodies[parentIndex].satelliteNames[bodies[parentIndex].satelliteCount] = name;
		bodies[parentIndex].satelliteCount++;

		SciVec3 parentPos = bodies[parentIndex].position;
		SciVec3 parentVel = bodies[parentIndex].velocity;

		double dist = std::stod(orbit->at(6)) * 1000.0f;

		// vis-viva equation
		double speed = std::sqrt((GRAV_CONST * bodies[parentIndex].mass) / dist); //v ≈ √(GM/a)

		int phaseAng = GetRandomValue(0, 360);
		double phaseRad = double(phaseAng) * (PI / 180);

		SciVec3 relPos = { 0, 0, 0 };
		relPos.z = dist * cos(phaseRad);
		relPos.x = dist * sin(phaseRad);

		int rotAng = phaseAng + 90;
		double rotRad = double(rotAng) * (PI / 180);

		SciVec3 relVel = { 0, 0, 0 };
		relVel.z = speed * cos(rotRad);
		relVel.x = speed * sin(rotRad);

		satellite.position = parentPos + relPos;
		satellite.velocity = parentVel + relVel;

		satellite.color = WHITE;

		bodies.push_back(satellite);
	}


}

#endif //LOADER_H
