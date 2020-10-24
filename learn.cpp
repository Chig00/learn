#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// The default data source.
constexpr const char* DATA_FILE = "learn.dat";

/**
 * A mathematical matrix that supports multiplication, transposition, and inversion.
 */
class Matrix {
    public:
        /**
         * Constructs an empty matrix.
         */
        Matrix() noexcept:
            data()
        {}
        
        /**
         * Constructs a matrix with size, but no values.
         */
        Matrix(int rows, int columns) noexcept:
            data(rows, std::vector<double>(columns))
        {}
        
        /**
         * Constructs a matrix with the given size and values.
         */
        Matrix(int rows, int columns, const std::vector<double>& data) noexcept:
            data(rows, std::vector<double>(columns))
        {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                    this->data[i][j] = data[i * columns + j];
                }
            }
        }
        
        /**
         * Returns the result of matrix transposition on this matrix.
         */
        Matrix transpose() const noexcept {
            // The resultant matrix.
            Matrix transposition(data[0].size(), data.size());
            
            // The data is transposed.
            for (int i = 0; i < data.size(); ++i) {
                for (int j = 0; j < data[i].size(); ++j) {
                    transposition.data[j][i] = data[i][j];
                }
            }
            
            // The result is returned.
            return transposition;
        }
        
        /**
         * Returns the result of multiplying this matrix with the given matrix.
         */
        Matrix multiply(const Matrix& matrix) const noexcept {
            // The resultant matrix.
            Matrix product(data.size(), matrix.data[0].size());
            
            // Each cell is computed.
            for (int i = 0; i < data.size(); ++i) {
                for (int j = 0; j < matrix.data[0].size(); ++j) {
                    for (int k = 0; k < data[i].size(); ++k) {
                        product.data[i][j] += data[i][k] * matrix.data[k][j];
                    }
                }
            }
            
            // The result is returned.
            return product;
        }
        
        /**
         * Returns the result of matrix inversion on this matrix.
         */
        Matrix invert() const noexcept {
            // A copy of this matrix is made for computation.
            Matrix clone(*this);
            
            // The resultant matrix.
            Matrix inverse(data.size(), data.size());
            
            // The inverse is initialised to the identity.
            for (int i = 0; i < data.size(); ++i) {
                inverse.data[i][i] = 1;
            }
            
            // Gaussian elimation algorithm.
            for (int h = 0, k = 0; h < data.size() && k < data.size(); ++k) {
                int i_max = h;
                
                // The pivot is found.
                for (int i = h + 1; i < data.size(); ++i) {
                    if (std::abs(clone.data[i_max][k]) < std::abs(clone.data[i][k])) {
                        i_max = i;
                    }
                }
                
                // A pivot was found.
                if (clone.data[i_max][k]) {
                    // The greatest valued row is swapped to the top.
                    std::vector<double> temp(clone.data[h]);
                    clone.data[h] = clone.data[i_max];
                    clone.data[i_max] = temp;
                    
                    temp = inverse.data[h];
                    inverse.data[h] = inverse.data[i_max];
                    inverse.data[i_max] = temp;
                    
                    // The lower rows are reduced.
                    for (int i = h + 1; i < data.size(); ++i) {
                        double f = clone.data[i][k] / clone.data[h][k];
                        
                        for (int j = 0; j < data.size(); ++j) {
                            clone.data[i][j] -= clone.data[h][j] * f;
                            inverse.data[i][j] -= inverse.data[h][j] * f;
                        }
                    }
                    
                    ++h;
                }
            }
            
            for (int i = data.size() - 1; i >= 0; --i) {
                for (int j = data.size() - 1; j >= 0; --j) {
                    if (j == i) {
                        for (int k = 0; k < data.size(); ++k) {
                            inverse.data[i][k] /= clone.data[i][j];
                        }
                        
                        for (int k = 0; k < data.size(); ++k) {
                            clone.data[i][k] /= clone.data[i][j];
                        }
                        
                        break;
                    }
                    
                    else {
                        for (int k = 0; k < data.size(); ++k) {
                            inverse.data[i][k] -= clone.data[i][j] * inverse.data[j][k];
                        }
                        
                        for (int k = 0; k < data.size(); ++k) {
                            clone.data[i][k] -= clone.data[i][j] * clone.data[j][k];
                        }
                    }
                }
            }
            
            // The result is returned.
            return inverse;
        }
        
        /**
         * Outputs a representation of the matrix to standard output.
         */
        void print() const noexcept {
            std::cout << std::endl;
            
            for (int i = 0; i < data.size(); ++i) {
                for (int j = 0; j < data[i].size(); ++j) {
                    std::cout << data[i][j] << ' ';
                }
                
                std::cout << std::endl;
            }
        }
    
    private:
        // The values stored within the matrix.
        std::vector<std::vector<double>> data;
};

/**
 * Learns the data in the given data file to make predictions for new data.
 * Uses multivariate regression to do so.
 * The data file should have the following format:
 *     [entry count] [input count] [output count]
 *     [inputs separated by spaces] [outputs separated by spaces]
 *     [inputs separated by spaces] [outputs separated by spaces]
 *     ...
 *     [prediction count]
 *     [inputs to have outputs predicted separated by spaces]
 *     [inputs to have outputs predicted separated by spaces]
 *     ...
 */
int main(int argc, char** argv) {
    // The name of the data file.
    const char* filename;
    
    // The data file's name was provided through the command line.
    if (argc > 1) {
        filename = argv[1];
    }
    
    // The default data file should be used.
    else {
        filename = DATA_FILE;
    }
    
    // The data file is opened for reading.
    std::ifstream file(filename);
    
    // The number of data entries.
    int entry_count;
    
    // The number of inputs per data entry.
    int input_count;
    
    // The number of outputs per data entry.
    int output_count;
    
    // The data format is extracted.
    file >> entry_count >> input_count >> output_count;
    
    // The input count is incremented for the column of 1s in the input matrix.
    ++input_count;
    
    // The vectors storing inputs and outputs are initialised.
    std::vector<double> inputs(entry_count * input_count);
    std::vector<double> outputs(entry_count * output_count);
    
    // The inputs and outputs are extracted from the data file.
    for (int i = 0; i < entry_count; ++i) {
        // Each line of input is prepended with a 1 (for the input matrix).
        inputs[i * input_count] = 1;
        
        // Inputs are extracted.
        for (int j = 1; j < input_count; ++j) {
            file >> inputs[i * input_count + j];
        }
        
        // Outputs are extracted.
        for (int j = 0; j < output_count; ++j) {
            file >> outputs[i * output_count + j];
        }
    }
    
    // The input matrix is constructed.
    Matrix x(entry_count, input_count, inputs);
    
    // The output matrix is constructed.
    Matrix y(entry_count, output_count, outputs);
    
    // The estimator is constructed.
    Matrix b(x.transpose().multiply(x).invert().multiply(x.transpose()).multiply(y));
    
    // The prediction count is extracted.
    file >> entry_count;
    
    // The desired predictions are extratced from the data file.
    // The inputs and outputs are extracted from the data file.
    for (int i = 0; i < entry_count; ++i) {
        // Each line of input is prepended with a 1 (for the input matrix).
        inputs[i * input_count] = 1;
        
        for (int j = 1; j < input_count; ++j) {
            file >> inputs[i * input_count + j];
        }
    }
    
    // The prediction input matrix is constructed.
    Matrix px(entry_count, input_count, inputs);
    
    // The prediction output matrix is constructed and displayed.
    Matrix py(px.multiply(b));
    py.print();
    
    // Succesful termination.
    return 0;
}