#include "Triangle.h"


Triangle::Triangle()
{
    float triPosX = 0.0;
    float triPosY = 0.0;
    float triVelX = 0.0;
    float triVelY = 0.0;

    position = Vector3(triPosX, triPosY, 0.0);
    velocity = Vector3(triVelX, triVelY, 0.0);

    color1 = {0.5, 0.2, 1.0, 1.0};
    color2 = {1.0, 0.5, 1.0, 1.0};
    scale = 0.5;
}

Triangle::Triangle(int width, int height)
{
    float triPosX = (width * 0.3) + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(width * 0.7));
    float triPosY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(height));
    float angle = -1.0f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f);
    float triVelX = sin(angle * PI/2);
    float triVelY = cos(angle * PI/2);

    position = Vector3(triPosX, triPosY, 0.0);
    velocity = Vector3(triVelX, triVelY, 0.0);

    color1 = {0.5, 0.2, 1.0, 1.0};
    color2 = {1.0, 0.5, 1.0, 1.0};
    scale = 0.7;
}

Triangle::Triangle(float x, float y)
{
    float angle = -1.0f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f);
    float triVelX = sin(angle * PI/2);
    float triVelY = cos(angle * PI/2);

    position = Vector3(x, y, 0.0);
    velocity = Vector3(triVelX, triVelY, 0.0);

    color1 = {0.5, 0.2, 1.0, 1.0};
    color2 = {1.0, 0.5, 1.0, 1.0};
    scale = 0.7;
}

void Triangle::Draw()
{
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(position.x, position.y, position.z);
    glScalef(scale, scale, 0.0f);
    glRotatef(rotationAngle, 0.0, 0.0, 1.0);
    DrawTriangle();

    glPopMatrix();
}

void Triangle::DrawTriangle()
{
    glBegin(GL_TRIANGLES);

    glColor4f(color2.r, color2.g, color2.b, color2.a);
    glVertex2f(0.0, 0.0);
    glVertex2f(-4.0, 15.0);
    glVertex2f(4.0, 15.0);

    glColor4f(color1.r, color1.g, color1.b, color1.a);
    glVertex2f(0.0, 0.0);
    glVertex2f(-2.0, 7.0);
    glVertex2f(2.0, 7.0);

    glEnd();
}

void Triangle::Update()
{
    velocity += acceleration;
    velocity.SetLimit(maxSpeed);
    position += velocity;

    Vector3 angleVector = velocity;
    rotationAngle = (float) 90 + atan2(angleVector.y, angleVector.x) * 180 / PI;
    acceleration = Vector3(0,0,0);
}

void Triangle::ChangeColor(bool check)
{
    static bool modeCheck = false;
    if(modeCheck != check)
    {
        Vector4 temp = color1;
        color1 = color2;
        color2 = temp;
        modeCheck = !modeCheck;
    }
}

float Lerping(float value, float start1, float stop1, float start2, float stop2)
{
   return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

void Triangle::CalculateBoid(int id, std::vector<Triangle> &triangles, float aliMulti, float cohMulti, float sepMulti)
{
    Alignments(id, triangles, 50.0f, aliMulti, 1.0, 1.0);
    Cohesion(id, triangles, 50.0f, cohMulti, 1.0, 1.0);
    Separation(id, triangles, 25.0f, sepMulti, 1.0, 1.0);
}

void Triangle::Alignments(int id, std::vector<Triangle> &triangles,
                          float maxDistance, float mag, float speedMag,
                          float forceMag)
{
    Vector3 steeringVector;
    Vector3 distanceVector;
    float distance;
    int total = 0;
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        if(id != i) /*! Reconsider this part */
        {
            distanceVector = triangles[i].position - position;
            distance = distanceVector.GetMagnitude();

            if(distance < maxDistance)
            {
                steeringVector += triangles[i].velocity;
                total++;
            }
        }
    }

    if(total > 0)
    {
        steeringVector /= total;
        steeringVector.SetMagnitude(speed);
        steeringVector -= velocity;
        steeringVector.SetLimit(force);
        steeringVector *= mag;
        acceleration += steeringVector;
    }
}

