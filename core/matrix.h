#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "vector.h"

#define TRACK_INSTRUCTION() setInstruction(__func__);

namespace notlab{
    /**
     * @class Matrix
     * @tparam T Element type (e.g. int, float, double)
     * @brief Dense mathematical matrix (1-based indexing)
     * @details
     *   Matrix class storing elements in row-major order. 
     *   Provides element access, static factories, and string conversion.
     */
    template<typename T>
    class Matrix{
        private:
            // m(3,1) -> 3row, 1col
            size_t m_numOfRows;
            size_t m_numOfCols;

            std::vector<T> m_data;
            std::string m_name;
            std::string m_lastInstruction;

            /**
             * @brief Private constructor from row and column count.
             * @param rows Number of rows.
             * @param cols Number of columns.
             * @param name Specifies name of Matrix.
             */
            Matrix(size_t rows, size_t cols, const std::string& name = "unnamed"): m_numOfRows(rows), m_numOfCols(cols), m_name(name),m_lastInstruction("none"),m_data(rows*cols){}

        public:
            /**
             * @brief Creates a zero matrix of given size.
             * @param rows Number of rows.
             * @param cols Number of columns.
             * @param name Specifies name of Matrix.
             * @return Matrix of shape (rows, cols) filled with zeros.
             */
            static Matrix<T> zeros(size_t rows, size_t cols, const std::string& name = "unnamed"){
                return Matrix<T>(rows, cols, name);
            }

            /**
             * @brief Creates an identity matrix (square) of given size.
             * @param size Number of rows and columns (matrix is square).
             * @param name Specifies name of Matrix.
             * @return Identity matrix of given size.
             */
            static Matrix<T> identity(size_t size,const std::string& name = "unnamed"){
                Matrix<T> ide(size, size,name);
                for(size_t i = 1; i<=size; i++){
                    ide(i,i) = static_cast<T>(1);
                }

                return ide;
            }

            /**
             * @brief Adds row to matrix.
             * @param row List of elements that is added to row.
             */
            void addRow(std::initializer_list<T> row){
                TRACK_INSTRUCTION();
                if(row.size() != m_numOfCols){
                    throw std::runtime_error("Length of columns dont match up");
                }
                m_data.insert(m_data.end(), row.begin(), row.end());
                m_numOfRows++;
                
            }

            /**
             * @brief Adds row to matrix.
             * 
             * @param row Vector of elements that is added to row.
             */
            void addRow(const Vector<T>& row){
                TRACK_INSTRUCTION();
                if(row.getSize() != m_numOfCols){
                    throw std::runtime_error("Length of columns dont match up");
                }
                m_data.insert(m_data.end(), row.getData().begin(), row.getData().end());
                m_numOfRows++;
            }

            /**
             * @brief Adds row to matrix.
             * 
             * @param row Matrix of elements that is added to row.
             */
            void addRow(const Matrix<T>& row){
                if(row.getNumberOfColums() != m_numOfCols){
                    throw std::runtime_error("Length of row dont match up");
                }
                TRACK_INSTRUCTION();
                m_data.insert(m_data.end(), row.getData().begin(), row.getData().end());
                m_numOfRows += row.getNumberOfRows();
            }

            /**
             * @brief Adds column to Matrix.
             * 
             * @param col List of elements that is added to column.
             */
            void addColumn(std::initializer_list<T> col){
                if(col.size() != m_numOfRows){
                    throw std::runtime_error("Lnegth of rows dont match up");
                }
                TRACK_INSTRUCTION();
                auto colIt = col.begin();

                for(size_t row = m_numOfRows; row-->0;){
                    size_t insertIndex = (row + 1) * m_numOfCols;
                    m_data.insert(m_data.begin() + insertIndex, *std::next(colIt,row));
                }
                m_numOfCols++;
            }

