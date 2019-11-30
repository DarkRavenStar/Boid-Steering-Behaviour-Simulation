#include <string>
#include "Vector3.h"

Vector3::Vector3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector3::Vector3(float X, float Y, float Z)
{
    x = X;
    y = Y;
    z = Z;
}

void Vector3::SetVector3(float X, float Y, float Z)
{
    x = X;
    y = Y;
    z = Z;
}

Vector3 Vector3::operator+(const Vector3& other)
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other)
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float num)
{
    return Vector3(x * num, y * num, z * num);
}

Vector3 Vector3::operator/(float num)
{
    return Vector3(x / num, y / num, z / num);
}

//! Addition assignment operator.
Vector3& Vector3::operator+=(const Vector3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

//! Subtract assignment operator.
Vector3& Vector3::operator-=(const Vector3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

//! Multiply assignment operator.
Vector3& Vector3::operator*=(float val)
{
    x *= val;
    y *= val;
    z *= val;
    return *this;
}

//! Divide assignment operator.
Vector3& Vector3::operator/=(float val)
{
    x /= val;
    y /= val;
    z /= val;
    return *this;
}

bool Vector3::operator!=(const Vector3& other)
{
    if(x != other.x && y != other.y && z != other.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Vector3 Vector3::GetCrossProduct(const Vector3& other)
{
    float tempX = (y * other.z) - (z * other.y);
    float tempY = (z * other.x) - (x * other.z);
    float tempZ = (x * other.y) - (y * other.x);
    return Vector3(tempX, tempY, tempZ);
}

//! Get length/magnitude
float Vector3::GetMagnitude() //It is also length
{
    return float(sqrt(x*x + y*y + z*z));
}

//! Get squared length/magnitude
float Vector3::GetSquaredMagnitude()
{
    return (x * x + y * y + z * z);
}

float Vector3::GetDotProduct(const Vector3& other)
{
    return float (x*other.x + y*other.y + z*other.z);
}

Vector3 Vector3::GetNormalization()
{
    float magnitude = this->GetMagnitude();
    float tempX;
    float tempY;
    float tempZ;
    if (magnitude > 0)
    {
        tempX = x/magnitude;
        tempY = y/magnitude;
        tempZ = z/magnitude;
    }
    else
    {
        tempX = 0;
        tempY = 0;
        tempZ = 0;
    }
    return Vector3(tempX, tempY, tempZ);
}

void Vector3::SetLimit(float limit)
{
    float limitSize = GetMagnitude();

    if (limitSize > limit)
    {
        x /= limitSize;
        y /= limitSize;
        z /= limitSize;
    }
}

//! Normalize the entire vector instead of getting a normalized copy
void Vector3::Normalize()
{
    float l = GetMagnitude();
    if (l > 0)
    {
        float invL = 1.0f / l;
        x *= invL;
        y *= invL;
        z *= invL;
    }
}

void Vector3::SetMagnitude(float val)
{
    Normalize();
    x *= val;
    y *= val;
    z *= val;
}

float Vector3::GetAngleBetween(Vector3 v)
{
    if (x == 0 && y == 0) return 0.0f;
    if (v.x == 0 && v.y == 0) return 0.0f;

    double dot = x * v.x + y * v.y;
    double v1mag = sqrt(x * x + y * y);
    double v2mag = sqrt(v.x * v.x + v.y * v.y);
    double amt = dot / (v1mag * v2mag); //Based of definition of dot product
    //dot product / product of magnitudes gives amt
    if (amt <= -1) {
        return PI;
    } else if (amt >= 1) {
        return 0;
    }
    float tmp = acos(amt);
    return tmp;
}
