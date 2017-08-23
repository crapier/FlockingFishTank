
#include "globals.h"

// fish model
vector<glm::vec3 > fish_vertices;
vector<vector<size_t> > fish_faces;

void load_fish() {

	fstream input_file;
	input_file.open("fish.obj", fstream::in);

	string line;
	while (getline(input_file, line)) {
		// If line isn't a comment
		if (line.length() > 0 && line[0] != '#') {
			// If line is a vertex
			if (line[0] == 'v') {
				stringstream ss(line);

				// read off prefix char
				char prefix;
				ss >> prefix;

				// XYZ Coordianate
				float coord[3];

				// Get the 3 coordinates for the vertex
				ss >> coord[0] >> coord[1] >> coord[2];

				// Add Vertex
				fish_vertices.push_back(glm::vec3(coord[0], coord[1], coord[2]));
			}
			// If line is a face
			else if (line[0] == 'f') {
				stringstream ss(line);

				// read off prefix char
				char prefix;
				ss >> prefix;

				// face indecies
				int vert[3];

				// Get the 4 indices
				ss >> vert[0] >> vert[1] >> vert[2];

				// Add Face
				vector<size_t> f;
				f.push_back(vert[0] - 1);
				f.push_back(vert[1] - 1);
				f.push_back(vert[2] - 1);
				fish_faces.push_back(f);
			}
		}
	}
}

void Boid::draw() {

	glColor3f(color[0], color[1], color[2]);

	/* Rotation for model code found at  http://stackoverflow.com/questions/6992541/opengl-rotation-in-given-direction */

	glm::vec3 delta = vel;
	glm::vec3 up;
	glm::vec3 direction(glm::normalize(delta));
	if (abs(direction.x)< 0.00001 && abs(direction.z) < 0.00001) {
		if (direction.y > 0)
			up = glm::vec3(0.0, 0.0, -1.0); //if direction points in +y
		else
			up = glm::vec3(0.0, 0.0, 1.0); //if direction points in -y 
	}
	else {
		up = glm::vec3(0.0, 1.0, 0.0); //y-axis is the general up
	}
	up = glm::normalize(up);
	glm::vec3 right = glm::normalize(glm::cross(up, direction));
	up = glm::normalize(glm::cross(direction, right));

	glm::mat4 m = glm::mat4(right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		direction.x, direction.y, direction.z, 0.0f,
		pos.x, pos.y, pos.z, 1.0f);

	glPushMatrix();		
		glMultMatrixf(glm::value_ptr(m));

		glBegin(GL_TRIANGLES);
		for (size_t i = 0; i < fish_faces.size(); i++) {

			for (size_t j = 0; j < fish_faces[i].size(); j++) {
				glm::vec3 v = fish_vertices[fish_faces[i][j]];
				glVertex3d(v.x, v.y, v.z);
			}
		}
		glEnd();
	glPopMatrix();
}