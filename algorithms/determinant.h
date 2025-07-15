#pragma once

#include "../core/matrix.h"

namespace notlab
{   
    /**
     * @brief Calculates determinant by Laplace method.
     * 
     * @tparam T Type of Matrix.
     * @param matrix Matrix to calculate determinant.
     * @return float Determinant.
     */
    template<typename T>
    float determinantByLaplace(const Matrix<T>& matrix){
        if(!matrix.isSqure()){
            throw std::runtime_error("Matrix must be square to calculate determinant");
        }
        size_t dimensions = matrix.getNumberOfRows();
        if(dimensions == 1){
            return static_cast<float>(matrix(1,1));
        }
        if(dimensions == 2){
            return matrix(1,1) * matrix(2,2) - matrix(1,2) * matrix(2,1);
        }

        float determinant = 0;
        for(size_t i = 1; i<=dimensions; i++){
            Matrix<T> minor = matrix.minorMatrix(1, i);
            int sign = ((i % 2) ? 1 : -1);
            determinant += sign * matrix(1,i) * determinantByLaplace(minor);
        }

        return determinant;
    }

} // namespace notlab
