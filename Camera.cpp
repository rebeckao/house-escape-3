#include "Camera.h"

Camera::Camera(){}

void Camera::updateViewProjectionFromInput(){

	using namespace glm;
	//Spherical coordinates to Cartesian coordinates conversion
	direction = vec3(cos(vertical_angle) * sin(horizontal_angle), sin(vertical_angle), cos(vertical_angle) * cos(horizontal_angle));
	right = vec3(sin(horizontal_angle - 3.14f / 2.0f), 0, cos(horizontal_angle - 3.14f / 2.0f));
	up = cross(right, direction);

	// Projection matrix : Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection_matrix = perspective(field_of_view, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	view_matrix = lookAt(position, position + direction, up);
}