#pragma once

#include <algorithm>
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

    static void pushToOperand(Token& op, std::vector<Token>& operatorStack, std::vector<std::unique_ptr<Expression>>& operandStack){
            if(op.type == TokenType::UnaryMinus){

                if(operandStack.empty()){
                    throw std::runtime_error("Unary Minus: missing operand");
                }

                auto unaryArgument = std::move(operandStack.back());
                operandStack.pop_back();
                operandStack.push_back(std::make_unique<UnaryOperator>(Operator::Minus, std::move(unaryArgument)));
            }
            else{

                if(operandStack.size() < 2){
                    throw std::runtime_error("Binary operator: missing operands");
                }

                auto right = std::move(operandStack.back());
                operandStack.pop_back();
                auto left = std::move(operandStack.back());
                operandStack.pop_back();
                Operator binaryOp = getOperatorFromString(op.tokenContent);
                operandStack.push_back(std::make_unique<BinaryOperator>(binaryOp, std::move(left), std::move(right)));
            }
    } 

    std::unique_ptr<Expression> parseTokens(const std::vector<Token>& tokens){
        std::vector<Token> operatorStack;
        std::vector<std::unique_ptr<Expression>> operandStack;
        std::vector<int> functionArgumentsCountStack;

        for(size_t i = 0; i<tokens.size(); i++){

            const Token& token = tokens[i];
            const Token* prevToken = (i > 0) ? &tokens[i-1] : nullptr;

            std::cout << token.tokenContent << std::endl;
            if(token.type == TokenType::Number){
                operandStack.push_back(std::make_unique<Constant>(std::stof(token.tokenContent)));
            }
            else if(token.type == TokenType::Variable){
                operandStack.push_back(std::make_unique<Variable>(token.tokenContent));
            }
            else if(token.type == TokenType::Operator || token.type == TokenType::UnaryMinus){

                int precendenceOfToken = getOperatorPrecedence(token);
                bool isRightAssoc = (token.tokenContent == "^");

                while(!operatorStack.empty()){
                    
                    int precendenceOfTop = getOperatorPrecedence(operatorStack.back());

                    if(precendenceOfToken < precendenceOfTop || (precendenceOfToken == precendenceOfTop && !isRightAssoc)){
                        Token op = operatorStack.back();
                        operatorStack.pop_back();
                    
                        pushToOperand(op, operatorStack, operandStack);
                    }
                    else{
                        break;
                    }
                }
                operatorStack.push_back(token);
            }
            else if(token.type == TokenType::Function){
                operatorStack.push_back(token);
            }
            else if(token.type == TokenType::Comma){
                if(functionArgumentsCountStack.empty()){
                    throw std::runtime_error("Invalid use of token ','");
                }

                while(!operatorStack.empty() && operatorStack.back().type != TokenType::LeftParentheses){
                    Token op = operatorStack.back();
                    operatorStack.pop_back();


                    pushToOperand(op, operatorStack, operandStack);
                }
                if(operatorStack.empty()){
                    throw std::runtime_error("Comma without maching '('");
                }
                functionArgumentsCountStack.back()++;
            }
            else if(!operatorStack.empty() && token.type == TokenType::LeftParentheses){
                if(operatorStack.back().type == TokenType::Function){
                    functionArgumentsCountStack.push_back(0);
                }
                operatorStack.push_back(token);
            }
            else if(token.type == TokenType::RightParentheses){
                while(operatorStack.back().type != TokenType::LeftParentheses){
                    Token op = operatorStack.back();
                    operatorStack.pop_back();

                    pushToOperand(op, operatorStack, operandStack);

                    
                }
                operatorStack.pop_back();


                if(operatorStack.back().type == TokenType::Function){
                    Token function = operatorStack.back();
                    operatorStack.pop_back();

                    if(prevToken && prevToken->type != TokenType::LeftParentheses && prevToken->type != TokenType::Comma){
                        functionArgumentsCountStack.back()++;
                        std::cout << functionArgumentsCountStack.back() << std::endl;
                    }

                    std::vector<std::unique_ptr<Expression>> arguments;

                    for(int i = 0; i < functionArgumentsCountStack.back(); i++){
                        arguments.push_back(std::move(operandStack.back()));
                        operandStack.pop_back();
                    }

                    std::reverse(arguments.begin(), arguments.end());
                    operandStack.push_back(std::make_unique<Function>(function.tokenContent, std::move(arguments))); 

                    functionArgumentsCountStack.pop_back();
                }
            }
        }

        while(!operatorStack.empty()){
            Token op = operatorStack.back();
                    operatorStack.pop_back();

                    pushToOperand(op, operatorStack, operandStack);
        }


        if(operandStack.size() != 1){
            throw std::runtime_error("Operand stack bad size");
        }
        return std::move(operandStack.back());
    }
}