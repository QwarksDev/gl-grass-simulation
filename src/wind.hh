#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;

class Wind
{
public:
    Wind() {}
    ~Wind() = default;

    virtual float get_strength(vec2 pos) = 0;
    virtual vec3 get_direction(vec2 pos) = 0;
    virtual void update() = 0;
};

class ConstantWind : public Wind
{
private:
    float strength;
    vec3 direction;

public:
    ConstantWind(float strength, vec3 direction, float speed)
        : Wind(), strength(strength), direction(direction)
    {}

    float get_strength(vec2) override
    {
        return strength;
    }

    vec3 get_direction(vec2) override
    {
        return direction;
    }

    void update() override {}
};

class PerlinWind : public Wind
{
private:
    float strength;
    vec3 direction;
    float speed;
    float time;

public:
    PerlinWind(float strength, vec3 direction, float speed)
        : Wind(), strength(strength), direction(direction), speed(speed), time(0)
    {}

    float get_strength(vec2 pos) override
    {
        return strength * sin(time + pos.x * 0.1 + pos.y * 0.1);
    }

    vec3 get_direction(vec2) override
    {
        return direction;
    }

    void update() override
    {
        time += speed;
    }
};