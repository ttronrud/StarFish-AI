#ifndef SFAI_LIBRARY_H
#define SFAI_LIBRARY_H

#include <vector>
#include <string>
//If really necessary can include it here
//#include "C:\Users\ttronrud\starfish\vcpkg\installed\x86-windows\include\fdeep\fdeep.hpp"

class SFAI{
private:
public:
    std::string model_name;
    std::vector<std::vector<float>> chunks;

    __declspec(dllexport) void __stdcall set_model_name(std::string new_name);

    __declspec(dllexport) void __stdcall add_chunk(std::vector<float> new_chunk);

    __declspec(dllexport) void __stdcall add_chunk(float *new_chunk, int n);

    __declspec(dllexport) void __stdcall populate_chunks(int n);

    __declspec(dllexport) void __stdcall hello();

    __declspec(dllexport) float __stdcall PredictChunk(std::vector<float> inputECGChunk);

    __declspec(dllexport) std::vector<float> __stdcall PredictChunks();
};


#endif //SFAI_LIBRARY_H
