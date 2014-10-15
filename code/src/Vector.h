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


////////////////////////////////////////////////////////////////////////////////
template <class T>
class vec3
{
public:
    
    vec3(const T x_ = 0, const T y_ = 0, const T z_ = 0) : x(x_), y(y_), z(z_) {};
    
    const T norm() const {
        return sqrt(x * x + y * y + z * z);
    }
    
    const bool operator == (const vec3<T>& v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    
    const bool operator != (const vec3<T>& v) const {
        return !(*this == v);
    }
    
    const vec3<T> operator + (const vec3<T>& v) const {
        return vec3<T>(x + v.x, y + v.y, z + v.z);
    }
    
    template <typename S>
    const vec3<T> operator - (const vec3<S>& v) const {
        return vec3<T>(x - v.x, y - v.y, z - v.z);
    }
    
    template <typename S>
    vec3<T>& operator = (const vec3<S>& v) const 
    {
        x = v.x;
        y = v.y;
        z = v.z;

        return *this;
    }
    
    template <typename S>
    const vec3<T> operator / (const S s) const {
        return vec3<T>(x / s, y / s, z / s);
    }

    template <typename S>
    void operator /= (const S s) {
        x /= s;
        y /= s;
        z /= s;
    }
    
    friend vec3<T> operator * (const T s, const vec3<T>& v) {
        return vec3<T>(s * v.x, s * v.y, s * v.z);
    }
    
    friend vec3<T> operator * (const vec3<T>& v, const T s) {
        return vec3<T>(v.x * s, v.y * s, v.z * s);
    }
    
    friend T dot(const vec3<T>& v1, const vec3<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    
    void operator += (const vec3<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    
    void operator *= (const T s) {
        x *= s;
        y *= s;
        z *= s;
    }
    
    friend ostream& operator << (ostream& out, const vec3<T>& v) {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return out;
    }
    
    T x;
    T y;
    T z;
};


////////////////////////////////////////////////////////////////////////////////
typedef vec3<int> vec3i;
typedef vec3<float> vec3f;


////////////////////////////////////////////////////////////////////////////////
template<>
struct hash<vec3i> {
    size_t operator()(vec3i const & v) const noexcept {
        return 51 * (51 * (51 + std::hash<int>()(v.x)) + std::hash<int>()(v.y)) + std::hash<int>()(v.z);
    }
};


#endif
