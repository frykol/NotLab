#include <vector>
#include <string>
#include <sstream>

namespace notlab{
    template<typename T>
    class Matrix{
        private:
            // m(3,1) -> 3row, 1col
            size_t m_numOfRows;
            size_t m_numOfCols;

            std::vector<T> m_data;

            Matrix(size_t rows, size_t cols): m_numOfRows(rows), m_numOfCols(cols),m_data(rows*cols){}

        public:

            static Matrix<T> zeros(size_t rows, size_t cols){
                return Matrix<T>(rows, cols);
            }

            std::string toString(){
                std::stringstream ss;
                
                for(size_t i = 0; i<m_numOfRows; i++){
                    ss << "| ";
                    for(size_t j = 0; j<m_numOfCols; j++){
                        ss << m_data[i*m_numOfCols + j] << " ";
                    }
                    ss << "|\n";
                }

                return ss.str();
            }

            T& operator()(size_t row, size_t col){
                if(row - 1 > m_numOfRows || col -1 > m_numOfCols){
                    throw std::runtime_error("Index out of bounds");
                }
                return m_data[row-1 * m_numOfCols + col-1];
            }
    };

    using MatrixI = Matrix<int>;
    using MatrixF = Matrix<float>;
}