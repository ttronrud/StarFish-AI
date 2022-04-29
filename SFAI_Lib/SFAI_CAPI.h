//
// Created by ttronrud on 2022-04-28.
//

#ifndef SFAI_SFAI_CAPI_H
#define SFAI_SFAI_CAPI_H

#include <string>
#include <vector>
#include <iostream>
#include "library.h"

extern "C" {
__declspec(dllexport) void __stdcall CalcAll(char* model_name, float* all_chunks, int N, int N_chunks, float* ret_vals, int* N_ret_vals);
};


#endif //SFAI_SFAI_CAPI_H
