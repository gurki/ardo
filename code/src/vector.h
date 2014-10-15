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
    
        const T abs() const {
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
    
        const vec2<T> operator - (const vec2<T>& v) const {
            return vec2<T>(x - v.x, y - v.y);
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


////////////////////////////////////////////////////////////////////////////////
template<>
struct hash<vec2i> {
    size_t operator()(vec2i const & v) const noexcept {
        return 51 * (51 + std::hash<int>()(v.x)) + std::hash<int>()(v.y);
    }
};


#endif
