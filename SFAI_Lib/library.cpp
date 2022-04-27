#include "library.h"

#include <iostream>
#include "fdeep/fdeep.hpp"
__declspec(dllexport) void __stdcall hello()
{
    fdeep::model test = fdeep::load_model("fdeep_model.json");
    std::vector<fdeep::tensor> dummy_in = test.generate_dummy_inputs();
    auto result = test.predict(dummy_in);
    std::cout << fdeep::show_tensors(result) << std::endl;
}
