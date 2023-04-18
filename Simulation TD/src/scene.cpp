#include "scene.hpp"


using namespace cgp;


// Spring force applied on particle p_i with respect to position p_j.
vec3 spring_force(vec3 const& p_i, vec3 const& p_j, float L0, float K)
{
	vec3 const p = p_i - p_j;
	float const L = norm(p);
	vec3 const u = p / L;

	vec3 const F = -K * (L - L0) * u;
	return F;
}


void scene_structure::simulation_step(float dt)
{
	// Simulation parameters
	float m = 0.01f;       // particle mass
	float K = 5.0f;        // spring stiffness
	float mu = 0.005f;      // damping coefficient

	vec3 g = { 0,0,-9.81f }; // gravity

	// Forces
	for (int i = 1; i<N-1 ; i++){
		vec3 f_spring = spring_force(positions[i], positions[i-1], L0, K)+spring_force(positions[i], positions[i+1], L0, K);
		vec3 f_weight = m * g;
		vec3 f_damping = -mu * vitesses[i];
		forces[i] = f_spring + f_weight + f_damping;
	}

	vec3 f_spring = spring_force(positions[N-1], positions[N-2], L0, K);
	vec3 f_weight = m * g;
	vec3 f_damping = -mu * vitesses[N-1];
	forces[N-1] = f_spring + f_weight + f_damping;

	// Numerical Integration
	for (int i = 0; i<N; i++){
		vitesses[i] = vitesses[i] + dt * forces[i] / m;
		positions[i] = positions[i] + dt * vitesses[i];
	}

}


void scene_structure::draw_segment(vec3 const& a, vec3 const& b)
{
	segment.vbo_position.update(numarray<vec3>{ a, b });
	draw(segment, environment);
}

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 6.0f, 4.0f, 4.0f }, { 0,0,0 }, { 0,0,1 });
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Initial position and speed of particles
	// ******************************************* //
	for (int i = 0; i<N ; i++){
		positions[i]={ 0.0f, i*0.15f, 0.0f };
		vitesses[i]={ 0,0,0 };
	}
	L0 = 0.1f;

	particle_sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.05f));
	segment.display_type = curve_drawable_display_type::Segments;
	segment.initialize_data_on_gpu({ {0,0,0}, {1,0,0} });

}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();

	simulation_step(timer.scale * 0.005f);

	particle_sphere.model.translation = positions[0];
	particle_sphere.material.color = { 0,0,0 };
	draw(particle_sphere, environment);

	for (int i = 1; i<N ; i++){
		particle_sphere.model.translation = positions[i];
		particle_sphere.material.color = { 1,0,0 };
		draw(particle_sphere, environment);

		draw_segment(positions[i-1], positions[i]);
	}

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}
