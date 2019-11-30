#pragma once
#include "AllHeader.h"
#include "Vector3.h"

enum Mode
{
    ATTRACT,
    REPEL,
    OBSTACLE
};

struct Circle
{
    Vector3 position;
    Vector4 color;
    float r = 8;
    Mode mode;

    Circle(Vector3 pos, float radius, Vector4 col, Mode m)
    {
        color = col;
        r = radius;
        mode = m;
        position = pos;
    }

    void Temp()
    {
        //float cirPosX = r*4 + (width * 0.4) + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(width * 0.6 - r*4));
        //float cirPosY = r*4 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(height - r*4));;
        //Vector3 position = Vector3(cirPosX, cirPosY, 0.0);
        //circles.push_back(Circle(position, r, attColor, ATTRACT));
    }

    void DrawCircle()
    {
        float DEF_D = 1.0f;
        float k = 0.0f;

        glBegin(GL_TRIANGLES);
        glColor4f(color.r, color.g, color.b, color.a);
        for (k = 0; k <= 360; k += DEF_D)
        {
            glVertex2f(position.x, position.y);
            glVertex2f(position.x + cos(k) * r, position.y + sin(k) * r);
            glVertex2f(position.x + cos(k + DEF_D * 0.1) * r,
                       position.y + sin(k + DEF_D * 0.1) * r);
        }
        glEnd();
    }

    bool CheckCirclePosition(Vector3 input)
    {
        Vector3 distVector = input - position;
        float distance = distVector.GetMagnitude();

        if(distance < r * 2)
        {
            return true;
        }

        return false;
    }

    void UpdateCirclePosition(Vector3 input)
    {
        position = input;
    }
};
