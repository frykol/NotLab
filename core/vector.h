#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace notlab
{
    template<typename T>
    class Vector{
        private:
            std::vector<T> m_data;

        public:
            Vector(std::initializer_list<T> list): m_data(list) {}
            explicit Vector(size_t cap): m_data(cap) {}

            void operator=(std::initializer_list<T> list){
                m_data = list;
                
            }
            
            size_t getSize() const {return m_data.size();}

            T& operator[](size_t i) {
                if (i + 1 > m_data.size()){
                    throw std::runtime_error("Index out of bounds");
                }
                return m_data[i];
            }

            const T& operator[](size_t i) const {
                if (i + 1 > m_data.size()){
                    throw std::runtime_error("Index out of bounds");
                }
                return m_data[i];
            }

            // Vector<T> operator+(const Vector<T>& second) const{
            //     if( this->getSize() != second.getSize()){
            //         throw std::runtime_error("Length of vectors don't match");
            //     }
            //     Vector<T> resoult(getSize());
            //     for(size_t i = 0; i<getSize(); i++){
            //         resoult[i] = m_data[i] + second[i];
            //     }
            //     return resoult;

            // }


            std::string toString(){
                std::stringstream ss;
                ss << "[ "; 
                size_t i;
                for(i = 0; i< getSize() - 1; i++){
                    ss << m_data[i];
                    ss << ", ";
                }
                ss << m_data[i];
                ss << " ]";
                return ss.str();
            }
    };

    using VectorI = Vector<int>;
    using VectorF = Vector<float>;

    template<typename T, typename U, typename Op>
    auto elementwise(const Vector<T>& left, const Vector<U>& right, Op operation){
        if(left.getSize() != right.getSize()){
            throw std::runtime_error("Length of vectors don't match");
        }
        using resultType = decltype(operation(T() , U()));
        Vector<resultType> result(left.getSize());
        for(int i = 0; i<left.getSize(); i++){
            result[i] = operation(static_cast<resultType>(left[i]), static_cast<resultType>(right[i]));
        }
        return result;
    }


    template<typename T, typename U>
    auto operator+(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::plus<>());}
    template<typename T, typename U>
    auto operator-(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::minus<>());}
    template<typename T, typename U>
    auto operator*(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::multiplies<>());}

    template<typename T, typename U>
    auto operator*(const Vector<T>& left, const U& scalar) {
        using resultType = decltype(T() + U());
        Vector<resultType> result(left.getSize());
        for(int i = 0; i<left.getSize(); i++){
            result[i] = left[i] * scalar;
        }
        return result;
    }

    template<typename T, typename U>
    auto operator*(const T& scalar, const Vector<U>& right) {
        using resultType = decltype(T() + U());
        Vector<resultType> result(right.getSize());
        for(int i = 0; i<right.getSize(); i++){
            result[i] = right[i] * scalar;
        }
        return result;
    }


    template<typename T, typename U>
    auto operator/(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::divides<>());}

    template<typename T, typename U>
    auto operator/(const Vector<T>& left, const U& scalar) {
        using resultType = decltype(T() + U());
        Vector<resultType> result(left.getSize());
        for(int i = 0; i<left.getSize(); i++){
            result[i] = left[i] / scalar;
        }
        return result;
    }

}
