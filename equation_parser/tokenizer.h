#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace notlab{
    
    enum class TokenType{
        None = 0, Number, Variable, Operator, LeftParentheses, RightParentheses, UnaryMinus
    };

    struct Token{
        TokenType type;
        std::string tokenContent;
    };

    bool isUnaryMinusAllowed(const std::vector<Token>& tokens) {
    
        if (tokens.empty())
            return true;
        TokenType prev = tokens.back().type;
        return prev == TokenType::Operator || prev == TokenType::LeftParentheses || prev == TokenType::UnaryMinus;
    }  

    bool isDigitAllowed(const std::vector<Token>& tokens){
        if(tokens.empty())
            return true;

        TokenType prev = tokens.back().type;

        return prev == TokenType::Operator || prev == TokenType::LeftParentheses || prev == TokenType::UnaryMinus;
    }

    bool isVariableAllowed(const std::vector<Token>& tokens){
        if(tokens.empty())
            return true;

        TokenType prev = tokens.back().type;

        return prev == TokenType::Operator || prev == TokenType::LeftParentheses || prev == TokenType::UnaryMinus;
    }

    bool isOperatorAllowed(const std::vector<Token>& tokens){
        if(tokens.empty())
            return false;

        TokenType prev = tokens.back().type;

        return prev == TokenType::RightParentheses || prev == TokenType::Number || prev == TokenType::Variable;
    }


    /**
     * @brief Tokenizing equation string
     * 
     * @param equation String representing mathematical equation
     * @return std::vector<Token> Vector of Tokens.
     */
    std::vector<Token> tokenize(const std::string& equation){
        std::vector<Token> tokens;

        int levelOfParentheses = 0;

        size_t i = 0;


        while(i < equation.size()){
            if(std::isspace(equation[i])){
                i++;
                continue;
            }

            if(std::isdigit(equation[i])){

                size_t digitStartPosition = i;

                while(i < equation.size() && (std::isdigit(equation[i]) || equation[i] == '.')){
                    i++;
                }

                if(isDigitAllowed(tokens)){
                    tokens.push_back({TokenType::Number, equation.substr(digitStartPosition, i-digitStartPosition)});
                }
                else{
                    throw std::runtime_error(std::string("Illegal character before number: ") + equation[i]);
                }

            }

            else if(std::isalpha(equation[i])){

                size_t variableStartPosition = i;

                while(i < equation.size() && (std::isalpha(equation[i]) || std::isdigit(equation[i]))){
                    i++;
                }

                if(isVariableAllowed(tokens)){
                    tokens.push_back({TokenType::Variable, equation.substr(variableStartPosition, i - variableStartPosition)});
                }
                else{
                    throw std::runtime_error(std::string("Illegal character before variable: ") + equation[i]);
                }
            }

            else if(equation[i] == '+' ||  equation[i] == '*' || equation[i] == '/' || equation[i] == '^'){
                if(isOperatorAllowed(tokens)){
                    tokens.push_back({TokenType::Operator, std::string(1, equation[i])});
                }
                else{
                    throw std::runtime_error(std::string("Illegal character before operator: ") + equation[i]);
                }

                i++;
            }

            else if(equation[i] == '-'){
                if(isUnaryMinusAllowed(tokens)){
                    tokens.push_back({TokenType::UnaryMinus, "-"});
                }
                else{
                    tokens.push_back({TokenType::Operator, "-"});
                }
                i++;
            }

            else if (equation[i] == '('){
                tokens.push_back({TokenType::LeftParentheses, "("});

                levelOfParentheses++;


                i++;
            }

            else if (equation[i] == ')'){
                tokens.push_back({TokenType::RightParentheses, ")"});

                levelOfParentheses--;
                if(levelOfParentheses < 0){
                    throw std::runtime_error("Number of right paranthesies is greater than left");
                }
                i++;
            }

            else{
                throw std::runtime_error(std::string("Unknow character: ") + equation[i]);
            }

        }

        if(levelOfParentheses > 0 ){
            throw std::runtime_error("Number of left paranthesies is greater than right");
        }
        if(tokens.back().type == TokenType::UnaryMinus || tokens.back().type == TokenType::Operator){
            throw std::runtime_error("Operator can't be placed at the end of equation");
        }

        return tokens;
    }

} // namespace notlab
