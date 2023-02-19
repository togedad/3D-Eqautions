//#include <cuda_runtime.h>
//#include "cuda_kernel.cuh"
//#include "device_launch_parameters.h"

#include "vectorMaths.h"
#include "mesh.h"
#include <iostream>


int sortTriangleDistToCameraKernal(std::vector<mesh> i, std::vector<triangle>& o);