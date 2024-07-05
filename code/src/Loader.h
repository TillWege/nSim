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

#define GM_FACTOR 1e9;

#define MASS_SUN 1.989e30f

#define GRAV_CONST 6.67430e-11f

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
		40.0f,
        MASS_SUN,
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
		body.displayRadius = 30.0f;

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

void loadSatellites()
{
	auto satelliteData = loadCSV(SATELLITE_ASSETS_PATH);
	TraceLog(LOG_DEBUG, "Loaded %d satellites", satelliteData.size());
}

#endif //LOADER_H
