
#include "globals.h"

using namespace std;

// Simulation global
Simulation sim;

// threading
HANDLE simulation_thread;
DWORD simulation_thread_id;
HANDLE glut_thread;
DWORD glut_thread_id;


int main(int argc, char** argv)
{
	int num_boids = 500;
	int num_way = 10;
	if (argc >= 2) {
		num_boids = atoi(argv[1]);
	}
	if (argc >= 3) {
		num_way = atoi(argv[2]);
	}


	srand(clock());

	load_fish();

	// settup box faces
	vector<Polygon_3d> faces;

	glm::vec3 box1_color{ 21 / 255.0f, 45 / 255.0f, 115 / 255.0f };
	vector<glm::vec3> box1_points{ glm::vec3(-300.0f, -300.0f, -300.0f), glm::vec3(-300.0f, 300.0f, -300.0f), glm::vec3(-300.0f, 300.0f, 300.0f), glm::vec3(-300.0f, -300.0f, 300.0f) };
	faces.push_back(Polygon_3d(box1_points, box1_color));

	glm::vec3 box2_color{ 21 / 255.0f, 45 / 255.0f, 115 / 255.0f };
	vector<glm::vec3> box2_points{ glm::vec3(300.0f, -300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, -300.0f), glm::vec3(300.0f, -300.0f, -300.0f) };
	faces.push_back(Polygon_3d(box2_points, box2_color));

	glm::vec3 box3_color{ 21 / 255.0f, 45 / 255.0f, 115 / 255.0f };
	vector<glm::vec3> box3_points{ glm::vec3(300.0f, -300.0f, -300.0f), glm::vec3(300.0f, 300.0f, -300.0f), glm::vec3(-300.0f, 300.0f, -300.0f), glm::vec3(-300.0f, -300.0f, -300.0f) };
	faces.push_back(Polygon_3d(box3_points, box3_color));

	glm::vec3 box4_color{ 21 / 255.0f, 45 / 255.0f, 115 / 255.0f };
	vector<glm::vec3> box4_points{ glm::vec3(-300.0f, -300.0f, 300.0f), glm::vec3(-300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, -300.0f, 300.0f) };
	faces.push_back(Polygon_3d(box4_points, box4_color));

	glm::vec3 box5_color{ 21 / 255.0f, 45 / 255.0f, 115 / 255.0f };
	vector<glm::vec3> box5_points{ glm::vec3(-300.0f, 300.0f, -300.0f), glm::vec3(300.0f, 300.0f, -300.0f), glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(-300.0f, 300.0f, 300.0f) };
	faces.push_back(Polygon_3d(box5_points, box5_color));

	glm::vec3 box6_color{ 79 / 255.0f, 46 / 255.0f, 46 / 255.0f };
	vector<glm::vec3> box6_points{ glm::vec3(-300.0f, -300.0f, 300.0f), glm::vec3(300.0f, -300.0f, 300.0f), glm::vec3(300.0f, -300.0f, -300.0f), glm::vec3(-300.0f, -300.0f, -300.0f) };
	faces.push_back(Polygon_3d(box6_points, box6_color));

	// inisitalize scene and simulation
	// 20 boids, 3 waypoints, with faces as box
	// scene, gravity, air resistance, elasitcity, friction, time step, time max
	Scene scene(num_boids, num_way, faces);

	glm::vec3 gravity(0.0f, -20.0f, 0.0f);
	float air_resistance = 0.9f;
	float elasiticty = 0.9f;
	float fiction = 0.1f;
	float time_step = 0.01f;
	float time_max = 1000.0f;

	// boid settings
	float avoidance = 100.0f;
	float matching = 0.5f;
	float centering = 0.1f;

	// waypoint setting
	float waypoint_accel = 0.1f;

	sim = Simulation(scene, gravity, air_resistance, elasiticty, fiction, time_step, time_max, avoidance, matching, centering, waypoint_accel);


	// start simulation thread
	sim_args s_arg;
	simulation_thread = CreateThread(
		NULL,				//security attributes
		0,					//stack size
		simulation_loop_thread,	//thread function
		&s_arg,				//argument to thread
		0,					//creation flags
		&simulation_thread_id);		//returns thread id


	// start display thread
	glut_args g_arg;
	g_arg.argc = argc;
	g_arg.argv = argv;
	glut_thread = CreateThread(
		NULL,				//security attributes
		0,					//stack size
		glut_main,	//thread function
		&g_arg,				//argument to thread
		0,					//creation flags
		&glut_thread_id);		//returns thread id

	atexit(simulation_stop);

	HANDLE handles[2];
	handles[0] = simulation_thread;
	handles[1] = glut_thread;

	// wait on threads to stop
	WaitForMultipleObjects(2, handles, true, INFINITE);
	

	return 0;
}