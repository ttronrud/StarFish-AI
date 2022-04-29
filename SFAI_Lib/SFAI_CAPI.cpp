//
// Created by ttronrud on 2022-04-28.
//
#include "SFAI_CAPI.h"

#define CHUNK_SIZE 2160

extern "C" __declspec(dllexport) void __stdcall CalcAll(char* model_name, float* all_chunks, int N, int N_chunks, float* ret_vals, int* N_ret_vals)
{
    std::cout << model_name << std::endl;
    float tmp[CHUNK_SIZE];
    int offset = 0;
    SFAI *_sfai = new SFAI(model_name);
    std::cout << "Model created!" << std::endl;
    //all_chunks is a flat array (for best compatibility)
    for(int i = 0; i < N_chunks; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            tmp[j] = all_chunks[i*CHUNK_SIZE + j];
        }
        //copy over this chunk
        _sfai->add_chunk(tmp, CHUNK_SIZE);
    }
    std::cout << "Chunks added!" << std::endl;
    _sfai->PredictChunks();
    std::cout << "chunks predicted!" << std::endl;
    _sfai->GetResults(ret_vals,N_ret_vals);
    std::cout << "Ref vals written" << std::endl;
}
#include "SFAI_CAPI.h"
