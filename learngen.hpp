#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

/**
 * Returns the output for the function at the given index.
 */
double function(int index, const std::vector<double>& inputs) noexcept {
    switch (index) {
        // Identity function.
        case 0:
            return inputs[0];
        
        // Increment function.
        case 1:
            return inputs[0] + 1;
        
        // Double function.
        case 2:
            return 2 * inputs[0];
        
        // Square function.
        case 3:
            return inputs[0] * inputs[0];
        
        // Addition function.
        case 4:
            return inputs[0] + inputs[1];
        
        // Multiplication function.
        case 5:
            return inputs[0] * inputs[1];
        
        // 3-way addition function.
        case 6:
            return inputs[0] + inputs[1] + inputs[2];
            
        // 5-way variable coefficient addition.
        case 7:
            return inputs[0] + 2 * inputs[1] + 3 * inputs[2] + 4 * inputs[3] + 5 * inputs[4];
    }
    
    return 0;
}
