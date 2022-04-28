//
// Created by ttronrud on 2022-04-27.
//

#include "SFAI_Console.h"
#include "library.h"
#include <vector>
#include <iostream>
#include <fstream>

int main()
{
    float tst_arr[2160];
    std::fstream fin;
    fin.open("test_dat_good.txt",std::ios::in);
    std::string tp;
    int i = 0;
    while(std::getline(fin, tp))
    {
        if(i >= 2160)
            break; //don't allow more than the max
        tst_arr[i] = (float)std::atof(tp.c_str());
        i++;
    }
    //if we're too few entries, populate with zeroes
    if(i < 2160-1)
    {
        while(i < 2160)
        {
            tst_arr[i] = 0;
            i++;
        }
    }

    SFAI _sfai;
    _sfai.set_model_name("fdeep_model.json");
    _sfai.add_chunk(tst_arr, 2160);
    //_sfai.populate_chunks(5);
    std::vector<float> results = _sfai.PredictChunks();
    float total_est_beats = 0;
    for(int i = 0; i < results.size(); i++)
    {
        total_est_beats += results[i];
    }
    std::cout << "Total est. beats: " << total_est_beats << std::endl;
    return 0;
}
