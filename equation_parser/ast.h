#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <cmath>
#include <variant>

namespace notlab
{
    enum class Operator{
        None = 0, Plus, Minus, Multiplies, Divide, Power
    };

    struct Expression{
        virtual ~Expression() {}
        virtual float eval(const std::map<std::string, float>& vars) const = 0;
    };

    struct Constant : Expression{
        float value;
        Constant(float v): value(v) {}
        float eval(const std::map<std::string, float>& vars) const override {return value; }
    };

    struct Variable: Expression{
        std::string name;
        Variable(const std::string& n): name(n) {}
        float eval(const std::map<std::string, float>& vars) const override{
            auto it = vars.find(name);
            if(it == vars.end()){
                throw std::runtime_error("Variable not found");
            }
            return it->second;
        }
    };

    struct Function : Expression{
        std::string name;
        std::vector<std::unique_ptr<Expression>> arguments;

        Function(const std::string& n, std::vector<std::unique_ptr<Expression>> arg): name(n), arguments(std::move(arg)){}

        float eval(const std::map<std::string, float>& vars) const override{ 
            std::vector<float> argumentsValue;
            argumentsValue.reserve(arguments.size());
            for(auto& arg: arguments){
                argumentsValue.push_back(arg->eval(vars));
            }

            if(name == "sin"){
                if(argumentsValue.size() != 1){
                    throw std::runtime_error("sin: number of arguments is different than one");
                }
                return std::sin(argumentsValue[0]);
            }
            if(name == "max"){
                if(argumentsValue.size() != 2){
                    throw std::runtime_error("max: number of arguments is different than 2");
                }
                return std::max(argumentsValue[0], argumentsValue[1]);
            }

            throw std::runtime_error("Unknow function: " + name);
        }
    };

    struct UnaryOperator : Expression{
        Operator op;
        std::unique_ptr<Expression> expression;

        UnaryOperator(Operator o, std::unique_ptr<Expression> expr): op(o), expression(std::move(expr)){}

        float eval(const std::map<std::string, float>& vars) const override{ 
            float expressionValue = expression->eval(vars);
            switch (op)
            {
                case Operator::Minus:
                    return -1 * expressionValue;
                default:
                    throw std::runtime_error("Unknow operator");
            }
        }
    };

    struct BinaryOperator : Expression{
        Operator op;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;

        BinaryOperator(Operator o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        :op(o), left(std::move(l)), right(std::move(r)){}

        float eval(const std::map<std::string, float>& vars) const override{
            float leftValue = left->eval(vars);
            float rightValue = right->eval(vars);
            switch (op)
            {
                case Operator::Plus:
                    return leftValue + rightValue;
                case Operator::Minus:
                    return leftValue - rightValue;
                case Operator::Multiplies:
                    return leftValue * rightValue;
                case Operator::Divide:
                    if(std::abs(rightValue) < 1e-8){
                        throw std::runtime_error("Can't divided by zero");
                    }
                    return leftValue / rightValue;
                case Operator::Power:
                    return std::pow(leftValue, rightValue);
                default:
                    throw std::runtime_error("Unknow operator");
            }
        }  
    };

} // namespace notlab
