#pragma once

#include "../core/matrix.h"
#include "../core/vector.h"

namespace notlab{


    /**
     * @brief Performs forward Substitution.
     * 
     * @tparam T Type of Matrix and Vector.
     * @param L Square Matrix to substitute.
     * @param b Vector of length same as dimension of Matrix.
     * @return Vector<T> Substituted Vector.
     */
    template<typename T>
    Vector<T> forwardSubstitution(const Matrix<T>& L, const Vector<T>& b){
        size_t dimensionOfMatrix = L.getNumberOfRows();
        if(!L.isSqure()){
            throw std::runtime_error("Matrix must be square to perform substituition");
        }
        if(dimensionOfMatrix != b.getSize()){
            throw std::runtime_error("Number of element in Vector don't match with dimension of matrix");
        }

        Vector<T> substitudedVector = Vector<T>::zeros(dimensionOfMatrix);
        
        for(size_t i = 1; i<=dimensionOfMatrix; i++){
            T value = 0;
            for(size_t j = 1; j < i; j++){
                value += L(i,j) * substitudedVector(j);
            }
            substitudedVector(i) = (b(i) - value) / L(i,i);
        }

        return substitudedVector;
    }

    /**
    * @brief Performs backward Substitution.
    * 
    * @tparam T Type of Matrix and Vector.
    * @param L Square Matrix to substitute.
    * @param b Vector of length same as dimension of Matrix.
    * @return Vector<T> Substituted Vector.
    */
    template<typename T>
    Vector<T> backwardSubstitution(const Matrix<T>& U, const Vector<T>& b){
        size_t dimensionOfMatrix = U.getNumberOfRows();
        if(!U.isSqure()){
            throw std::runtime_error("Matrix must be square to perform substituition");
        }
        if(dimensionOfMatrix != b.getSize()){
            throw std::runtime_error("Number of element in Vector don't match with dimension of matrix");
        }

        Vector<T> substitudedVector = Vector<T>::zeros(dimensionOfMatrix);
        
        for(size_t i = dimensionOfMatrix; i >= 1; i--){
            T value = 0;
            for(size_t j = dimensionOfMatrix; j > i; j--){
                value += U(i,j) * substitudedVector(j);
            }
            substitudedVector(i) = (b(i) - value) / U(i,i);
        }

        return substitudedVector;
    }

} //notlab