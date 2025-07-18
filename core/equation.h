#pragma once

#include <memory>
#include <vector>
#include <set>
#include "../equation_parser/ast.h"
#include "../equation_parser/tokenizer.h"
#include "../equation_parser/parser.h"

#include "vector.h"
#include "matrix.h"

namespace notlab
{
    class Equation{
        private:
            std::unique_ptr<Expression> m_expression;
            std::vector<Token> m_tokens;
            std::string m_equationString;

            std::vector<std::string> m_variables;

            void prepareVariables(){
                std::set<std::string> found;
                for(size_t i = 0; i < m_tokens.size(); i++){
                    if(m_tokens[i].type == TokenType::Variable && found.insert(m_tokens[i].tokenContent).second){
                        m_variables.push_back(m_tokens[i].tokenContent);
                    }
                }
            }

        public:
            Equation(const std::string& equationString): m_equationString(equationString){
                m_tokens = tokenize(m_equationString);
                prepareVariables();
                m_expression = std::move(parseTokens(m_tokens));
            }

            /**
             * @brief Evaluate equation with one variable and one value
             * 
             * @param variableValue Value to evaluate
             * @return float Value evaluated
             */
            float eval(float variableValue){
                if(m_variables.size() > 1){
                    throw std::runtime_error("Number of variables in equation is greater than 1");
                }   
                float val = m_expression->eval({{m_variables.back(), variableValue}});

                return val;
            }

            /**
             * @brief Evaluate equation with one variable and many values.
             * 
             * @param variableValues Values to evaluate.
             * @return VectorF Values evaluated
             */
            VectorF eval(const VectorF& variableValues){
                if(m_variables.size() > 1){
                    throw std::runtime_error("Number of variables in equation is greater than 1");
                }
                std::string variableName = m_variables.back();
                VectorF returnValues = VectorF::zeros(variableValues.getSize());
                for(size_t i = 1; i<= variableValues.getSize(); i++){
                    returnValues(i) = m_expression->eval({{variableName, variableValues(i)}});
                }
                return returnValues;
            }

            /**
             * @brief Evaluate equation with many variables and many values.
             * 
             * @param variablesValues Values to evaluate.
             * @return VectorF Values evaluated.
             */
            VectorF eval(const MatrixF& variablesValues){
                if(variablesValues.getNumberOfColums() != m_variables.size()){
                    std::string errorMsg("Number of given variables don't match up, expected: " + std::to_string((int)m_variables.size()));
                    throw std::runtime_error(errorMsg);
                }

                VectorF returnValues = VectorF::zeros(variablesValues.getNumberOfRows());

                for(size_t i = 1; i <= variablesValues.getNumberOfRows(); i++){
                    VectorF row = variablesValues.getRow(i);
                    std::map<std::string, float> rowMap;
                    for(size_t i = 1; i<= row.getSize(); i++){
                        rowMap.insert(std::pair<std::string, float>(m_variables[i-1], row(i)));
                    }

                    returnValues(i) = m_expression->eval(rowMap);
                }

                return returnValues;
            }
    };


} // namespace notlab
