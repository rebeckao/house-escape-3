#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
	float field_of_view = 1.0f; //radians
public:
	glm::mat4 model_matrix = glm::mat4(1.0f); // unit matrix, model is at origin of world space.
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

	glm::vec3 position = glm::vec3(0, 0, 5);
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	float horizontal_angle = 3.14f; //radians
	float vertical_angle = 0.0f; //radians

	Camera();

	void updateViewProjectionFromInput();
};

#endif