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

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost;
using namespace boost::multiprecision;

#ifdef PHYSICS_USE_HIGHER_PRECISION

typedef cpp_bin_float<128> p_dec;
typedef int128_t p_int;

#elif defined PHYSICS_USE_HIGH_PRECISION

typedef double p_dec;
typedef long long p_int;

#else

typedef float p_dec;
typedef int p_int;

#endif
