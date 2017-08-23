
#include "globals.h"

using namespace std;




void Simulation::calculate_accelaration(Boid b, glm::vec3 * accel) {
	*accel = glm::vec3(0,0,0);

	glm::vec3 flocking_avoid = glm::vec3(0, 0, 0);
	glm::vec3 flocking_match = glm::vec3(0, 0, 0);
	glm::vec3 flocking_center = glm::vec3(0, 0, 0);

	glm::vec3 flocking_hunger = glm::vec3(0, 0, 0);

	// avoid walls
	if (b.pos.x > 280) {
		accel->x += (300.0 / 20.0) * (300 - b.pos.x) - 300;
	}
	else if (b.pos.x < -280) {
		accel->x += (-300.0 / 20.0) * (b.pos.x + 300) + 300;
	}

	if (b.pos.y > 280) {
		accel->y += (300.0 / 20.0) * (300 - b.pos.y) - 300;
	}
	else if (b.pos.y < -280) {
		accel->y += (-300.0 / 20.0) * (b.pos.y + 300) + 300;
	}

	if (b.pos.z > 280) {
		accel->z += (300.0 / 20.0) * (300 - b.pos.z) - 300;
	}
	else if (b.pos.z < -280) {
		accel->z += (-300.0 / 20.0) * (b.pos.z + 300) + 300;
	}

	int closest_mate = -1;
	float closest_mate_distance = 2000.0f;

	// flocking rules
	for (size_t i = 0; i < sim.scene.boids.size(); i++) {
		if (i != b.id && sim.scene.boids[i].alive) {
			// distance between boids
			glm::vec3 x_ij = sim.scene.boids[i].pos - b.pos;
			float distance = glm::length(x_ij);
			if (distance != 0) {
				glm::vec3 v_ij = x_ij / distance;

				//check if possible mate
				if (!sim.scene.boids[i].hungry() && distance < closest_mate_distance) {
					closest_mate = i;
					closest_mate_distance = distance;
				}

				//reduce effect for far off boids
				float distance_limiter = 1;
				if (distance > 50) {
					distance_limiter = (100.0f - distance) / (100.0f - 50.0f);
				}
				if (distance > 100) {
					distance_limiter = 0;
				}

				//avoidance
				flocking_avoid += (v_ij  * (-avoidance / distance)) * distance_limiter;

				//velocity matching
				flocking_match += ((sim.scene.boids[i].vel - b.vel) * matching) * distance_limiter;

				//centering
				flocking_center += (x_ij * centering) * distance_limiter;
			}
		}
	}

	if (closest_mate >= 0 && sim.scene.dead_boids.size() > 0) {
		if (rand() / (RAND_MAX / 100.0) > 95.0) {
			glm::vec3 new_pos = (b.pos + sim.scene.boids[closest_mate].pos) / 2.0f;
			glm::vec3 new_color = (b.color + sim.scene.boids[closest_mate].color) / 2.0f;

			int x = 10;
			sim.scene.create_boid(sim.scene.dead_boids.front(), &new_pos, &new_color);
			sim.scene.dead_boids.pop();
		}
	}

	if (b.hungry()) {
		// find nearest waypoint
		int nearest_id = -1;
		float nearest_distance = 1000.0;
		for (size_t i = 0; i < sim.scene.waypoints.size(); i++) {
			glm::vec3 x_ij = sim.scene.waypoints[i].pos - b.pos;
			float distance = glm::length(x_ij);
			if (distance < nearest_distance) {
				nearest_id = i;
				nearest_distance = distance;
			}
		}
		// accelerate towards nearest waypoint
		if (nearest_id != -1) {
			glm::vec3 x_ij = sim.scene.waypoints[nearest_id].pos - b.pos;
			glm::vec3 v_ij = glm::normalize(x_ij) * 100.0f;

			flocking_hunger = (v_ij - b.vel) * waypoint_accel;
		}
	}

	float accel_max = 200.0f;

	float accel_residual = accel_max - glm::length(*accel);
	if (glm::length(flocking_avoid) != 0) {
		*accel += glm::normalize(flocking_avoid) * min(accel_residual, glm::length(flocking_avoid));
	}

	if (glm::length(flocking_hunger) != 0) {
		accel_residual = accel_max - glm::length(*accel);
		*accel += glm::normalize(flocking_hunger) * min(accel_residual, glm::length(flocking_hunger));
	}

	if (glm::length(flocking_match) != 0) {
		accel_residual = accel_max - glm::length(*accel);
		*accel += glm::normalize(flocking_match) * min(accel_residual, glm::length(flocking_match));
	}

	if (glm::length(flocking_center) != 0) {
		accel_residual = accel_max - glm::length(*accel);
		*accel += glm::normalize(flocking_center) * min(accel_residual, glm::length(flocking_center));
	}
	

	accel_residual = accel_max - glm::length(*accel);
	if (glm::length(b.vel) != 0) {
		*accel += glm::normalize(b.vel) * accel_residual;
	}


	//  air resistance
	*accel += b.vel * -air_resistance;
}

