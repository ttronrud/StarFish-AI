#include "library.h"

#include <iostream>
#include "fdeep/fdeep.hpp"

//Constructor that takes in a name... Not technically necessary, but it's fun
//to LARP as OOP
SFAI::SFAI(std::string new_name)
{
    set_model_name(new_name);
}
//Sets the model name variable
void SFAI::set_model_name(std::string new_name)
{
    model_name = new_name;
}
//Adds a chunk (in a float vector form) to the object's storage
void SFAI::add_chunk(std::vector<float> new_chunk)
{
    chunks.push_back(new_chunk);
}
//Adds a chunk (in float array form) to the object's storage
void SFAI::add_chunk(float *new_chunk, int n)
{
    std::vector<float> nchunk;
    for(int i = 0; i < n; i++)
    {
        nchunk.push_back(new_chunk[i]);
    }
    add_chunk(nchunk);
}
//Populate the chunk storage with "n" dummy zero'ed input arrays
void SFAI::populate_chunks(int n)
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

//Generate zero-ed dummy inputs and feed them through the prediction method
//As a test function. Mostly deprecated now!
void SFAI::hello()
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

//Use the standard model to make a prediction on a chunk of ECG - not the stored chunks
float SFAI::PredictChunk(std::vector<float> inputECGChunk)
{
    std::cout << "loading model" << std::endl;
    fdeep::model mod = fdeep::load_model("fdeep_model.json");
    std::cout << "model loaded" << std::endl;
    fdeep::tensor t(fdeep::tensor_shape(2160,1),inputECGChunk);
    std::vector<fdeep::tensor> ts;
    ts.push_back(t);
    std::cout << "running predict" << std::endl;
    return mod.predict_single_output(ts);
}

//Use the stored model to make predictions on all stored chunks
//Add output to the stored result_beats per-chunk data
//and return the vector itself for further use
std::vector<float> SFAI::PredictChunks()
{
    try
    {
        std::cout << "Starting Prediction" << std::endl;
        std::vector<fdeep::tensor> ts;
        std::vector<fdeep::tensor> result;
        std::cout << "Loading model" << std::endl;

        fdeep::model mod = fdeep::load_model(model_name);


        for(int i = 0; i < chunks.size(); i++)
        {
            std::cout << "Pushing chunk" << i << std::endl;
            ts.clear();
            ts.push_back(fdeep::tensor(fdeep::tensor_shape(2160,1),chunks[i]));
            result_beats.push_back(mod.predict_single_output(ts));
        }
        return result_beats;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        return result_beats;
    }

}
void SFAI::GetResults(float* res, int* N)
{
    if(res == NULL || N == NULL)
        return; //don't try to write to

    *N = result_beats.size();
    for(int i = 0; i < result_beats.size(); i++)
    {
        res[i] = result_beats[i];
    }
}

//Clear stored data, retain model path
void SFAI::RESET()
{
    chunks.clear();
    result_beats.clear();
}
