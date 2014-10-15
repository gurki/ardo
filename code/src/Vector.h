#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

#include <vector>
#include <math.h>


using namespace std;


////////////////////////////////////////////////////////////////////////////////
template <class T>
class vec2
{
    public:
        
        vec2(const T x_ = 0, const T y_ = 0) : x(x_), y(y_) {};
    
        const T norm() const {
            return sqrt(x * x + y * y);
        }
    
        const bool operator == (const vec2<T>& v) const {
            return x == v.x && y == v.y;
        }

        const bool operator != (const vec2<T>& v) const {
            return !(*this == v);
        }
    
        const vec2<T> operator + (const vec2<T>& v) const {
            return vec2<T>(x + v.x, y + v.y);
        }
    
        template <typename S>
        const vec2<T> operator - (const vec2<S>& v) const {
            return vec2<T>(x - v.x, y - v.y);
        }

        template <typename S>
        vec2<T>& operator = (const vec2<S>& v) const {
            x = v.x;
            y = v.y;
            return *this;
        }
        
        template <typename S>
        const vec2<T> operator / (const S s) const {
            return vec2<T>(x / s, y / s);
        }
    
        friend vec2<T> operator * (const T s, const vec2<T>& v) {
            return vec2<T>(s * v.x, s * v.y);
        }
    
        friend vec2<T> operator * (const vec2<T>& v, const T s) {
            return vec2<T>(v.x * s, v.y * s);
        }

        friend T dot(const vec2<T>& v1, const vec2<T>& v2) {
            return v1.x * v2.x + v1.y * v2.y;
        }
    
        void operator += (const vec2<T>& v) {
            x += v.x;
            y += v.y;
        }
        
        void operator *= (const T s) {
            x *= s;
            y *= s;
        }
    
        friend ostream& operator << (ostream& out, const vec2<T>& v) {
            out << "(" << v.x << ", " << v.y << ")";
            return out;
        }
    
        T x;
        T y;
};


////////////////////////////////////////////////////////////////////////////////
typedef vec2<int> vec2i;
typedef vec2<float> vec2f;



////////////////////////////////////////////////////////////////////////////////
template<>
struct hash<vec2i> {
    size_t operator()(vec2i const & v) const noexcept {
        return 51 * (51 + std::hash<int>()(v.x)) + std::hash<int>()(v.y);
    }
};


#endif
