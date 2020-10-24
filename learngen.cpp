#include "learngen.hpp"

// The minimum number of arguments to be passed through the command line.
constexpr int ARGC = 6;

/**
 * Outputs the inputs and desired functions' outputs to the file.
 */
void output(
    std::ofstream& file,
    const std::vector<int>& indices,
    const std::vector<double>& inputs
) noexcept {
    for (int i = 0; i < inputs.size(); ++i) {
        file << inputs[i] << ' ';
    }
    
    for (int i = 0; i < indices.size(); ++i) {
        file << function(indices[i], inputs) << ' ';
    }
    
    file << '\n';
}

/**
 * Allows for a variable number of inputs to be iterated through.
 */
void variable_loop(
    std::ofstream& file,
    int input_count,
    double min,
    double max,
    double step,
    const std::vector<int>& indices,
    std::vector<double>& inputs,
    int index = 0
) noexcept {
    // The vector of inputs has been built.
    if (index == input_count) {
        output(file, indices, inputs);
    }
    
    // The vector of inputs has not been fully built, yet.
    else {
        for (inputs[index] = min; inputs[index] <= max; inputs[index] += step) {
            variable_loop(file, input_count, min, max, step, indices, inputs, index + 1);
        }
    }
}

/**
 * A program that generates data for the machine to learn.
 * Generates data using an inclusive range of inputs given to the given output file.
 * Functions to be learnt must be specified by their index.
 * The argument syntax is as follows:
 *     [output filename] [input count] [range min] [range max] [range step] [function indices]
 * This program only produces raw data and does not produce the learning metadata.
 */
int main(int argc, char** argv) {
    // Sufficient number of arguments.
    if (argc > ARGC) {
        // The filename and range are extracted.
        const char* filename = argv[1];
        int input_count = std::stoi(argv[2]);
        double min = std::stod(argv[3]);
        double max = std::stod(argv[4]);
        double step = std::stod(argv[5]);
        
        // The function indices.
        std::vector<int> indices(argc - ARGC);
        
        // The function indices are extracted.
        for (int i = ARGC; i < argc; ++i) {
            indices[i - ARGC] = std::stoi(argv[i]);
        }
        
        // The file is opened.
        std::ofstream file(filename);
        
        // A vector of the function inputs (to be built using recursion).
        std::vector<double> inputs(input_count);
        
        // A variably sized iteration over the range.
        variable_loop(file, input_count, min, max, step, indices, inputs);
        
        // Successful termination.
        return 0;
    }
    
    // Insufficient number of arguments.
    else {
        std::cerr << "\nInsufficient argument count.\n";
        return 1;
    }
}