#pragma once

#include <memory>
#include <vector>
#include "ast.h"
#include "tokenizer.h"

namespace notlab{

    int getOperatorPrecedence(const Token& operatorToken){
        if(operatorToken.tokenContent == "^"){
            return 4;
        }
        if(operatorToken.type == TokenType::UnaryMinus){
            return 3;
        }
        else if(operatorToken.tokenContent == "*" || operatorToken.tokenContent == "/"){
            return 2;
        }
        if(operatorToken.tokenContent == "+" || operatorToken.tokenContent == "-"){
            return 1;
        }
        return 0;
    }

    Operator getOperatorFromString(const std::string& op){
        if(op == "+"){
            return Operator::Plus;
        }
        else if(op == "-"){
            return Operator::Minus;
        }
        else if(op == "*"){
            return Operator::Multiplies;
        }
        else if(op == "/"){
            return Operator::Divide;
        }
        else if(op == "^"){
            return Operator::Power;
        }
        else{
            throw std::runtime_error("Unknow operator when parsing equation");
        }
    }

    std::unique_ptr<Expression> parseTokens(const std::vector<Token>& tokens){
        std::vector<Token> operatorStack;
        std::vector<std::unique_ptr<Expression>> operandStack;

        for(size_t i = 0; i<tokens.size(); i++){
            const Token& token = tokens[i];
            if(token.type == TokenType::Number){
                operandStack.push_back(std::make_unique<Constant>(std::stof(token.tokenContent)));
            }
            else if(token.type == TokenType::Variable){
                operandStack.push_back(std::make_unique<Variable>(token.tokenContent));
            }
            else if(token.type == TokenType::Operator || token.type == TokenType::UnaryMinus){
                while(!operatorStack.empty()
                && getOperatorPrecedence(token) < getOperatorPrecedence(operatorStack.back())){

                    Token op = operatorStack.back();
                    operatorStack.pop_back();

                    if(op.type == TokenType::UnaryMinus){
                        auto unaryArgument = std::move(operandStack.back());
                        operandStack.pop_back();
                        operandStack.push_back(std::make_unique<UnaryOperator>(Operator::Minus, std::move(unaryArgument)));
                    }
                    else{
                        auto right = std::move(operandStack.back());
                        operandStack.pop_back();
                        auto left = std::move(operandStack.back());
                        operandStack.pop_back();
                        Operator binaryOp = getOperatorFromString(op.tokenContent);
                        operandStack.push_back(std::make_unique<BinaryOperator>(binaryOp, std::move(left), std::move(right)));
                    }

                }
                operatorStack.push_back(token);
            }
            else if(token.type == TokenType::LeftParentheses){
                operatorStack.push_back(token);
            }
            else if(token.type == TokenType::RightParentheses){
                while(operatorStack.back().type != TokenType::LeftParentheses){
                    Token op = operatorStack.back();
                    operatorStack.pop_back();

                    if(op.type == TokenType::UnaryMinus){
                        auto unaryArgument = std::move(operandStack.back());
                        operandStack.pop_back();
                        operandStack.push_back(std::make_unique<UnaryOperator>(Operator::Minus, std::move(unaryArgument)));
                    }
                    else{
                        auto right = std::move(operandStack.back());
                        operandStack.pop_back();
                        auto left = std::move(operandStack.back());
                        operandStack.pop_back();
                        Operator binaryOp = getOperatorFromString(op.tokenContent);
                        operandStack.push_back(std::make_unique<BinaryOperator>(binaryOp, std::move(left), std::move(right)));
                    }
                }
                operatorStack.pop_back();
            }
        }

        while(!operatorStack.empty()){
            Token op = operatorStack.back();
                    operatorStack.pop_back();

                    if(op.type == TokenType::UnaryMinus){
                        auto unaryArgument = std::move(operandStack.back());
                        operandStack.pop_back();
                        operandStack.push_back(std::make_unique<UnaryOperator>(Operator::Minus, std::move(unaryArgument)));
                    }
                    else{
                        auto right = std::move(operandStack.back());
                        operandStack.pop_back();
                        auto left = std::move(operandStack.back());
                        operandStack.pop_back();
                        Operator binaryOp = getOperatorFromString(op.tokenContent);
                        operandStack.push_back(std::make_unique<BinaryOperator>(binaryOp, std::move(left), std::move(right)));
                    }
        }


        if(operandStack.size() != 1){
            throw std::runtime_error("Operand stack bad size");
        }
        return std::move(operandStack.back());
    }
}