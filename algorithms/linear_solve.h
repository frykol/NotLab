#pragma once

#include "../core/matrix.h"
#include "../core/vector.h"
#include "lu_decomposition.h"
#include "substitution.h"

namespace notlab
{
    /**
     * @brief Solves linear equation Ax = b
     * 
     * @tparam T 
     * @tparam U 
     * @param A Value Matrix.
     * @param b Result Vector.
     * @return VectorF X Vector.
     */
    template<typename T, typename U>
    VectorF linearSolveByLu(const Matrix<T>& A, const Vector<U>& b){
        if(!A.isSqure()){
            throw std::runtime_error("Matrix A must be square in order to solve equation");
        }


        //MatrixF matrixA = castMatrix<float>(A);
        VectorF vectorb = castVector<float>(b);

        std::pair<MatrixF, MatrixF> lu = gaussDollitle(A);

        VectorF y = forwardSubstitution(lu.first, vectorb);
        
        VectorF x = backwardSubstitution(lu.second, y);

        return x;

    }

    /**
     * @brief Solves linear equation Ax = b
     * 
     * @tparam T 
     * @tparam U 
     * @param A Value Matrix.
     * @param b result Matrix.
     * @return VectorF X vector.
     */
    template<typename T, typename U>
    VectorF linearSolveByLu(const Matrix<T>& A, const Matrix<U>& b){
        if(!A.isSqure()){
            throw std::runtime_error("Matrix A must be square in order to solve equation");
        }
        if(b.getNumberOfColums() != 1){
            throw std::runtime_error("Matrix A must have one column to solve equation");
        }
        Vector<U> bVector = b.getColumn(1);
        return linearSolveByLu(A, bVector);
    }


} // namespace notlab