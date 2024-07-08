//
// Created by tillw on 08/07/2024.
//

#ifndef CONSTS_H
#define CONSTS_H

#define WINDOW_TITLE "nSim"

// PHYSICS CONSTANTS

#define UNIT_SIZE 1000000.0f // 1 unit = 1,000,000 m
#define GRAV_CONST 6.67430e-11f
#define GM_FACTOR 1e9

#define MASS_SUN 1.989e30f
#define RADIUS_SUN 6.9634e8f

#define SECONDS_PER_DAY 86400

// SIMULATION CONSTANTS

#define TIME_STEP 1 // Seconds per sim tick

// GRAPHICS CONSTANTS

// PLANET TRAILS

#define PLANET_TRAIL_RESOLUTION 50 // Points per day
#define PLANET_TRAIL_STEP (SECONDS_PER_DAY / PLANET_TRAIL_RESOLUTION) // Seconds per point
#define PLANET_TRAIL_LENGTH 365 // Days

// SATELLITE TRAILS

#define SATELLITE_TRAIL_RESOLUTION 2000 // Points per day
#define SATELLITE_TRAIL_STEP (SECONDS_PER_DAY / SATELLITE_TRAIL_RESOLUTION) // Seconds per point
#define SATELLITE_TRAIL_LENGTH 1

// MOON TRAILS

#endif //CONSTS_H
