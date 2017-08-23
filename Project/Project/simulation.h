#pragma once

#include "globals.h"


struct Simulation {
	Scene scene;
	glm::vec3 gravity;
	float air_resistance;
	float elasticity;
	float friction;
	bool coulomb_friction;
	float time_step;
	float time_max;

	// boid settings
	float avoidance;
	float matching;
	float centering;
	
	// waypoint setting
	float waypoint_accel;

	Simulation() {
		gravity = glm::vec3(0.0, -3.0, 0.0);
		air_resistance = 0.1;
		elasticity = 0.9;
		friction = 0.01;
		time_step = 0.03;
		time_max = 120.0;

		coulomb_friction = false;

		avoidance = 0.5;
		matching = 0.5;
		centering = 0.5;
		
		waypoint_accel = 100.0;
	}

	Simulation(Scene s, glm::vec3 g, float ar, float e, float f, float ts, float tm, float av, float mt, float ct, float wp) {
		scene = s;
		gravity = g;
		air_resistance = ar;
		elasticity = e;
		friction = f;
		time_step = ts;
		time_max = tm;

		coulomb_friction = false;

		avoidance = av;
		matching = mt;
		centering = ct;
		waypoint_accel = wp;
	}

	void calculate_accelaration(Boid b, glm::vec3* accel);

	void calculate_accelaration(Waypoint w, glm::vec3* accel);

	Boid euler_integration(Boid b, glm::vec3* accel, float ts);

	Waypoint euler_integration(Waypoint w, glm::vec3* accel, float ts);

	void check_waypoints(Boid * b);
};

struct sim_args {

};

DWORD WINAPI simulation_loop_thread(LPVOID lpParam);

void simulation_stop(void);