void Simulation::calculate_accelaration(Waypoint w, glm::vec3 * accel) {
	*accel += w.vel * -air_resistance + gravity;
}

Boid Simulation::euler_integration(Boid b, glm::vec3* accel, float ts) {
	Boid update = b;

	update.vel += *accel * ts;

	update.pos += update.vel * ts;

	return update;
}

Waypoint Simulation::euler_integration(Waypoint b, glm::vec3* accel, float ts) {
	Waypoint update = b;

	update.vel += *accel * ts;

	update.pos += update.vel * ts;

	return update;
}

void Simulation::check_waypoints(Boid * b) {
	for (size_t i = 0; i < sim.scene.waypoints.size(); i++) {
		if (sim.scene.waypoints[i].placed) {
			glm::vec3 x_ij = sim.scene.waypoints[i].pos - b->pos;
			float distance = glm::length(x_ij);
			if (distance < 5) {
				sim.scene.place_waypoint(i);

				b->get_food();
			}
		}
	}
}


DWORD WINAPI simulation_loop_thread(LPVOID lpParam) {

	sim.scene.create_boids();

	int time_step_count = 0;
	for (float h = 0; h < sim.time_max; h += sim.time_step) {
		float this_timestep = sim.time_step;
		clock_t start = clock();

		
		for (size_t i = 0; i < sim.scene.boids.size(); i++) {
			if (sim.scene.boids[i].alive) {
				// calculate accelatrion
				glm::vec3 calculated_accel;
				sim.calculate_accelaration(sim.scene.boids[i], &calculated_accel);

				// apply euler integration
				Boid next_state = sim.euler_integration(sim.scene.boids[i], &calculated_accel, this_timestep);

				// update
				sim.scene.boids[i] = next_state;

				// check for waypoint gathered
				sim.check_waypoints(&sim.scene.boids[i]);
			}
		}

		for (size_t i = 0; i < sim.scene.waypoints.size(); i++) {
			if (sim.scene.waypoints[i].placed) {
				// calculate accelatrion
				glm::vec3 calculated_accel;
				sim.calculate_accelaration(sim.scene.waypoints[i], &calculated_accel);

				// apply euler integration
				Waypoint next_state = sim.euler_integration(sim.scene.waypoints[i], &calculated_accel, this_timestep);

				// update
				sim.scene.waypoints[i] = next_state;

				if (sim.scene.waypoints[i].pos.y < -290.0) {
					sim.scene.waypoints[i].placed = false;
				}
			}
			else {
				if (rand() / (RAND_MAX / 100.0) > 99.0) {
					sim.scene.place_waypoint(i);
				}
			}
		}

		for (size_t i = 0; i < sim.scene.boids.size(); i++) {
			if (!sim.scene.boids[i].get_hungry()) {
				sim.scene.dead_boids.push(i);
			}
		}
		

		// wait rest of time step minus calcuations
		float duration = (clock() - start) / (float)CLOCKS_PER_SEC;
		if (duration < this_timestep) {
			Sleep(1000.0 * (this_timestep - duration));
		}

		time_step_count++;
	}

	return 0;
}


void simulation_stop(void) {
	// terminate threads (kind of works?)
	DWORD g_exit_code = 0;
	TerminateThread(glut_thread, g_exit_code);
	DWORD s_exit_code = 0;
	TerminateThread(simulation_thread, s_exit_code);
}
