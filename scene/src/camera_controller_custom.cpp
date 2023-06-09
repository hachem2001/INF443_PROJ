#include "camera_controller_custom.hpp"

using namespace cgp;


void camera_controller_custom::update(mat4& camera_matrix_view) {
	camera_matrix_view = camera_model.matrix_view();
}

void camera_controller_custom::action_mouse_move(mat4& camera_matrix_view)
{
	// Preconditions
	assert_cgp_no_msg(inputs != nullptr);
	assert_cgp_no_msg(window != nullptr);
	if (!is_active) return;

	vec2 const& p1 = inputs->mouse.position.current;
	vec2 const& p0 = inputs->mouse.position.previous;
	vec2 const dp = p1 - p0;

	bool const event_valid = !inputs->mouse.on_gui;
	bool const click_left = inputs->mouse.click.left;
	bool const click_right = inputs->mouse.click.right;

	if (event_valid) {
		if (!is_cursor_trapped)  {
			if (click_left)
				camera_model.manipulator_rotate_roll_pitch_yaw(0, dp.y, -dp.x);
			else if (click_right)
				camera_model.manipulator_translate_front(-(p1 - p0).y);
		}
		else if (is_cursor_trapped) {
			camera_model.manipulator_rotate_roll_pitch_yaw(0, dp.y, -dp.x);
		}
	}

	update(camera_matrix_view);
}


void camera_controller_custom::action_keyboard(mat4&)
{
	if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_C) && inputs->keyboard.shift) {
		is_cursor_trapped = !is_cursor_trapped;
		if (is_cursor_trapped)
			glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	// Escape also gives back the cursor
	if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE)) {
		is_cursor_trapped = false;
		glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}


void camera_controller_custom::idle_frame(mat4& camera_matrix_view)
{
	// Preconditions
	assert_cgp_no_msg(inputs != nullptr);
	assert_cgp_no_msg(window != nullptr);
	if (!is_active) return;

	float const magnitude = 5 * inputs->time_interval;

	vec3 before_position = camera_model.position();

	// displacement with WSAD
	if (inputs->keyboard.is_pressed(GLFW_KEY_R))
		camera_model.manipulator_translate_in_plane({ 0,-magnitude });
	if (inputs->keyboard.is_pressed(GLFW_KEY_F))
		camera_model.manipulator_translate_in_plane({ 0, magnitude });
	if (inputs->keyboard.is_pressed(GLFW_KEY_A)) {
		camera_model.manipulator_translate_in_plane({ magnitude ,0 });
	}
	if (inputs->keyboard.is_pressed(GLFW_KEY_D)) {
		camera_model.manipulator_translate_in_plane({ -magnitude ,0 });
}
	if (inputs->keyboard.is_pressed(GLFW_KEY_W)) {	
		camera_model.manipulator_translate_front(-magnitude);
	}
	if (inputs->keyboard.is_pressed(GLFW_KEY_S)) {
		camera_model.manipulator_translate_front(magnitude);
	}
	if (inputs->keyboard.ctrl == false) {
		if (inputs->keyboard.up)
			camera_model.manipulator_translate_in_plane({ 0,-magnitude });
		if (inputs->keyboard.down)
			camera_model.manipulator_translate_in_plane({ 0, magnitude });
		if (inputs->keyboard.left)
			camera_model.manipulator_translate_in_plane({ magnitude ,0 });
		if (inputs->keyboard.right)
			camera_model.manipulator_translate_in_plane({ -magnitude ,0 });
	}
	else {
		if (inputs->keyboard.up)
			camera_model.manipulator_translate_front(-magnitude);
		if (inputs->keyboard.down)
			camera_model.manipulator_translate_front(magnitude);
	}

	vec3 after_position = camera_model.position();

	before_pos = before_position;
	after_pos = after_position;

	update(camera_matrix_view);
}

void camera_controller_custom::set_rotation_axis_z()
{
	camera_model.set_rotation_axis({0,0,1});
}

void camera_controller_custom::set_position(vec3 position)
{
	camera_model.position_camera = position;
}
