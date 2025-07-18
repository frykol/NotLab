#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "vector.h"

#define TRACK_INSTRUCTION() setInstruction(__func__);

namespace notlab{

    /**
     * @struct is_matrix_type_valid
     * @brief Type trait to restrict allowed types for Matrix.
     * @tparam T Type to check.
     */
    template<typename T>
    struct is_matrix_type_valid : std::false_type{};

    /// Specialization: int is allowed.
    template<> struct is_matrix_type_valid<int>: std::true_type{};
    /// Specialization: float is allowed.
    template<> struct is_matrix_type_valid<float>: std::true_type{};
    /// Specialization: double is allowed.
    template<> struct is_matrix_type_valid<double>: std::true_type{};

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
        static_assert(is_matrix_type_valid<T>::value, "This type is not valid");
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
             * @brief Creates an empty matrix
             * 
             * @param name Specifies name of Matrix
             * @return Matrix<T> Empty matrix
             */
            static Matrix<T> empty(const std::string& name = "unnamed"){
                return Matrix<T>(0,0,name);
            }

            static Matrix<T> fromList(std::initializer_list<std::initializer_list<T>> rowsList, const std::string& name = "unnamed"){
                size_t numberOfRows = rowsList.size();
                size_t numberOfColumns = rowsList.begin()->size();
                Matrix<T> matrixFromList = Matrix<T>::empty(name);
                for(const auto& row : rowsList){
                    if (row.size() != numberOfColumns){
                        throw std::runtime_error("Inconsistent row size");
                    }
                    matrixFromList.addRow(row);
                }
                return matrixFromList;
            }