void Triangle::Cohesion(int id, std::vector<Triangle> &triangles,
                        float maxDistance, float mag, float speedMag,
                        float forceMag)
{
    Vector3 steeringVector;
    Vector3 distanceVector;
    float distance;
    int total = 0;
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        if(id != i) /*! Reconsider this part */
        {
            distanceVector = triangles[i].position - position;
            distance = distanceVector.GetMagnitude();

            if(distance < maxDistance)
            {
                steeringVector += triangles[i].position;
                total++;
            }
        }
    }

    if(total > 0)
    {
        steeringVector /= total;
        steeringVector -= position;
        steeringVector.SetMagnitude(speed);
        steeringVector -= velocity;
        steeringVector.SetLimit(force);
        steeringVector *= mag;
        acceleration += steeringVector;
    }
}

void Triangle::Separation(int id, std::vector<Triangle> &triangles,
                          float maxDistance, float mag, float speedMag,
                          float forceMag)
{
    Vector3 steeringVector;
    Vector3 distanceVector;
    float distance;
    int total = 0;
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        if(id != i) /*! Reconsider this part */
        {
            distanceVector =  position - triangles[i].position;
            distance = distanceVector.GetMagnitude();

            if(distance < maxDistance)
            {
                //! distanceVector.Normalize(); /*! Reconsider this part */
                distanceVector /= distance;
                steeringVector += distanceVector;
                total++;
            }
        }
    }

    if(total > 0)
    {
        steeringVector /= total; //! disable or enable to see the difference
        steeringVector.SetMagnitude(speed);
        steeringVector -= velocity;
        steeringVector.SetLimit(force);
        steeringVector *= mag;
        acceleration += steeringVector;
    }
}

void Triangle::Boundary(int width, int height,
                        bool enableLimit, float widthLimit, float heightLimit,
                        float mag, float speedMag, float forceMag)
{
    float widthInit = width * 0.3;
    if(enableLimit == false)
    {
        if(position.x > width + r) { position.x = -r + widthInit; }
        if(position.y > height + r) { position.y = -r; }
        if(position.x < -r + widthInit) { position.x = width + r; }
        if(position.y < -r) { position.y = height + r; }
    }
    else if(enableLimit == true)
    {
        float speed = maxSpeed * speedMag;
        float force = maxForce * forceMag;

        Vector3 boundaryForce;

        if (position.x < widthLimit + widthInit) { boundaryForce = Vector3(maxSpeed, velocity.y, 0.0f); }
        if (position.x > width - widthLimit) { boundaryForce = Vector3(-maxSpeed, velocity.y, 0.0f); }
        if (position.y < heightLimit) { boundaryForce = Vector3(velocity.x, maxSpeed, 0.0f); }
        if (position.y > height - heightLimit) { boundaryForce = Vector3(velocity.x, -maxSpeed, 0.0f); }

        if(boundaryForce.GetMagnitude() > 0)
        {
            boundaryForce.SetMagnitude(speed);
            Vector3 steer = boundaryForce - velocity;
            steer.SetLimit(force);
            steer *= mag;
            acceleration += steer;
        }
    }
}

void Triangle::Seek(Vector3 target, float mag, float speedMag, float forceMag)
{
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    Vector3 desiredVel = target - position;
    desiredVel.SetMagnitude(speed);
    Vector3 steer = desiredVel - velocity;
    steer.SetLimit(force);

    steer *= mag;
    acceleration += steer;
}

void Triangle::Arrive(Vector3 target, float mag, float speedMag,
                      float forceMag, float distanceLimit)
{
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    Vector3 desiredVel = target - position;
    float distance = desiredVel.GetMagnitude();

    if(distance < distanceLimit)
    {
        speed = Lerping(distance, 0, distanceLimit, 0, maxSpeed);
        desiredVel.SetMagnitude(speed);
    }
    else
    {
        desiredVel.SetMagnitude(speed);
    }

    Vector3 steer = desiredVel - velocity;
    steer.SetLimit(maxForce);
    steer *= mag;
    acceleration += steer;
}

