//
// Created by ttronrud on 2022-04-27.
//

#include "SFAI_Console.h"

std::vector<float> load_ecg(std::string fname)
{
    std::vector<float> tst_arr;
    std::fstream fin;
    fin.open(fname,std::ios::in);
    std::string tp;
    int i = 0;
    while(std::getline(fin, tp))
    {
        if(i >= 2160)
            break; //don't allow more than the max
        tst_arr.push_back((float)std::atof(tp.c_str()));
        i++;
    }
    //if we're too few entries, populate with zeroes
    if(i < 2160-1)
    {
        while(i < 2160)
        {
            tst_arr.push_back(0);
            i++;
        }
    }
    return tst_arr;
}

int main(int argc, char * argv[])
{
    float SEC_PER_CHUNK = 6;
    std::vector<std::string> fnames;
    std::string fname = "test_dat_bad.txt";
    //get first CL arg as filename
    if(argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            fnames.push_back(argv[i]);
        }
    }
    else
    {
        fnames.push_back(fname);
    }

    //Bones of the actual stuff:
    float test_results[64]; //should be higher lel
    int N_tst_res = 0;

    SFAI *_sfai = new SFAI("fdeep_model.json");

    //add all chunks to SFAI
    for(int i = 0; i < fnames.size(); i++)
    {
        _sfai->add_chunk(load_ecg(fnames[i]));
    }

    //std::vector<float> results = _sfai->PredictChunks();
    //Instead of getting the return, we'll try the more "external library" way
    _sfai->PredictChunks();
    _sfai->GetResults(test_results,&N_tst_res);
    float total_est_beats = 0;
    for(int i = 0; i < N_tst_res; i++)
    {
        std::cout << "Chunk " << i << " beats: " << test_results[i] << "\tHR est: " << 60*test_results[i]/SEC_PER_CHUNK <<std::endl;
        total_est_beats += test_results[i];
    }
    std::cout << "Total est. beats: " << total_est_beats << std::endl;
    std::cout << "HR est: " << 60*total_est_beats/(N_tst_res*SEC_PER_CHUNK) << std::endl;
    return 0;
}
