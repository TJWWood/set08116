#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\projection.hpp>
#include <iostream>

using namespace std;
using namespace glm;

int main() {

	vec2 u(1.0f, 0.0f);
	vec2 v(1.0f, 0.0f);

	vec2 w = u + v;

	vec3 a(1.0f, 0.0f, 0.0f);
	vec3 b(0.0f, 1.0f, 0.0f);
	vec3 p = proj(a,b);

	vec4 k(0.0f, 1.0f, 1.0f, 1.0f);
	vec4 l(1.0f, 1.0f, 1.0f, 1.0f);

	

	vec2 u(24.0f, 10.0f);
	vec2 n = normalize(u);

}