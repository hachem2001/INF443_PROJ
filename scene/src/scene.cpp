#include "scene.hpp"

#include "room.hpp"

using namespace cgp;

mat4 portal_view(mat4 orig_view, Mesh* src, Mesh* dst) {
  mat4 mv = orig_view * src->object2world;
  mat4 portal_cam =
    // 3. transformation from source portal to the camera - it's the
    //    first portal's ModelView matrix:
    mv
    // 2. object is front-facing, the camera is facing the other way:
    * rotate(mat4(1.0), radians(180.0f), vec3(0.0,1.0,0.0))
    // 1. go the destination portal; using inverse, because camera
    //    transformations are reversed compared to object
    //    transformations:
    * inverse(dst->object2world)
    ;
  return portal_cam;
}

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	//camera_control.set_rotation_axis_z();
	//camera_control.look_at({ 40.0f,12.0f,12.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// ***************************************** //
	// Set-up 4 rooms
	// ***************************************** //

	float room1_length = 5.0f; //more than 4.0f
	float room2_length = 10.0f; //more than 4.0f
	float room3_length = 4.0f; //more than 4.0f
	float room4_length = 8.0f; //more than 4.0f
	float room_depth = 2.0f;
	float room_height = 2.0f;

	// room 1
	mesh room1_mesh = create_room_mesh(room1_length, room_depth, room_height);
	room1_mesh.apply_translation_to_position({0, 0, 0});
	room1.initialize_data_on_gpu(room1_mesh);
	
	room1.material.color = { 0.6f,0.85f,0.5f };

	//room 2
	mesh room2_mesh = create_room_mesh(room2_length, room_depth, room_height);
	room2_mesh.apply_translation_to_position({room1_length+1.0f, 0, 0});
	room2.initialize_data_on_gpu(room2_mesh);
	
	room2.material.color = { 0.5f,0.5f,0.7f };

	// room 3
	mesh room3_mesh = create_room_mesh(room3_length, room_depth, room_height);
	room3_mesh.apply_translation_to_position({room1_length+room2_length+2.0f, 0, 0});
	room3.initialize_data_on_gpu(room3_mesh);
	
	room3.material.color = { 0.7f,0.5f,0.5f };

	// room 4
	mesh room4_mesh = create_room_mesh(room4_length, room_depth, room_height);
	room4_mesh.apply_translation_to_position({room1_length+room2_length+room3_length+3.0f, 0, 0});
	room4.initialize_data_on_gpu(room4_mesh);
	
	room4.material.color = { 0.5f,0.5f,0.5f };

	// ***************************************** //
	// Set-up portals
	// ***************************************** //
	vec4 portal_vertices[] = {
    vec4(-1, -1, 0, 1),
    vec4( 1, -1, 0, 1),
    vec4(-1,  1, 0, 1),
    vec4( 1,  1, 0, 1),
  };
  for (unsigned int i = 0; i < sizeof(portal_vertices)/sizeof(portal_vertices[0]); i++) {
    portals[0].vertices.push_back(portal_vertices[i]);
    portals[1].vertices.push_back(portal_vertices[i]);
  }

  GLushort portal_elements[] = {
    0,1,2, 2,1,3,
  };
  for (unsigned int i = 0; i < sizeof(portal_elements)/sizeof(portal_elements[0]); i++) {
    portals[0].elements.push_back(portal_elements[i]);
    portals[1].elements.push_back(portal_elements[i]);
  }

  // 90° angle + slightly higher
  portals[0].object2world = translate(mat4(1), vec3(0, 1, -2));
  portals[1].object2world = rotate(mat4(1), -90.0f, vec3(0, 1, 0))
    * glm::translate(glm::mat4(1), glm::vec3(0, 1.2, -2));

  portals[0].upload();
  portals[1].upload();

  
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(room1, environment);
	draw(room2, environment);
	draw(room3, environment);
	draw(room4, environment);
	

	if (gui.display_wireframe){
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(room3, environment);
		draw_wireframe(room4, environment);
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
