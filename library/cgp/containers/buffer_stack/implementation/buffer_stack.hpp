#pragma once

#include "cgp/base/base.hpp"
#include <array>
#include <cmath>



namespace cgp
{

    // Implementation of generic size buffer_stack
    //   buffer_stack is a constant size structure (size known at compile time).
    //   Internal data is stored as std::array, and buffer_stack is compatible with std::array syntax.
    template <typename T, int N>
    struct buffer_stack
    {
        /** Internal data is a std::array of values */
        std::array<T, N> data;

        //buffer_stack();
        //buffer_stack(buffer_stack<T,N> const& arg);
        //buffer_stack(std::initializer_list<T> const& arg);

        /** Size of the buffer (N - known at compile time) */
        int size() const;

        /** Fill all data with the given value */
        buffer_stack<T,N>& fill(T const& value);

        /** Element access
         * Allows buffer[i], buffer(i), and buffer.at(i)
         * Bound checking is performed unless cgp_NO_DEBUG is defined. */
        T const& operator[](int index) const;
        T& operator[](int index);

        T const& operator()(int index) const;
        T& operator()(int index);

        T const& at(int index) const;
        T& at(int index);


        /** Iterators
         *  Iterators compatible with STL syntax and std::array */
        typename std::array<T, N>::iterator begin();
        typename std::array<T, N>::iterator end();
        typename std::array<T, N>::const_iterator begin() const;
        typename std::array<T, N>::const_iterator end() const;
        typename std::array<T, N>::const_iterator cbegin() const;
        typename std::array<T, N>::const_iterator cend() const;

        T const& at_unsafe(int index) const    { return data[index]; }
        T & at_unsafe(int index)               { return data[index]; }
    };



    /** Display all elements of the buffer. */
    template <typename T, int N> std::ostream& operator<<(std::ostream& s, buffer_stack<T, N> const& v);

    /** Direct compiled-checked access to data */
    template <int idx, typename T, int N> T const& get(buffer_stack<T,N> const& data);
    template <int idx, typename T, int N> T& get(buffer_stack<T, N>& data);


    /** Convert all elements of the buffer to a string.
     * \param buffer: the input buffer
     * \param separator: the separator between each element */
    template <typename T, int N> std::string str(buffer_stack<T, N> const& v, std::string const& separator = " ", std::string const& begin = "", std::string const& end = "");
    template <typename T, int N> std::string type_str(buffer_stack<T, N> const&);
    template <typename T, int N> int size_in_memory(buffer_stack<T, N> const& v);
    template <typename T, int N> auto const* ptr(buffer_stack<T,N> const& v);

    /** Equality check
     * Check equality (element by element) between two buffers.
     * Buffers with different size are always considered as not equal.
     * Only approximated equality is performed for comprison with float (absolute value between floats) */
    template <typename T1, typename T2, int N1, int N2> bool is_equal(buffer_stack<T1, N1> const& a, buffer_stack<T2, N2> const& b);
    template <typename T, int N> bool is_equal(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b);



    /** Math operators
     * Common mathematical operations between buffers, and scalar or element values. */
    template <typename T, int N> buffer_stack<T, N>  operator-(buffer_stack<T, N> const& a);