            /**
             * @brief Converts Vector to Matrix
             * 
             * @param vectorToMatrix Vector that is converted to Matrix
             * @param rows Number of rows
             * @param cols Number of columns
             * @return Matrix<T> Matrix constructed from Vector
             */
            static Matrix<T> fromVector(const Vector<T>& vectorToMatrix, size_t rows, size_t cols){
                if(rows * cols != vectorToMatrix.getSize()){
                    throw std::runtime_error("Dimentions of Matrix don't match with number of elements in vector");
                }
                Matrix<T> matrix = zeros(rows, cols);
                size_t index = 1;
                for(size_t row = 1; row <= rows; row++){
                    for(size_t col = 1; col <= cols; col++){
                        matrix(row,col) = vectorToMatrix(index);
                        index++;
                    }
                }
                return matrix;

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
             * @brief Transposing given matrix.
             * 
             * @param matrixToTranspose Matrix to transpose.
             * @return Matrix<T> Transposed Matrix.
             */
            static Matrix<T> transpose(const Matrix<T>& matrixToTranspose){
                Matrix<T> transposedMatrix = zeros(matrixToTranspose.getNumberOfColums(), matrixToTranspose.getNumberOfRows(), matrixToTranspose.getName() + "^T");
                transposedMatrix.setInstruction("transpose");
                for (size_t row = 1; row <= matrixToTranspose.getNumberOfRows(); row++) {
                    for (size_t column = 1; column <= matrixToTranspose.getNumberOfColums(); column++) {
                        transposedMatrix(column, row) = matrixToTranspose(row, column);
                    }
                }
                return transposedMatrix;
            }   

            /**
             * @brief transpose Matrix
             * 
             */
            void transpose(){
                TRACK_INSTRUCTION();
                std::vector<T> newData(m_numOfRows * m_numOfCols);
                for(size_t row = 1; row <= m_numOfRows; row++){
                    for(size_t column = 1; column <= m_numOfCols; column++){
                        newData[(column-1) * m_numOfRows + (row-1)] = m_data[(row-1) * m_numOfCols + (column-1)];
                    }
                    
                }
                std::swap(m_numOfRows, m_numOfCols);
                m_data = std::move(newData);
            }

            /**
             * @brief Converts Matrix to flat vector
             * 
             * @return Vector<T> flat Matrix
             */
            Vector<T> toVector(){
                Vector<T> matrixToVector = Vector<T>::zeros(0);
                matrixToVector.addBack(m_data, 0, getNumberOfElements());
                return matrixToVector;
            }

            /**
             * @brief Adds row to matrix.
             * @param row List of elements that is added to row.
             */
            void addRow(std::initializer_list<T> row){
                
                if((row.size() != m_numOfCols) && m_numOfCols != 0 && m_numOfRows != 0){
                    throw std::runtime_error("Length of columns dont match up");
                }
                TRACK_INSTRUCTION();
                m_data.insert(m_data.end(), row.begin(), row.end());
                m_numOfRows++;
                if(m_numOfCols == 0){
                    m_numOfCols = row.size();
                }
                
            }

            /**
             * @brief Adds row to matrix.
             * 
             * @param row Vector of elements that is added to row.
             */
            void addRow(const Vector<T>& row){
                
                if((row.getSize() != m_numOfCols) && m_numOfCols != 0 && m_numOfRows != 0){
                    throw std::runtime_error("Length of columns dont match up");
                }
                TRACK_INSTRUCTION();
                m_data.insert(m_data.end(), row.getData().begin(), row.getData().end());
                m_numOfRows++;
                if(m_numOfCols == 0){
                    m_numOfCols = row.getSize();
                }
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
             * @brief Get the row of Matrix
             * 
             * @param row index of row
             * @return Vector<T> Vector of row
             */
            Vector<T> getRow(size_t row) const{
                if(row < 1 || row > m_numOfRows){
                    throw std::runtime_error("Row out of bounds");
                }

                Vector<T> rowVector = Vector<T>::zeros(0);
                size_t firstElement = toIndex(row, 1);
                rowVector.addBack(m_data, firstElement, firstElement+m_numOfCols);
                return rowVector;
            }

            /**
             * @brief Adds column to Matrix.
             * 
             * @param col List of elements that is added to column.
             */
            void addColumn(std::initializer_list<T> col){
                if((col.size() != m_numOfRows) && m_numOfCols != 0 && m_numOfRows != 0){
                    throw std::runtime_error("Length of rows dont match up");
                }
                TRACK_INSTRUCTION();
                auto colIt = col.begin();

                if(m_numOfRows == 0){
                    m_data.reserve(col.size());
                    m_numOfRows = col.size();
                }

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
                if((col.getSize() != m_numOfRows) && m_numOfCols != 0 && m_numOfRows != 0){
                    throw std::runtime_error("Length of rows dont match up");
                }
                TRACK_INSTRUCTION();
                auto colIt = col.getData().begin();

                if(m_numOfRows == 0){
                    m_data.reserve(col.getSize());
                    m_numOfRows = col.getSize();
                }

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
                    throw std::runtime_error("Length of rows dont match up");
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
             * @brief Get the column of Matrix
             * 
             * @param column index of column
             * @return Vector<T> Vector of column 
             */
            Vector<T> getColumn(size_t column) const{
                if(column > m_numOfCols || column < 1){
                    throw std::runtime_error("column out of bounds");
                }
                Vector<T> columnVector = Vector<T>::zeros(m_numOfRows);

                for(size_t i=1; i<=m_numOfRows; i++){
                    columnVector(i) = m_data[toIndex(i, column)];
                }
                return columnVector;
            }

            /**
             * @brief Calculates minor Matrix.
             * 
             * @param omitRow Rows that must be omited.
             * @param omitColumn Columns that must be omited.
             * @return Matrix<T> Minor matrix.
             */
            Matrix<T> minorMatrix(size_t omitRow, size_t omitColumn) const{
                if(getNumberOfElements() == 0){
                    throw std::runtime_error("Can't calculate minor for empty matrix");
                }
                if(omitRow <= 0 || omitRow > m_numOfRows || omitColumn <= 0 || omitColumn > m_numOfCols){
                    throw std::runtime_error("Index out of bound when calculation matrix minor");
                }
                Matrix<T> minor = Matrix<T>::empty();
                for(size_t row = 1; row <= m_numOfRows; row++){
                    Vector<T> rowVector = Vector<T>::zeros(0);
                    for(size_t column = 1; column <= m_numOfCols; column++){
                        if(row == omitRow || column == omitColumn){
                            continue;
                        }
                        rowVector.addBack(m_data[toIndex(row,column)]);
                    }
                    if(rowVector.getSize() != 0){
                        minor.addRow(rowVector);
                    }
                }
                return minor;
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
             * @brief Checking if matrix is square
             * 
             * @return true if is square
             * @return false if is not square
             */
            bool isSqure() const{
                return m_numOfCols == m_numOfRows;
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
            void print() const{
                std::cout << "-----" << std::endl;
                std::cout << "Matrix: " << m_name << std::endl;
                std::cout << toString() << std::endl;
                std::cout << "-----" << std::endl;
            }

            /**
             * @brief Prints all of Marix information.
             * 
             */
            void printAll() const{
                std::cout << "-----" << std::endl;
                std::cout << "Matrix name: " << m_name << ", Dimension: " << m_numOfRows << "x" << m_numOfCols << std::endl;
                std::cout << "Last operation performed on Matrix: " << m_lastInstruction << std::endl;
                std::cout << toString() << std::endl;
                std::cout << "-----" << std::endl;
            }

            /**
             * @brief Set the last instruction performed on matrix
             * 
             * @param operation name of instruction
             */
            void setInstruction(const std::string& operation){
                m_lastInstruction = operation;
            }

            /**
             * @brief Set the name of matrix
             * 
             * @param name 
             */
            void setName(const std::string& name){
                m_name = name;
            }

            /**
             * @brief Get the name of matrix
             * 
             * @return std::string 
             */
            std::string getName() const{
                return m_name;
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


    template<typename T, typename U, typename Op>
    auto elementwise(const Matrix<T>& left, const Matrix<U>& right, Op operation){
        if((left.getNumberOfRows() != right.getNumberOfRows()) || (left.getNumberOfColums() != right.getNumberOfColums())){
            throw std::runtime_error("Numbers of columns or rows don't match up");
        }

        using resultType = decltype(operation(T(), U()));
        Matrix<resultType> resultMatrix = Matrix<resultType>::zeros(left.getNumberOfRows(), left.getNumberOfColums());
        for(size_t row = 1; row<=left.getNumberOfRows(); row++){
            for(size_t column = 1; column<=left.getNumberOfColums(); column++){
                resultMatrix(row, column) = operation(static_cast<resultType>(left(row,column)),static_cast<resultType>(right(row,column)));
            }
        }
        return resultMatrix;
    }

    template<typename T, typename U>
    auto operator+(const Matrix<T>& left, const Matrix<U>& right){ 
        auto matrix = elementwise(left, right, std::plus<>());
        matrix.setName(left.getName() + "+" + right.getName());
        matrix.setInstruction("Addition");
        return matrix;
    }

    template<typename T, typename U>
    auto operator-(const Matrix<T>& left, const Matrix<U>& right){
        auto matrix = elementwise(left, right, std::minus<>());
        matrix.setName(left.getName() + "-" + right.getName());
        matrix.setInstruction("Subtraction");
        return matrix;
    }

    template<typename T, typename U>
    auto operator*(const Matrix<T>& left, const Matrix<U>& right){
        if(left.getNumberOfColums() != right.getNumberOfRows()){
            throw std::runtime_error("Number of columns and rows dont match up");
        }

        using resultType = decltype(T() * U());
        Matrix<resultType> resultMatix = Matrix<resultType>::zeros(left.getNumberOfRows(), right.getNumberOfColums(), left.getName() + "*" + right.getName());
        resultMatix.setInstruction("Multiplication");
        
        for(size_t row = 1; row <= left.getNumberOfRows(); row++){
            for(size_t column = 1; column <= right.getNumberOfColums(); column++){
                resultType value = 0;
                for(size_t k = 1; k<=left.getNumberOfColums(); k++){
                    value += left(row,k) * right(k, column);
                }
                resultMatix(row, column) = value;
            }
        }
        return resultMatix;
    }

    template<typename T, typename U>
    auto operator*(const Matrix<T>& left, const U& scalar){
        using resultType = decltype(T() * U());
        Matrix<resultType> resultMatrix = Matrix<resultType>::zeros(left.getNumberOfRows(),left.getNumberOfColums(),left.getName());
        for(size_t row = 1; row<=left.getNumberOfRows(); row++){
            for(size_t column = 1; column<=left.getNumberOfColums(); column++){
                resultMatrix(row, column) = left(row, column) * scalar;
            }
        }
        resultMatrix.setInstruction("Multiplication");
        return resultMatrix;
    }

    template<typename T, typename U>
    auto operator*(const T& scalar, const Matrix<U>& left){
        return left * scalar;
    }

    template<typename T, typename U>
    auto operator/(const Matrix<T>& left, const Matrix<U>& right){
        if(left.getNumberOfColums() != right.getNumberOfRows()){
            throw std::runtime_error("Number of columns and rows dont match up");
        }


        auto rightInverse = inverseByLu(right); 
        using resultType = decltype(T() * float());
        Matrix<resultType> resultMatix = Matrix<resultType>::zeros(left.getNumberOfRows(), right.getNumberOfColums(), left.getName() + "/" + right.getName());
        resultMatix.setInstruction("Division");
               
        for(size_t row = 1; row <= left.getNumberOfRows(); row++){
            for(size_t column = 1; column <= right.getNumberOfColums(); column++){
                resultType value = 0;
                for(size_t k = 1; k<=left.getNumberOfColums(); k++){
                    value += left(row,k) * rightInverse(k, column);
                }
                resultMatix(row, column) = value;
            }
        }
        return resultMatix;
    }

    /**
     * @brief Checks if two Matrieses are equal.
     * 
     * @tparam T Type of first matrix.
     * @tparam U Type of second matrix.
     * @param first First matrix.
     * @param second Second Matrix.
     * @return true If Matrieses are equal.
     * @return false If Matrieses are not equal.
     */
    template<typename T, typename U>
    bool isMatrixEqual(const Matrix<T>& first, const Matrix<U>& second){
        if((first.getNumberOfColums() != second.getNumberOfColums()) || (first.getNumberOfRows() != second.getNumberOfRows())){
            return false;
        }

        if(first.getNumberOfElements() == 0){
            return true;
        }

        for(size_t row = 1; row <= first.getNumberOfRows(); row++){
            for(size_t col = 1; col <= first.getNumberOfColums(); col++){
                if(first(row,col) != second(row, col)){
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * @brief Cast matrix to different type.
     * 
     * @tparam T Original type.
     * @tparam U Type to cast.
     * @param matrix Matrix to cast.
     * @return Matrix<U> Casted Matrix.
     */
    template<typename U, typename T>
    Matrix<U> castMatrix(const Matrix<T>& matrix){
        Matrix<U> matrixToReturn = Matrix<U>::zeros(matrix.getNumberOfRows(), matrix.getNumberOfColums());
        for(size_t row = 1; row <= matrix.getNumberOfRows(); row++){
            for(size_t column = 1; column <= matrix.getNumberOfColums(); column++){
                matrixToReturn(row,column) = static_cast<U>(matrix(row, column));
            }
        }
        return matrixToReturn;
    }
}