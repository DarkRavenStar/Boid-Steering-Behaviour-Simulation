#pragma once
#include "AllHeader.h"
#include "Vector3.h"

struct Triangle
{
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;

    Vector4 color1;
    Vector4 color2;
    float scale;
    float rotationAngle;

    float r = 8;
    float maxSpeed = 3;
    float maxForce = 0.05;

    Triangle();
    Triangle(int width, int height); //! Initialize Constructor
    Triangle(float x, float y); //! Runtime Constructor

    void Draw();
    void DrawTriangle();
    void CalculateBoid(int id, std::vector<Triangle> &triangles, float aliMulti, float cohMulti, float sepMulti);
    void Alignments(int id, std::vector<Triangle> &triangles,
                    float maxDistance, float mag, float speedMag,
                    float forceMag);

    void Cohesion(int id, std::vector<Triangle> &triangles,
                  float maxDistance, float mag, float speedMag,
                  float forceMag);

    void Separation(int id, std::vector<Triangle> &triangles,
                    float maxDistance, float mag, float speedMag,
                    float forceMag);

    void Boundary(int width, int height, bool enableLimit,
                  float widthLimit, float heightLimit, float mag,
                  float speedMag, float forceMag);

    void Update();
    void ChangeColor(bool check);
    void View(int id, std::vector<Triangle> &triangles);

    void Seek(Vector3 target, float mag, float speedMag, float forceMag);
    void Arrive(Vector3 target, float mag, float speedMag, float forceMag, float distanceLimit);
    void Repel(Vector3 target, float mag, float speedMag, float forceMag, float distanceLimit);
    void Obstacle(Vector3 target, float mag, float speedMag, float forceMag, float distanceLimit);
    void Attract(Vector3 target, float mag, float speedMag, float forceMag, float distanceLimit);

    void HollowCircle(Vector3 target, float mag, float speedMag,
                      float forceMag, float distanceLowerLimit,
                      float distanceUpperLimit);

    Vector3 HollowCirclePattern01(Vector3 target);

    Vector3 Origin(Vector3 target);
};