void Triangle::View(int id, std::vector<Triangle> &triangles)
{
    Vector3 viewVector;
    Vector3 distanceVector;

    float sightDistance = 100;
    float periphery = PI/4;
    float rotation;
    float distance;
    int total = 0;

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        if(id != i)
        {
            viewVector =  triangles[i].position - position;
            distance = distanceVector.GetMagnitude();
            Vector3 angleVector = viewVector - velocity;
            rotation = (float) 90 + atan2(angleVector.y, angleVector.x) * 180 / PI;

            if(rotation < periphery && distance < sightDistance)
            {
                triangles[i].ChangeColor(true);
            }
            else
            {
                triangles[i].ChangeColor(false);
            }
        }
    }
}

Vector3 Triangle::Origin(Vector3 target)
{
    //acceleration += (target - position);
    //acceleration.SetLimit(maxForce * 0.001);

    Vector3 desiredVel = target - position;
    float distanceLimit = 100;
    float distance = desiredVel.GetMagnitude();
    float speed = maxSpeed * 10;

    /*
    if(distance < distanceLimit)
    {
        speed = Lerping(distance, 0, 100, 0, maxSpeed);
    }
    */
    desiredVel.SetMagnitude(speed);
    Vector3 steer = desiredVel - velocity;
    steer.SetLimit(maxForce);
    return steer;
}

void Triangle::Repel(Vector3 target, float mag, float speedMag,
                     float forceMag, float distanceLimit)
{
    Vector3 desiredVel = target - position;
    float distance = desiredVel.GetMagnitude();
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    if(distance < distanceLimit)
    {
        desiredVel.SetMagnitude(speed);
        desiredVel *= -1;
        Vector3 steer = desiredVel - velocity;
        steer.SetLimit(force);
        steer *= mag;
        acceleration += steer;
    }
}

void Triangle::Attract(Vector3 target, float mag, float speedMag,
                       float forceMag, float distanceLimit)
{
    Vector3 desiredVel = target - position;
    float distance = desiredVel.GetMagnitude();
    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    //if(distance < distanceLimit)
    if(distance > distanceLimit/3 && distance < distanceLimit)
    {
        desiredVel.SetMagnitude(speed);
        Vector3 steer = desiredVel - velocity;
        steer.SetLimit(force);
        steer *= mag;
        acceleration += steer;
    }
}

void Triangle::HollowCircle(Vector3 target, float mag, float speedMag,
                            float forceMag, float distanceLowerLimit,
                            float distanceUpperLimit)
{
    Vector3 desiredVel = target - position;
    float distance = desiredVel.GetMagnitude();

    float speed = maxSpeed * speedMag;
    float force = maxForce * forceMag;

    if(distance > distanceLowerLimit && distance < distanceUpperLimit)
    {
        desiredVel.SetMagnitude(speed);
        Vector3 steer = desiredVel - velocity;
        steer.SetLimit(force);
        steer *= mag;
        acceleration += steer;
    }
    else if(distance < distanceLowerLimit)
    {
        desiredVel.SetMagnitude(speed);
        desiredVel *= -1;
        Vector3 steer = desiredVel - velocity;
        steer.SetLimit(force);
        steer *= mag;
        acceleration += steer;
    }
}

Vector3 Triangle::HollowCirclePattern01(Vector3 target)
{
    Vector3 desiredVel = target - position;
    Vector3 result;
    Vector3 resultUpper;
    Vector3 resultLower;
    float distanceLowerLimit = 200;
    float distanceUpperLimit = 500;
    float distance = desiredVel.GetMagnitude();
    float speed = maxSpeed * 100;
    float force = maxForce;

    if(distance < distanceUpperLimit) //distance > distanceLowerLimit &&
    {
        //desiredVel.SetMagnitude(maxSpeed);
        desiredVel.SetMagnitude(speed);
        Vector3 steer = desiredVel - velocity;
        steer.SetLimit(force);
        resultUpper = steer;
    }

    if(distance < distanceLowerLimit)
    {
        //desiredVel.SetMagnitude(maxSpeed);
        desiredVel.SetMagnitude(speed);
        desiredVel *= -1;
        Vector3 steer = desiredVel - velocity;
        steer.SetLimit(force);
        resultLower = steer;
    }

    //return resultUpper - resultLower;
    return resultUpper + resultLower;
}

