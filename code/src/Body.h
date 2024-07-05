//
// Created by tillw on 04/07/2024.
//

#ifndef BODY_H
#define BODY_H

#include <string>
#include <vector>
#include "raylib.h"
#include "SciVec3.h"

typedef struct Body Body;

#define UNIT_SIZE 100000000.0f // 1 unit = 10,000,000 m

struct Body
{
	std::string name;
    double radius;
    double displayRadius;
	double mass;
	Color color;
	SciVec3 position;
    SciVec3 velocity;

	Body* satellites[128];

	struct
	{
		std::vector<Vector3> data;
		size_t maxCount = 100 * 24;
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

	Vector3 getDisplayPosition()
	{
		return Vector3 {
            float(double(position.x / UNIT_SIZE)),
            float(double(position.y / UNIT_SIZE)),
            float(double(position.z / UNIT_SIZE))
        };
	}
};

#endif //BODY_H
