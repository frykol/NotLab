#pragma once

#include <memory>
#include <map>
#include <string>
#include <iostream>

namespace notlab
{
    enum class Operator{
        None = 0, Plus, Minus, Multiplies, Divide
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
        float eval(const std::map<std::string, float>& vars) const override{ return vars.at(name);}
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
                return leftValue / rightValue;
            default:
                throw std::runtime_error("Unknow operator");
            }
        }  
    };

} // namespace notlab
