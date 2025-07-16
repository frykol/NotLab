#pragma once

#include <vector>
#include <string>

#include <iostream>

#define OFFSET_TO_ELEMENT_BEFORE 2

namespace notlab{
    
    enum class TokenType{
        None = 0, Number, Variable, Operator, LeftParentheses, RightParentheses, UnaryMinus
    };

    struct Token{
        TokenType type;
        std::string tokenContent;
    };

    bool isUnaryMinusAllowed(const std::vector<Token>& tokens) {
    
        if (tokens.empty()) return true;
        TokenType prev = tokens.back().type;
        return prev == TokenType::Operator || prev == TokenType::LeftParentheses;
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
                tokens.push_back({TokenType::Number, equation.substr(digitStartPosition, i-digitStartPosition)});

                if(tokens.size() > OFFSET_TO_ELEMENT_BEFORE 
                && tokens[tokens.size() - OFFSET_TO_ELEMENT_BEFORE].type != TokenType::Operator
                && tokens[tokens.size() - OFFSET_TO_ELEMENT_BEFORE].type != TokenType::UnaryMinus
                &&  tokens[tokens.size() - OFFSET_TO_ELEMENT_BEFORE].type != TokenType::LeftParentheses){
                    throw std::runtime_error(std::string("Illegal character before number: ") + equation[i]);
                }

            }

            else if(std::isalpha(equation[i])){

                size_t variableStartPosition = i;

                while(i < equation.size() && (std::isalpha(equation[i]) || std::isdigit(equation[i]))){
                    i++;
                }
                tokens.push_back({TokenType::Variable, equation.substr(variableStartPosition, i - variableStartPosition)});

            }

            else if(equation[i] == '+' ||  equation[i] == '*' || equation[i] == '/'){
                tokens.push_back({TokenType::Operator, std::string(1, equation[i])});


                if((tokens.size() < OFFSET_TO_ELEMENT_BEFORE )
                    ||( tokens.size() >= OFFSET_TO_ELEMENT_BEFORE 
                    && tokens[tokens.size() - OFFSET_TO_ELEMENT_BEFORE].type != TokenType::Number
                    && tokens[tokens.size() - OFFSET_TO_ELEMENT_BEFORE].type != TokenType::RightParentheses
                    && tokens[tokens.size() - OFFSET_TO_ELEMENT_BEFORE].type != TokenType::Variable)){
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

        for(size_t i = 0; i<tokens.size(); i++){
            std::cout << tokens[i].tokenContent << std::endl;
        }

        return tokens;
    }

} // namespace notlab
