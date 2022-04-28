#ifndef SFAI_LIBRARY_H
#define SFAI_LIBRARY_H

#include <vector>
#include <string>

class SFAI{
private:
public:
    std::string model_name;

    std::vector<std::vector<float>> chunks;
    std::vector<float> result_beats;

    __declspec(dllexport) __stdcall SFAI(std::string new_name);

    __declspec(dllexport) void __stdcall set_model_name(std::string new_name);

    __declspec(dllexport) void __stdcall add_chunk(std::vector<float> new_chunk);

    __declspec(dllexport) void __stdcall add_chunk(float *new_chunk, int n);

    __declspec(dllexport) void __stdcall populate_chunks(int n);

    __declspec(dllexport) void __stdcall hello();

    __declspec(dllexport) float __stdcall PredictChunk(std::vector<float> inputECGChunk);

    __declspec(dllexport) std::vector<float> __stdcall PredictChunks();

    //populate a float pointer with the per-chunk results
    //populate an int pointer with the number of chunk results
    __declspec(dllexport) void __stdcall GetResults(float* res, int* N);

    __declspec(dllexport) void __stdcall RESET();
};


#endif //SFAI_LIBRARY_H
