#pragma once

#include "../core/matrix.h"

namespace notlab
{
    /**
     * @brief Decomposition of Matrix from Gauss Dollitle algorithm
     * 
     * @tparam T Type of matrix
     * @param matrixToDecompose Matrix that is decomposed 
     * @return std::pair<MatrixF, MatrixF> Pair of Matrix L and Matrix U
     */
    template<typename T>
    std::pair<MatrixF, MatrixF> gaussDollitle(const Matrix<T>& matrixToDecompose){
        if(!matrixToDecompose.isSqure()){
            throw std::runtime_error("Matrix must be square to decompose");
        }
        int dimentionOfMatrix = matrixToDecompose.getNumberOfColums();

        MatrixF uMatrix = MatrixF::zeros(dimentionOfMatrix, dimentionOfMatrix, "U");
        MatrixF lMatrix = MatrixF::identity(dimentionOfMatrix, "L");

        for(size_t i = 1; i<= dimentionOfMatrix; i++){
            for(size_t j = i; j <= dimentionOfMatrix; j++){
                float sum = 0;
                for(size_t k = 1; k<=i-1; k++){
                    sum += lMatrix(i,k) * uMatrix(k,j);
                }
                uMatrix(i,j) = matrixToDecompose(i,j) - sum;
            }
            for(size_t j = i+1; j<=dimentionOfMatrix; j++){
                float sum = 0;
                for(size_t k = 1; k<=i-1; k++){
                    sum += lMatrix(j,k) * uMatrix(k,i); 
                }   
                lMatrix(j,i) = (1/uMatrix(i,i)) * (matrixToDecompose(j,i) - sum);
            }
        }
        std::pair<MatrixF, MatrixF> matrixToReturn = std::make_pair(lMatrix, uMatrix);
        return matrixToReturn;
    }

} // namespace notlab
