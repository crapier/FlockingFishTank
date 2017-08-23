#pragma once


#include "globals.h"

using namespace std;


void load_fish();

struct Boid {
	glm::vec3 pos;
	glm::vec3 vel;

	glm::vec3 color;
	int hunger;

	bool alive;

	int id;

	Boid() {
		color.r = (0.0f + rand()) / RAND_MAX;
		color.g = (0.0f + rand()) / RAND_MAX;
		color.b = (0.0f + rand()) / RAND_MAX;

		id = -1;

		hunger = 3;

	}

	Boid(glm::vec3 p, glm::vec3 v) {
		pos = p;
		vel = v;

		alive = false;


		color.r = (0.0f + rand()) / RAND_MAX;
		color.g = (0.0f + rand()) / RAND_MAX;
		color.b = (0.0f + rand()) / RAND_MAX;

		hunger = 3;

		id = -1;

	}

	void draw();

	bool hungry() {
		if (hunger < 2) {
			return true;
		}
		else {
			return false;
		}
	}

	bool get_hungry() {
		if (rand() / (RAND_MAX / 200.0) > 199.0) {
			hunger--;

			if (hunger < 0) {
				if (rand() / (RAND_MAX / 100.0) > 90.0) {
					alive = false;
				}
				else {
					hunger = 0;
				}
			}
		}

		return alive;
	}

	void get_food() {
		hunger++;
		if (hunger > 3) {
			hunger = 3;
		}
	}
};

struct Polygon_3d {
	vector<glm::vec3> points;
	glm::vec3 normal;
	glm::vec3 average;

	glm::vec3 color;

	bool one_sided;

	Polygon_3d(){
		points.push_back(glm::vec3(0.0, 0.0, 0.0));
		points.push_back(glm::vec3(0.0, 0.0, 0.0));
		points.push_back(glm::vec3(0.0, 0.0, 0.0));
		points.push_back(glm::vec3(0.0, 0.0, 0.0));

		color = { 0.0, 0.0, 0.0 };

		normal = glm::vec3(0.0, 0.0, 0.0);
		average = glm::vec3(0.0, 0.0, 0.0);

		one_sided = true;
	}

	Polygon_3d(vector<glm::vec3> p, glm::vec3 c) {
		points = p;

		// calculate normal (inward facing)
		glm::vec3 u = points[2] - points[1];
		glm::vec3 v = points[0] - points[1];
		normal = glm::cross(u, v);
		normal = glm::normalize(normal);

		// calc avg
		average = glm::vec3(0.0, 0.0, 0.0);
		for (size_t i = 0; i < points.size(); i++) {
			average.x += points[i].x;
			average.y += points[i].y;
			average.z += points[i].z;
		}
		average.x = average.x / points.size();
		average.y = average.y / points.size();
		average.z = average.z / points.size();

		color = c;

		one_sided = true;

	}

	void draw() {
		glColor3f(color[0], color[1], color[2]);

		if (points.size() == 3) {
			glBegin(GL_TRIANGLES);
		}
		else if (points.size() == 4) {
			glBegin(GL_QUADS);
		}
		else {
			glBegin(GL_POLYGON);
		}
		
		for (size_t i = 0; i < points.size(); i++) {
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}

		if (!one_sided) {
			for (int j = points.size() - 1; j >= 0; j--) {
				glVertex3f(points[j].x, points[j].y, points[j].z);
			}
		}
		glEnd();
	}

	float distance(glm::vec3 p) {
		// vector from point on plane to p
		glm::vec3 vec = p - points[0];

		// project vector onto normal
		return glm::dot(vec, normal);
	}

	void invert_normal() {
		normal = glm::vec3(-1 * normal.x, -1 * normal.y, -1 * normal.z);
	}

	void set_one_sided(bool s) {
		one_sided = s;
	}
};

struct Waypoint {
	glm::vec3 pos;
	glm::vec3 vel;
	bool placed;
	int id;

	Waypoint() {
		placed = false;
	}

	void draw() {
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
			glTranslatef(pos.x, pos.y, pos.z);
			glutSolidSphere(2, 10, 10);
		glPopMatrix();
	}
};

struct Scene {
	vector<Boid> boids;
	vector<Polygon_3d> faces;
	
	queue<int> dead_boids;

	vector<Waypoint> waypoints;

	Scene() {

	}

	Scene(int num_boids, int num_waypoints, vector<Polygon_3d> p) {
		boids = vector<Boid>(num_boids);
		waypoints = vector<Waypoint>(num_waypoints);
		faces = p;
	}

	void create_boid(size_t i, glm::vec3 *pos = NULL, glm::vec3 *color = NULL) {
		float x, y, z, vx, vy, vz;

		if (pos == NULL) {
			x = (rand() / (RAND_MAX / 560.0)) - 280.0;
			y = (rand() / (RAND_MAX / 560.0)) - 280.0;
			z = (rand() / (RAND_MAX / 560.0)) - 280.0;

			boids[i].pos = glm::vec3(x, y, z);
		}
		else {
			boids[i].pos = *pos;
		}

		if (color != NULL) {
			boids[i].color = *color;
		}

		vx = (rand() / (RAND_MAX / 300.0)) - 150.0;
		vy = (rand() / (RAND_MAX / 300.0)) - 150.0;
		vz = (rand() / (RAND_MAX / 300.0)) - 150.0;

		boids[i].vel = glm::vec3(vx, vy, vz);

		boids[i].alive = true;
		boids[i].id = i;
		boids[i].hunger = 3;
	}

	void create_boids(){
		for (size_t i = 0; i < boids.size(); i++) {
			create_boid(i);
		}
	}

	void place_waypoint(int id) {
		float x, y, z;
		x = (rand() / (RAND_MAX / 560.0)) - 280.0;
		y = 280.0;
		z = (rand() / (RAND_MAX / 560.0)) - 280.0;

		waypoints[id].pos = glm::vec3(x, y, z);
		waypoints[id].vel = glm::vec3(0, 0, 0);

		waypoints[id].placed = true;
	}
};