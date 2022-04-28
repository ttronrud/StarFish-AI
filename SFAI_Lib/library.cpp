#include "library.h"

#include <iostream>
#include "fdeep/fdeep.hpp"


__declspec(dllexport) void __stdcall SFAI::set_model_name(std::string new_name)
{
    model_name = new_name;
}
__declspec(dllexport) void __stdcall SFAI::add_chunk(std::vector<float> new_chunk)
{
    chunks.push_back(new_chunk);
}
__declspec(dllexport) void __stdcall SFAI::add_chunk(float *new_chunk, int n)
{
    std::vector<float> nchunk;
    for(int i = 0; i < n; i++)
    {
        nchunk.push_back(new_chunk[i]);
    }
    add_chunk(nchunk);
}

__declspec(dllexport) void __stdcall SFAI::populate_chunks(int n)
{
    if(model_name.empty())
    {
        std::cout << "No model to generate dummy chunks for" << std::endl;
        return;
    }
    fdeep::model test = fdeep::load_model(model_name);
    for(int i = 0; i < n; i++)
    {
        add_chunk(test.generate_dummy_inputs()[0].to_vector());
        //test_ins.push_back(test.generate_dummy_inputs()[0].to_vector());
    }
}
//Generate a zero-ed dummy input and feed it through the prediction method
__declspec(dllexport) void __stdcall SFAI::hello()
{
    set_model_name("fdeep_model.json");

    //generate dummy input, convert to float vector, send to PredictChunk
    fdeep::model test = fdeep::load_model(model_name);
    std::vector<fdeep::tensor> dummy_in = test.generate_dummy_inputs();
    std::vector<float> nuVec = dummy_in[0].to_vector();
    float ret = PredictChunk(nuVec);
    std::cout << ret << std::endl;

    //Next, try the multi-chunk system
    std::vector<std::vector<float>> test_ins;
    for(int i = 0; i < 10; i++)
    {
        add_chunk(test.generate_dummy_inputs()[0].to_vector());
        //test_ins.push_back(test.generate_dummy_inputs()[0].to_vector());
    }
    std::vector<float> rets = PredictChunks();
    float total_est_beats = 0;
    for(int i = 0; i < rets.size(); i++)
    {
        total_est_beats += rets[i];
    }
    std::cout << "Total est. beats: " << total_est_beats << std::endl;
}

//Use the standard model to make a prediction on a chunk of ECG
__declspec(dllexport) float __stdcall SFAI::PredictChunk(std::vector<float> inputECGChunk)
{
    fdeep::model mod = fdeep::load_model("fdeep_model.json");
    fdeep::tensor t(fdeep::tensor_shape(2160,1),inputECGChunk);
    std::vector<fdeep::tensor> ts;
    ts.push_back(t);
    auto result = mod.predict(ts);
    return result[0].to_vector()[0];
}

__declspec(dllexport) std::vector<float> __stdcall SFAI::PredictChunks()
{
    std::vector<fdeep::tensor> ts;
    std::vector<fdeep::tensor> result;
    std::vector<float> res;
    fdeep::model mod = fdeep::load_model(model_name);
    for(int i = 0; i < chunks.size(); i++)
    {
        ts.clear();
        ts.push_back(fdeep::tensor(fdeep::tensor_shape(2160,1),chunks[i]));
        result = mod.predict(ts);
        res.push_back(result[0].to_vector()[0]);
    }
    return res;
}
