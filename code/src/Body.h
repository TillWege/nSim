//
// Created by tillw on 04/07/2024.
//

#ifndef BODY_H
#define BODY_H

#include <string>
#include <vector>
#include "raylib.h"
#include "SciVec3.h"
#include "Consts.h"

typedef struct Body Body;

struct Body
{
	std::string name;
    double radius;
	double mass;
	Color color{};
	SciVec3 position;
    SciVec3 velocity;
	bool isPlanet;

	int satelliteCount = 0;
	Body* satellites[128]{};

	std::vector<Vector3> trail;
	size_t trailLength = 0;

	explicit Body (bool isPlanet)
	{
		this->name = "";
		this->radius = 0;
		this->mass = 0;
		this->color = BLANK;
		this->position = {0, 0, 0};
		this->velocity = {0, 0, 0};
		this->satelliteCount = 0;
		this->isPlanet = isPlanet;


		if (isPlanet)
			this->trailLength = PLANET_TRAIL_LENGTH * PLANET_TRAIL_RESOLUTION;
		else
			this->trailLength = SATELLITE_TRAIL_LENGTH * SATELLITE_TRAIL_RESOLUTION;

		trail.reserve(trailLength);
	}

	void appendTrail(Vector3 point)
	{
		if (trail.size() >= this->trailLength)
			trail.erase(trail.begin());

		trail.push_back(point);
	}


	[[nodiscard]] size_t trailCount() const
	{
		return trail.size();
	}

	[[nodiscard]] Vector3 getDisplayPosition() const
	{
		return Vector3 {
            float(double(position.x / UNIT_SIZE)),
            float(double(position.y / UNIT_SIZE)),
            float(double(position.z / UNIT_SIZE))
        };
	}

	[[nodiscard]] size_t getEffectiveRadius() const
	{
		return size_t(radius / UNIT_SIZE);
	}
};

#endif //BODY_H
