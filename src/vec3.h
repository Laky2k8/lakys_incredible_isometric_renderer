#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 
{
	public:
		float x, y, z; // Position

		vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

		// Getters
		float getX() const { return x; }
		float getY() const { return y; }
		float getZ() const { return z; }

		// Setters
		void setX(float x) { this->x = x; }
		void setY(float y) { this->y = y; }
		void setZ(float z) { this->z = z; }

		vec3 operator-() const { return vec3(-x, -y, -z); }

		vec3& operator+=(const vec3& v) 
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		vec3& operator*=(double t) 
		{
			x *= t;
			y *= t;
			z *= t;
			return *this;
		}

		vec3& operator/=(double t) 
		{
			return *this *= 1/t;
		}

		// Length/magnitude of vector
		float length() const 
		{
			return sqrt(x*x + y*y + z*z);
		}

		// Squared length
		float length_squared() const 
		{
			return x*x + y*y + z*z;
		}

		vec3& normalize() 
		{
			float len = length();
			if (len > 0) {
				*this /= len;
			}
			return *this;
		}

		float dot(const vec3& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.x, t*v.y, t*v.z);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}


#endif