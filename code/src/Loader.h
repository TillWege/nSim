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

#define SATELLITE_ASSETS_PATH ASSETS_PATH"/satellites.csv"

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

	Body sun = {
		"Sun",
		696340000.0f,
		20.0f,
		1.989e30f,
		YELLOW,
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};

	bodies.push_back(sun);

	for (auto& row : planetData)
	{
		Body body;
		body.name = row[PLANET_NAME_INDEX];
		body.mass = std::stof(row[PLANET_MASS_INDEX]) * MASS_FACTOR;
		body.radius = (std::stof(row[PLANET_DIAMETER_INDEX]) / 2.0f) * 1000.f;
		body.position = { 0, 0,0 };
		body.displayRadius = 10.0f;

		float dist = std::stof(row[PLANET_DISTANCE_INDEX]) * DISTANCE_FACTOR;

		int phaseAng = GetRandomValue(0, 360);

		float phaseRad = float(phaseAng) * (PI / 180);

		body.position.z = dist * float(cos(phaseRad));
		body.position.x = dist * float(sin(phaseRad));
		body.position.y = 0;

		float speed = std::stof(row[PLANET_SPEED_INDEX]) * SPEED_FACTOR;

		int rotAng = phaseAng + 90;
		float rotRad = float(rotAng) * (PI / 180);

		body.velocity.z = speed * float(cos(rotRad));
		body.velocity.x = speed * float(sin(rotRad));
		body.velocity.y = 0;


		if(body.name == "Mercury")
			body.color = GRAY;
		else if (body.name == "Venus")
			body.color = YELLOW;
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

		if (body.name=="Venus")
			return;
	}
}

void loadSatellites()
{
	auto satelliteData = loadCSV(SATELLITE_ASSETS_PATH);
	TraceLog(LOG_DEBUG, "Loaded %d satellites", satelliteData.size());
}

#endif //LOADER_H
