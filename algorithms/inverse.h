#pragma once
#include "../core/matrix.h"
#include "../core/vector.h"
#include "lu_decomposition.h"
#include "substitution.h"

namespace notlab{


    /**
     * @brief Inverse matrix using LU decomposition
     * 
     * @tparam T Type of matrix that is inversed
     * @param matrixToInverse matrix that is inverted
     * @return MatrixF inverted matrix
     */
    template<typename T>
    MatrixF inverseByLu(const Matrix<T>& matrixToInverse){
        if(!matrixToInverse.isSqure()){
            throw std::runtime_error("Matrix must be square to inverse");
        }

        int dimentionOfMatrix = matrixToInverse.getNumberOfColums();


        MatrixF identity = MatrixF::identity(dimentionOfMatrix);
        std::pair<MatrixF, MatrixF> luMatrix = gaussDollitle(matrixToInverse);

        MatrixF xMatrix = MatrixF::empty(matrixToInverse.getName() + "^-1");

        for(size_t k = 1; k<=dimentionOfMatrix; k++){

            //VectorF y = VectorF::zeros(dimentionOfMatrix);
            VectorF identityColumn = identity.getColumn(k);

            // for(size_t i = 1; i<=dimentionOfMatrix; i++){
            //     float value = 0;
            //     for(size_t j = 1; j < i; j++){
            //         value += luMatrix.first(i,j) * y(j);
            //     }
            //     y(i) = (identityColumn(i) - value) / luMatrix.first(i,i);
            // }
            VectorF y = forwardSubstitution(luMatrix.first, identityColumn);

            // VectorF x = VectorF::zeros(dimentionOfMatrix);

            // for(size_t i = dimentionOfMatrix; i>=1; i--){
            //     float value = 0;
            //     for(size_t j = dimentionOfMatrix; j>i; j--){
            //         value += luMatrix.second(i,j) * x(j);
            //     }
            //     x(i) = (y(i) - value)/luMatrix.second(i,i);
            // }

            VectorF x = backwardSubstitution(luMatrix.second, y);

            xMatrix.addColumn(x);

            
        }
        xMatrix.setInstruction("Inverse");
        return xMatrix;
    }

} //notlab