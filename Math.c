#include "Math.h"

const Vec2 Vec2_Up    = {  0.0f,  1.0f };
const Vec2 Vec2_Down  = {  0.0f, -1.0f };
const Vec2 Vec2_Left  = { -1.0f,  0.0f };
const Vec2 Vec2_Right = {  1.0f,  0.0f };
const Vec2 Vec2_Zero  = {  0.0f,  0.0f };
const Vec2 Vec2_One   = {  1.0f,  1.0f };

Vec2 Vec2_Set(float x, float y)
{
    Vec2 v = { .x = x, .y = y };
    return v;
}

Vec2 Vec2_Add(Vec2 v1, Vec2 v2)
{
    Vec2 Vec2_Zero = { v1.x + v2.x, v1.y + v2.y };
    return Vec2_Zero;
}

Vec2 Vec2_Sub(Vec2 v1, Vec2 v2)
{
    Vec2 Vec2_Zero = { v1.x - v2.x, v1.y - v2.y };
    return Vec2_Zero;
}

Vec2 Vec2_Scale(Vec2 v, float s)
{
    Vec2 Vec2_Zero = { v.x * s, v.y * s };
    return Vec2_Zero;
}

float Vec2_Length(Vec2 v)
{
    return (sqrtf((pow( v.x, 2 ) + pow( v.y, 2 ))));
}

Vec2 Vec2_Normalize(Vec2 v)
{
    float v_norme = Vec2_Length( v );
    Vec2 Vec2_Zero = { ( v.x / v_norme ), ( v.y / v_norme )};
    return Vec2_Zero;
}

float Vec2_Distance(Vec2 v1, Vec2 v2)
{
    Vec2 Distance = Vec2_Sub( v1, v2 );
    return Vec2_Length( Distance );
}
