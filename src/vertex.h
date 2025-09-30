#ifndef VERTEX_H
#define VERTEX_H

#include "vec3.h"
using namespace std;
#define M_PI 3.14159265358979323846

// Distance between two vectors
float dist(float x1, float y1, float x2, float y2) {
	return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

// Angle between two vectors
float abtw(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return atan2(dy, dx);
}

class Vertex : public vec3 
{
	public:

		Vertex() : vec3() {}
		Vertex(float x, float y, float z) : vec3(x, y, z) {}
		
		// Copy constructor
		Vertex(const vec3& v) : vec3(v) {}

		vec3 getProjection(vec3 cameraPos, vec3 cameraAngle, float centerX, float centerY, float zoom) const
		{

			float posX = getX() - cameraPos.getX();
			float posY = getY() - cameraPos.getY();
			float posZ = getZ() - cameraPos.getZ();
			
			// Convert angles from degrees to radians
			float rotX = cameraAngle.getX() * M_PI / 180.0f;
			float rotY = cameraAngle.getY() * M_PI / 180.0f;
			float rotZ = cameraAngle.getZ() * M_PI / 180.0f;
			
			// Rotation around X-axis (pitch - looking up/down)
			float tempY = posY * cos(rotX) - posZ * sin(rotX);
			float tempZ = posY * sin(rotX) + posZ * cos(rotX);
			posY = tempY;
			posZ = tempZ;
			
			// Rotation around Y-axis (yaw - looking left/right)
			float tempX = posX * cos(rotY) + posZ * sin(rotY);
			tempZ = -posX * sin(rotY) + posZ * cos(rotY);
			posX = tempX;
			posZ = tempZ;
			
			// Rotation around Z-axis (roll - tilting head)
			tempX = posX * cos(rotZ) - posY * sin(rotZ);
			tempY = posX * sin(rotZ) + posY * cos(rotZ);
			posX = tempX;
			posY = tempY;
					
			// Isometric projection (orthographic projection)
			float screenX = centerX + posX * zoom;
			float screenY = centerY - posY * zoom; // Negative because screen Y increases downward
			
			return vec3(screenX, screenY, posZ); // Return the projected position in 2D space
		}
};

vec3 cameraSpace_old(const vec3& worldPt, const vec3& worldPos, const vec3& worldRot) {
    // translate into camera origin
    vec3 p = worldPt - worldPos;

    float rotX = worldRot.getX() * M_PI / 180.0f;
    float rotY = worldRot.getY() * M_PI / 180.0f;
    float rotZ = worldRot.getZ() * M_PI / 180.0f;

    // Rotate around X-axis
    float cosX = cos(rotX), sinX = sin(rotX); 
    p = vec3(p.x,
             p.y * cosX - p.z * sinX,
             p.y * sinX + p.z * cosX);

    // Rotate around Y-axis
    float cosY = cos(rotY), sinY = sin(rotY); 
    p = vec3(p.x * cosY + p.z * sinY,
             p.y,
            -p.x * sinY + p.z * cosY);

    // Z rotation
    float cosZ = cos(rotZ), sinZ = sin(rotZ);
    p = vec3(p.x * cosZ - p.y * sinZ,
             p.x * sinZ + p.y * cosZ,
             p.z);

    return p;
}

vec3 cameraSpace(const vec3& worldPt, const vec3& cameraPos, const vec3& cameraAngle) 
{
    float posX = worldPt.getX() - cameraPos.getX();
    float posY = worldPt.getY() - cameraPos.getY();
    float posZ = worldPt.getZ() - cameraPos.getZ();
    
	
    float rotX = cameraAngle.getX() * M_PI / 180.0f;
    float rotY = cameraAngle.getY() * M_PI / 180.0f;
    float rotZ = cameraAngle.getZ() * M_PI / 180.0f;
    
    // Rotation around X-axis (pitch - looking up/down)
    float tempY = posY * cos(rotX) - posZ * sin(rotX);
    float tempZ = posY * sin(rotX) + posZ * cos(rotX);
    posY = tempY;
    posZ = tempZ;
    
    // Rotation around Y-axis (yaw - looking left/right)
    float tempX = posX * cos(rotY) + posZ * sin(rotY);
    tempZ = -posX * sin(rotY) + posZ * cos(rotY);
    posX = tempX;
    posZ = tempZ;
    
    // Rotation around Z-axis (roll - tilting head)
    tempX = posX * cos(rotZ) - posY * sin(rotZ);
    tempY = posX * sin(rotZ) + posY * cos(rotZ);
    posX = tempX;
    posY = tempY;
    
    return vec3(posX, posY, posZ);
}

vec3 calculateFaceNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3) 
{
    vec3 edge1(v2.getX() - v1.getX(), v2.getY() - v1.getY(), v2.getZ() - v1.getZ());
    vec3 edge2(v3.getX() - v1.getX(), v3.getY() - v1.getY(), v3.getZ() - v1.getZ());
    
    // Cross product for normal
    vec3 normal(
        edge1.getY() * edge2.getZ() - edge1.getZ() * edge2.getY(),
        edge1.getZ() * edge2.getX() - edge1.getX() * edge2.getZ(),
        edge1.getX() * edge2.getY() - edge1.getY() * edge2.getX()
    );
    
    return normal;
}

bool backFacing(const Vertex& p0, const Vertex& p1, const Vertex& p2)
{
    // Calculate 2D cross product of (p1-p0) x (p2-p0)
    float cross = (p1.getX() - p0.getX()) * (p2.getY() - p0.getY()) - 
                  (p1.getY() - p0.getY()) * (p2.getX() - p0.getX());
    
    return cross > 0; // Positive cross product means face is facing away
}

// Get barycentric coordinates of a triangle
vec3 getBarycentric(vec3 vert1, vec3 vert2, vec3 vert3, vec3 point)
{
    auto v0 = vert2 - vert1;
    auto v1 = vert3 - vert1;
    auto v2 = point - vert1;

    auto d00 = v0.dot(v0);
    auto d01 = v0.dot(v1);
    auto d11 = v1.dot(v1);
    auto d20 = v2.dot(v0);
    auto d21 = v2.dot(v1);
    auto det = (d00 * d11 - d01 * d01);

    auto v = (d11 * d20 - d01 * d21) / det;
    auto w = (d00 * d21 - d01 * d20) / det;
    auto u = 1.0f - v - w;

    return vec3(u, v, w);
}

#endif