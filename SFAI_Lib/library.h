#ifndef SFAI_LIBRARY_H
#define SFAI_LIBRARY_H

#include <vector>
#include <string>

class SFAI {
private:

public:
    std::string model_name;

    std::vector<std::vector<float>> chunks;
    std::vector<float> result_beats;

    SFAI(std::string new_name);

    void set_model_name(std::string new_name);

    void add_chunk(std::vector<float> new_chunk);

    void add_chunk(float *new_chunk, int n);

    void populate_chunks(int n);

    void hello();

    float PredictChunk(std::vector<float> inputECGChunk);

    std::vector<float> PredictChunks();

    //populate a float pointer with the per-chunk results
    //populate an int pointer with the number of chunk results
    void GetResults(float *res, int *N);

    void RESET();
};


#endif //SFAI_LIBRARY_H
