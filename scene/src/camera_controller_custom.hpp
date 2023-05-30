#pragma once

#include "cgp/cgp.hpp"

using namespace cgp;

struct camera_controller_custom : camera_controller_generic_base
{
	camera_first_person_euler camera_model;

	vec3 before_pos;
	vec3 after_pos;

	/// @brief Sets rotation axis of camera to z up.
	void set_rotation_axis_z();

	void set_position(vec3 position);

	/// @brief Update camera rotation when the mouse moves.
	/// @param camera_matrix_view 
	void action_mouse_move(mat4& camera_matrix_view);

	// Key 'C' (in capital) to capture the cursor
	void action_keyboard(mat4& camera_matrix_view);

	// Keys up/down/left/right used to translate the camera
	//void idle_frame(camera_orbit_spherical_coord& camera);
	void idle_frame(mat4& camera_matrix_view);

	void update(mat4& camera_matrix_view);

private:
	// true = cursor captured (/infinite motion), false = cursor free
	bool is_cursor_trapped = false;
};