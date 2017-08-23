#pragma once

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#include <ctime>
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "glm/vec3.hpp" 
#include "glm/vec4.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 
#include "glm/gtx/vector_angle.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "GL\glut.h"

#include "scene.h"
#include "simulation.h"
#include "glut_thread.h"


// Simulation global
extern Simulation sim;

// threading
extern HANDLE simulation_thread;
extern DWORD simulation_thread_id;

extern HANDLE glut_thread;
extern DWORD glut_thread_id;


extern std::vector<glm::vec3 > fish_vertices;
extern std::vector<vector<size_t> > fish_faces;

