#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace notlab
{
    /**
     * @struct is_vector_type_valid
     * @brief Type trait to restrict allowed types for Vector.
     * @tparam T Type to check.
     */
    template<typename T>
    struct is_vector_type_valid : std::false_type {};

    /// Specialization: int is allowed.
    template<> struct is_vector_type_valid<int>: std::true_type{};
    /// Specialization: float is allowed.
    template<> struct is_vector_type_valid<float>: std::true_type{};
    /// Specialization: double is allowed.
    template<> struct is_vector_type_valid<double>: std::true_type{};

    /** 
     * @class Vector
     * @tparam T Element type (e.g. int, float, double).
     * @brief Dynamic mathematical vector with elementwise operations and factories.
     * @description
     * The Vector class enables creation of dynamic numeric vectors, performing elementwise operations,
     * appending/prepending vectors, and string conversion.
     * Only int, float, or double are allowed as element types.
     */
    template<typename T>
    class Vector{
        static_assert(is_vector_type_valid<T>::value, "This type is not valid");
        private:
            /// Internal data storage.
            std::vector<T> m_data;

            /**
             * @brief Private constructor from initializer list.
             * @param list Elements to initialize.
             */
            Vector(std::initializer_list<T> list): m_data(list) {}

            /**
             * @brief Private constructor from size (default-initialized elements).
             * @param cap Vector size.
             */
            explicit Vector(size_t cap): m_data(cap) {}

        public:

            /**
             * @brief Create a new vector of zeros.
             * @param n Size of the new vector.
             * @return Vector of length n filled with zeros.
             */
            static Vector<T> zeros(size_t n){
                Vector<T> v(n);
                return v;
            }

            /**
             * @brief Create a vector from a list of values.
             * @param list List of values.
             * @return Vector containing the given values.
             */
            static Vector<T> fromList(std::initializer_list<T> list){
                return Vector<T>(list);
            }

            /**
             * @brief Add an element to the back of the vector.
             * @param element Value to append.
             */
            void addBack(const T& element){
                m_data.push_back(element);
            }

            /**
             * @brief Append another vector to the back.
             * @param vector Other vector to append.
             */
            void addBack(const Vector<T>& vector){
                m_data.insert(m_data.end(), vector.m_data.begin(), vector.m_data.end());
            }

            /**
             * @brief Append another vector subset to the back.
             * 
             * @param vector Other vector to append
             * @param from First index in subset
             * @param to Last index in subset
             */
            void addBack(const Vector<T>& vector, size_t from, size_t to){
                m_data.insert(m_data.begin(), vector.getData().begin() + from,vector.getData().begin() + to);
            }

            void addBack(const std::vector<T>& vector, size_t from, size_t to){
                m_data.insert(m_data.begin(), vector.begin() + from,vector.begin() + to);
            }

            /**
             * @brief Add an element to the front of the vector.
             * @param element Value to prepend.
             */
            void addFront(const T& element){
                m_data.insert(m_data.begin(), element);
            }

            /**
             * @brief Prepend another vector to the front.
             * @param vector Other vector to prepend.
             */
            void addFront(const Vector<T>& vector){
                m_data.insert(m_data.begin(), vector.m_data.begin(), vector.m_data.end());
            }

            void addFront(const Vector<T>& vector, size_t from, size_t to){
                m_data.insert(m_data.end(), vector.m_data.begin() + from, vector.m_data.begin() + to);
            }

            /**
             * @brief Get the Data object
             * 
             * @return const std::vector<T>& 
             */
            const std::vector<T>& getData() const{
                return m_data;
            }

            /**
             * @brief Get the number of elements in the vector.
             * @return Vector size.
             */
            size_t getSize() const {return m_data.size();}

            /**
             * @brief Assigns new elements from an initializer list (overwrites old contents).
             * @param list New elements.
             */
            void operator=(std::initializer_list<T> list){
                m_data.clear(); // instead of erase
                m_data = list;
            }

            /**
             * @brief Access the i-th element (read/write).
             * @param i Index (starting from 1).
             * @throws std::runtime_error if index is out of bounds.
             * @return Reference to the element.
             */
            T& operator()(size_t i){
                if (i > m_data.size() || i<1){
                    throw std::runtime_error("Index out of bounds");
                }
                return m_data[i - 1];
            }

            /**
             * @brief Access the i-th element (read-only).
             * @param i Index (starting from 0).
             * @throws std::runtime_error if index is out of bounds.
             * @return Const reference to the element.
             */
            const T& operator()(size_t i) const{
                if (i >= m_data.size()){
                    throw std::runtime_error("Index out of bounds");
                }
                return m_data[i];
            }

            /**
             * @brief Convert the vector to a readable string.
             * @return String representation of the vector.
             */
            std::string toString(){
                std::stringstream ss;
                ss << "[ "; 
                size_t i;
                for(i = 0; i< getSize(); i++){
                    if(i>0) ss << ", ";
                    ss << m_data[i];
                }
                ss << " ]";
                return ss.str();
            }
    };

    /** @typedef VectorI Vector of integers (int) */
    using VectorI = Vector<int>;
    /** @typedef VectorF Vector of floating-point values (float) */
    using VectorF = Vector<float>;
    /** @typedef VectorD Vector of double-precision values (double) */
    using VectorD = Vector<double>;

    /**
     * @brief Performs an elementwise operation on two vectors using the provided functor.
     * @tparam T Type of the first vector.
     * @tparam U Type of the second vector.
     * @tparam Op Type of operation (e.g. std::plus).
     * @param left First vector.
     * @param right Second vector.
     * @param operation Functor/lambda with the operation.
     * @throws std::runtime_error if vector sizes do not match.
     * @return Resulting vector.
     */
    template<typename T, typename U, typename Op>
    auto elementwise(const Vector<T>& left, const Vector<U>& right, Op operation){
        if(left.getSize() != right.getSize()){
            throw std::runtime_error("Length of vectors don't match");
        }
        using resultType = decltype(operation(T() , U()));
        Vector<resultType> result(left.getSize());
        for(size_t i = 0; i<left.getSize(); i++){
            result[i] = operation(static_cast<resultType>(left[i]), static_cast<resultType>(right[i]));
        }
        return result;
    }

    /**
     * @brief Adds two vectors elementwise.
     * @tparam T Type of the first vector.
     * @tparam U Type of the second vector.
     * @param left First vector.
     * @param right Second vector.
     * @return Vector of the elementwise sum.
     */
    template<typename T, typename U>
    auto operator+(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::plus<>());}
    /// Analogously for operator- (subtraction), operator* (multiplication), and operator/ (division).

    /**
     * @brief Subtrats two vectors elementwise.
     * @tparam T Type of the first vector.
     * @tparam U Type of the second vector.
     * @param left First vector.
     * @param right Second vector.
     * @return Vector of the elementwise subtraction.
     */
    template<typename T, typename U>
    auto operator-(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::minus<>());}

    /**
     * @brief Multiplies a vector by a scalar (vector * scalar).
     */
    template<typename T, typename U>
    auto operator*(const Vector<T>& left, const U& scalar) {
        using resultType = decltype(T() + U());
        Vector<resultType> result(left.getSize());
        for(size_t i = 0; i<left.getSize(); i++){
            result[i] = left[i] * scalar;
        }
        return result;
    }

    /**
     * @brief Multiplies a scalar by a vector (scalar * vector).
     */
    template<typename T, typename U>
    auto operator*(const T& scalar, const Vector<U>& right) {
        using resultType = decltype(T() + U());
        Vector<resultType> result(right.getSize());
        for(size_t i = 0; i<right.getSize(); i++){
            result[i] = right[i] * scalar;
        }
        return result;
    }

    /**
     * @brief Divides two vectors elementwise.
     */
    template<typename T, typename U>
    auto operator/(const Vector<T>& left, const Vector<U>& right) {return elementwise(left, right, std::divides<>());}

    /**
     * @brief Divides a vector by a scalar.
     */
    template<typename T, typename U>
    auto operator/(const Vector<T>& left, const U& scalar) {
        using resultType = decltype(T() + U());
        Vector<resultType> result(left.getSize());
        for(size_t i = 0; i<left.getSize(); i++){
            result[i] = left[i] / scalar;
        }
        return result;
    }

    /**
     * @brief Compares two vectors for elementwise equality.
     * @tparam T Type of the first vector.
     * @tparam U Type of the second vector.
     * @param first First vector.
     * @param second Second vector.
     * @return true if all elements are equal, false otherwise.
     */
    template<typename T, typename U>
    static bool isVectorEqual(const Vector<T>& first, const Vector<U>& second){
        if(first.getSize() != second.getSize()){
            return false;
        }
        using resultType = decltype(T() + U());
        size_t len = first.getSize();
        for(size_t i = 0; i<len; i++){
            resultType firstElement = static_cast<resultType>(first(i));
            resultType secondElement = static_cast<resultType>(second(i));
            if(firstElement != secondElement){
                return false;
            }
        }
        return true;
    }

} // namespace notlab