            /**
             * @brief Adds column to Matrix
             * 
             * @param col Vector of elements that is added to column;
             */
            void addColumn(const Vector<T>& col){
                if(col.getSize() != m_numOfRows){
                    throw std::runtime_error("Lnegth of rows dont match up");
                }
                TRACK_INSTRUCTION();
                auto colIt = col.getData().begin();

                for(size_t row = m_numOfRows; row-->0;){
                    size_t insertIndex = (row + 1) * m_numOfCols;
                    m_data.insert(m_data.begin() + insertIndex, *std::next(colIt,row));
                }
                m_numOfCols++;
            }

            /**
             * @brief Adds column to Matrix
             * 
             * @param col Matrix of elements that is added to column
             */
            void addColumn(const Matrix<T>& col){
                if(col.getNumberOfRows() != m_numOfRows){
                    throw std::runtime_error("Lnegth of rows dont match up");
                }
                TRACK_INSTRUCTION();
                m_data.reserve(m_data.size() + m_numOfRows * col.getNumberOfColums());
                for(size_t row = 0; row < m_numOfRows; row++){
                    size_t insertIndex = row * (m_numOfCols + col.getNumberOfColums()) + m_numOfCols;
                    m_data.insert(
                        m_data.begin() + insertIndex,
                        col.getData().begin() + row * col.getNumberOfColums(),
                        col.getData().begin() + (row + 1) * col.getNumberOfColums()
                    );
                }  
                m_numOfCols += col.getNumberOfColums();
            }

            
            /**
             * @brief Converts (row, col) to internal 1D index.
             * @param row Row number (1-based).
             * @param col Column number (1-based).
             * @throws std::runtime_error if index is out of bounds.
             * @return 1D array index.
             */
            size_t toIndex(size_t row, size_t col) const{
                if(row == 0 || row > m_numOfRows || col == 0 || col > m_numOfCols){
                    throw std::runtime_error("Index out of bounds");
                }
                return (row-1) * m_numOfCols + (col-1);
            }

            /**
             * @brief Returns the number of stored elements.
             * @return Total element count (rows × columns).
             */
            size_t getNumberOfElements() const{
                return m_data.size();
            }

            /**
             * @brief Returns the number of columns.
             * @return Number of columns.
             */         
            size_t getNumberOfColums() const{
                return m_numOfCols;
            }

            /**
             * @brief Returns the number of rows.
             * @return Number of rows.
             */
            size_t getNumberOfRows() const{
                return m_numOfRows;
            }

            /**
             * @brief Get the Data object.
             * 
             * @return const std::vector<T>&.
             */
            const std::vector<T>& getData() const{
                return m_data;
            }

            /**
             * @brief Returns a string representation of the matrix.
             * @return Matrix as a multi-line string.
             */
            std::string toString() const{
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

            /**
             * @brief Prints basic Matrix information.
             * 
             */
            void print(){
                std::cout << "-----" << std::endl;
                std::cout << "Matrix: " << m_name << std::endl;
                std::cout << toString() << std::endl;
                std::cout << "-----" << std::endl;
            }

            /**
             * @brief Prints all of Marix information.
             * 
             */
            void printAll(){
                std::cout << "-----" << std::endl;
                std::cout << "Matrix name: " << m_name << ", Dimension: " << m_numOfRows << "x" << m_numOfCols << std::endl;
                std::cout << "Last operation performed on Matrix: " << m_lastInstruction << std::endl;
                std::cout << toString() << std::endl;
                std::cout << "-----" << std::endl;
            }

            void setInstruction(const std::string& operation){
                m_lastInstruction = operation;
            }

            /**
             * @brief Element access (read/write) by (row, col).
             * @param row Row number (1-based).
             * @param col Column number (1-based).
             * @throws std::runtime_error if index is out of bounds.
             * @return Reference to element at (row, col).
             */
            T& operator()(size_t row, size_t col){
                size_t index = toIndex(row, col);
                return m_data[index];
            }

            /**
             * @brief Element access (read-only) by (row, col).
             * @param row Row number (1-based).
             * @param col Column number (1-based).
             * @throws std::runtime_error if index is out of bounds.
             * @return Const reference to element at (row, col).
             */
            const T& operator()(size_t row, size_t col) const{
                size_t index = toIndex(row, col);
                return m_data[index];
            }


    };

    using MatrixI = Matrix<int>;
    using MatrixF = Matrix<float>;
}