    template <typename T, int N> buffer_stack<T, N>& operator+=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>& operator+=(buffer_stack<T, N>& a, T const& b);
    template <typename T, int N> buffer_stack<T, N>  operator+(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>  operator+(buffer_stack<T, N> const& a, T const& b);
    template <typename T, int N> buffer_stack<T, N>  operator+(T const& a, buffer_stack<T, N> const& b);

    template <typename T, int N> buffer_stack<T, N>& operator-=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>& operator-=(buffer_stack<T, N>& a, T const& b);
    template <typename T, int N> buffer_stack<T, N>  operator-(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>  operator-(buffer_stack<T, N> const& a, T const& b);
    template <typename T, int N> buffer_stack<T, N>  operator-(T const& a, buffer_stack<T, N> const& b);

    template <typename T, int N> buffer_stack<T, N>& operator*=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>& operator*=(buffer_stack<T, N>& a, float b);
    template <typename T, int N> buffer_stack<T, N>  operator*(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>  operator*(buffer_stack<T, N> const& a, float b);
    template <typename T, int N> buffer_stack<T, N>  operator*(float a, buffer_stack<T, N> const& b);

    template <typename T, int N> buffer_stack<T, N>& operator/=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>& operator/=(buffer_stack<T, N>& a, float b);
    template <typename T, int N> buffer_stack<T, N>  operator/(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b);
    template <typename T, int N> buffer_stack<T, N>  operator/(buffer_stack<T, N> const& a, float b);
    template <typename T, int N> buffer_stack<T, N>  operator/(float a, buffer_stack<T, N> const& b);


    // Return the unit vector.
    //  Condition: ||v||>0
    template <typename T, int N> buffer_stack<T, N> normalize(buffer_stack<T, N> const& v);
    // Compute the unit vector without error 
    //   if ||v||>0, return v/||v|| 
    //   otherwise return default_zero_norm vector (without error)
    template <typename T, int N> buffer_stack<T, N> normalize(buffer_stack<T, N> const& v, buffer_stack<T, N> const& default_zero_norm);

    // Math operators applied to each component
    template <typename T, int N> T max(buffer_stack<T, N> const& v);
    template <typename T, int N> T min(buffer_stack<T, N> const& v);
    template <typename T, int N> T average(buffer_stack<T, N> const& a);
    template <typename T, int N> T sum(buffer_stack<T, N> const& v);
    template <typename T, int N> buffer_stack<T, N> abs(buffer_stack<T, N> const& a);
    template <typename T, int N> buffer_stack<T, N> clamp(buffer_stack<T, N> const& x, buffer_stack<T, N> const& p_min, buffer_stack<T, N> const& p_max);


}


/***********************************************/
/*           Template implementation           */
/***********************************************/

namespace cgp
{
    //template <typename T, size_t N>
    //buffer_stack<T, N>::buffer_stack()
    //    :data()
    //{}
    //template <typename T, size_t N>
    //buffer_stack<T, N>::buffer_stack(buffer_stack<T, N> const& arg)
    //    : data(arg)
    //{}

    //template <typename T, size_t N>
    //buffer_stack<T, N>::buffer_stack(std::initializer_list<T> const& arg)
    //    :data()
    //{
    //    assert_cgp(arg.size() >= N, "Insufficient initializer_list for buffer_stack");
    //    auto it = arg.begin();
    //    for (size_t k = 0; k < N; ++k)
    //        data[k] = *it;
    //}


    template <typename T, int N> int buffer_stack<T, N>::size() const
    {
        return N;
    }

    template <typename T, int N> buffer_stack<T, N>& buffer_stack<T, N>::fill(T const& value)
    {
        for (int k = 0; k < N; ++k)
            data[k] = value;
        return *this;
    }


    template <typename T, int N, typename INDEX_TYPE>
    void check_index_bounds(INDEX_TYPE index, buffer_stack<T, N> const& data)
    {
#ifndef cgp_NO_DEBUG

        if (index < 0 || index >= INDEX_TYPE(N))
        {
            std::string msg = "\n";
            msg += " - Try access buffer_stack["+str(index)+"]\n";
            msg += " - With container size ="+str(N)+"\n";
            msg += " - Buffer type: " + type_str(data) + "\n";

            if (index < 0)
            {
                msg += "\t> Buffer cannot be access with negative index.\n";
                msg += "\t  Help: \n";
                msg += "\t     Contrary to Python, negative indexing doesn't target the end of the buffer in C++. \n";
            }
            else if (N == 0)
            {
                msg += "\t> The container is empty: Its elements cannot be accessed.\n";
            }
            else if (index == INDEX_TYPE(N))
            {
                msg += "\t> Index reached the maximal size of the buffer \n";
                msg += "\t> The maximal possible index should be " + str(N - 1) + " \n";
                msg += "\t  Help: \n";
                msg += "\t     C++ has index starting at 0. \n";
                msg += "\t     Typical loops should go from index=0 to index<buffer.size(). \n";
            }
            else if (index >= INDEX_TYPE(N))
            {
                msg += "\t> Your index exceeded the size of the container\n";
            }


            msg += "\t  The function and variable that generated this error can be found in analysis the Call Stack.\n\n";

            error_cgp(msg);
        }
#endif
    }

    template <typename T, int N> T const& buffer_stack<T, N>::operator[](int index) const
    {
        check_index_bounds(index, *this);
        return data[index];
    }
    template <typename T, int N> T& buffer_stack<T, N>::operator[](int index)
    {
        check_index_bounds(index, *this);
        return data[index];
    }

    template <typename T, int N> T const& buffer_stack<T, N>::operator()(int index) const
    {
        check_index_bounds(index, *this);
        return data[index];
    }

    template <typename T, int N> T& buffer_stack<T, N>::operator()(int index)
    {
        check_index_bounds(index, *this);
        return data[index];
    }



    template <typename T, int N> T const& buffer_stack<T, N>::at(int index) const
    {
        return data.at(index);
    }

    template <typename T, int N> T& buffer_stack<T, N>::at(int index)
    {
        return data.at(index);
    }

    template <typename T, int N> typename std::array<T, N>::iterator buffer_stack<T, N>::begin()
    {
        return data.begin();
    }
    template <typename T, int N> typename std::array<T, N>::iterator buffer_stack<T, N>::end()
    {
        return data.end();
    }
    template <typename T, int N> typename std::array<T, N>::const_iterator buffer_stack<T, N>::begin() const
    {
        return data.begin();
    }
    template <typename T, int N> typename std::array<T, N>::const_iterator buffer_stack<T, N>::end() const
    {
        return data.end();
    }
    template <typename T, int N> typename std::array<T, N>::const_iterator buffer_stack<T, N>::cbegin() const
    {
        return data.cbegin();
    }
    template <typename T, int N> typename std::array<T, N>::const_iterator buffer_stack<T, N>::cend() const
    {
        return data.cend();
    }


    template <int idx, typename T, int N> T const& get(buffer_stack<T, N> const& data)
    {
        static_assert(idx>=0 && idx < N, "Incorrect element indexing");
        return data.data[idx];
    }
    template <int idx, typename T, int N> T& get(buffer_stack<T, N>& data)
    {
        static_assert(idx>=0 && idx < N, "Incorrect element indexing");
        return data.data[idx];
    }






    template <typename T, int N> std::string type_str(buffer_stack<T, N> const& )
    {
        return "buffer_stack<" + type_str(T()) + "," + str(N) + ">";
    }


    template <typename T, int N> std::ostream& operator<<(std::ostream& s, buffer_stack<T, N> const& v)
    {
        std::string s_out = str(v);
        s << s_out;
        return s;
    }
    template <typename T, int N> std::string str(buffer_stack<T, N> const& v, std::string const& separator, std::string const& begin, std::string const& end)
    {
        return cgp::detail::str_container(v, separator, begin, end);
    }

    template <typename T, int N> int size_in_memory(buffer_stack<T, N> const& v)
    {
        int s = 0;
        for (int k = 0; k < N; ++k)
            s += cgp::size_in_memory(v[k]);
        return s;
    }

    template <typename T1, typename T2, int N1, int N2> bool is_equal(buffer_stack<T1, N1> const& a, buffer_stack<T2, N2> const& b)
    {
        if (N1 != N2)
            return false;
        for (int k = 0; k < N1; ++k)
            if (is_equal(a[k], b[k]) == false)
                return false;
        return true;
    }

    template <typename T, int N> bool is_equal(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b)
    {
        return is_equal<T, T, N, N>(a, b);
    }




    template <typename T, int N> buffer_stack<T, N>  operator-(buffer_stack<T, N> const& a)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = -a[k];
        return res;
    }

    template <typename T, int N> buffer_stack<T, N>& operator+=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b)
    {
        for (int k = 0; k < N; ++k)
            a[k] += b[k];
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>& operator+=(buffer_stack<T, N>& a, T const& b)
    {
        for (int k = 0; k < N; ++k)
            a[k] += b;
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>  operator+(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] + b[k];
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator+(buffer_stack<T, N> const& a, T const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] + b;
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator+(T const& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a + b[k];
        return res;
    }

    template <typename T, int N> buffer_stack<T, N>& operator-=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b)
    {
        for (int k = 0; k < N; ++k)
            a[k] -= b[k];
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>& operator-=(buffer_stack<T, N>& a, T const& b)
    {
        for (int k = 0; k < N; ++k)
            a[k] -= b;
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>  operator-(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] - b[k];
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator-(buffer_stack<T, N> const& a, T const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] - b;
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator-(T const& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a - b[k];
        return res;
    }

    template <typename T, int N> buffer_stack<T, N>& operator*=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b)
    {
        for (int k = 0; k < N; ++k)
            a[k] *= b[k];
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>& operator*=(buffer_stack<T, N>& a, float b)
    {
        for (int k = 0; k < N; ++k)
            a[k] *= b;
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>  operator*(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] * b[k];
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator*(buffer_stack<T, N> const& a, float b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] * b;
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator*(float a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a * b[k];
        return res;
    }

    template <typename T, int N> buffer_stack<T, N>& operator/=(buffer_stack<T, N>& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] / b[k];
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>& operator/=(buffer_stack<T, N>& a, float b)
    {
        for (int k = 0; k < N; ++k)
            a[k] /= b;
        return a;
    }
    template <typename T, int N> buffer_stack<T, N>  operator/(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] / b[k];
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator/(buffer_stack<T, N> const& a, float b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a[k] / b;
        return res;
    }
    template <typename T, int N> buffer_stack<T, N>  operator/(float a, buffer_stack<T, N> const& b)
    {
        buffer_stack<T, N> res;
        for (int k = 0; k < N; ++k)
            res[k] = a / b[k];
        return res;
    }




    template <typename T, int N> T dot(buffer_stack<T, N> const& a, buffer_stack<T, N> const& b)
    {
        T s = {};
        for (int k = 0; k < N; ++k)
            s += a[k] * b[k];
        return s;
    }
    template <typename T, int N> T norm(buffer_stack<T, N> const& a)
    {
        using std::sqrt;
        return sqrt(dot(a, a));
    }

    template <typename T, int N> buffer_stack<T, N> normalize(buffer_stack<T, N> const& v)
    {
        float const n = norm(v);

#ifndef CGP_NO_DEBUG
        if ( norm(v)<1e-5f )
        {
            std::string const s = "Tried to normalize a vector " + type_str(v) + "[" + str(v) + "] with very small norm (" + str(norm(v)) + ")";
            error_cgp(s);
        }
#endif
        
        buffer_stack<T, N> vn = v / n;
        return vn;
    }
    template <typename T, int N> buffer_stack<T, N> normalize(buffer_stack<T, N> const& v, buffer_stack<T, N> const& default_zero_norm)
    {
        float const n = norm(v);
        if (norm(v) < 1e-5f)
            return default_zero_norm;
        return v / n;

    }

    template <typename T, int N> auto const* ptr(buffer_stack<T, N> const& v)
    {
        return ptr(v[0]);
    }


    template <typename T, int N> T max(buffer_stack<T, N> const& v)
    {
        static_assert(N > 0, "Cannot get max on empty buffer_stack");

        T const& current_max = get<0>(v);
        for (int k = 1; k < N; ++k) {
            T const& element = v.at_unsafe(k);
            if (element > current_max)
                current_max = element;
        }

        return current_max;
    }
    template <typename T, int N> T min(buffer_stack<T, N> const& v)
    {
        static_assert(N > 0, "Cannot get min on empty buffer_stack");

        T const& current_min = get<0>(v);
        for (int k = 1; k < N; ++k) {
            T const& element = v.at_unsafe(k);
            if (element < current_min)
                current_min = element;
        }

        return current_min;
    }
    template <typename T, int N> T average(buffer_stack<T, N> const& a)
    {
        static_assert(N > 0, "Cannot get average on empty buffer_stack");
        return sum(a)/static_cast<float>(N);
    }
    template <typename T, int N> T sum(buffer_stack<T, N> const& v)
    {
        T element; // assume element is initialized at 0
        for (int k = 0; k < N; ++k)
            element += v.at_unsafe(k);
        return element;
    }
    template <typename T, int N> buffer_stack<T, N> abs(buffer_stack<T, N> const& a)
    {
        buffer_stack<T, N> element;
        for (int k = 0; k < N; ++k)
            element[k] = abs(a[k]);
        return element;
    }
    template <typename T, int N> buffer_stack<T, N> clamp(buffer_stack<T, N> const& x, buffer_stack<T, N> const& p_min, buffer_stack<T, N> const& p_max)
    {
        buffer_stack<T, N> element;
        for (int k = 0; k < N; ++k)
            element[k] = clamp(x[k], p_min[k], p_max[k]);
        return element;
    }
}
