#pragma once

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include <array>
#include <algorithm>

#ifdef PHYSICS_USE_HIGH_PRECISION

typedef double p_dec;
typedef long long p_int;

#else

typedef float p_dec;
typedef int p_int;

#endif