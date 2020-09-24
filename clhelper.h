#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

#include "clinclude.h"

void printPlatform(int i, cl::Platform& p);
void printDevice(int i, cl::Device& d);

bool initOpenCL(cl::Platform& plat, cl::Device& dev, cl::Context& con, cl::Program& prog, cl::CommandQueue& q, std::ifstream& sourcefile, int platid = -1, int devid = -1);
