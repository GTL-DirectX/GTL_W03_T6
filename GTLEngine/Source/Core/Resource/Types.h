#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"

enum class EPrimitiveType
{
    None,
    Line,
    Plane,
    Sphere,
    Cube,
    Cylinder,
    Cone,
    Grid,
    Max,
};

struct FVertexSimple
{
    float X = 0.f, Y = 0.f, Z = 0.f;
    float R = 0.f, G = 0.f, B = 0.f, A = 1.f;
};

struct FVertexInfo
{
    uint NumVertices;
    ID3D11Buffer* VertexBuffer;
};

//////////
// 
// Constant Buffers
// 
//////////
enum class EConstantBufferType
{
    None,
    ChangesOnResize,
    ChangesEveryFrame,
    ChangesEveryObject,
    MVP,
    Max,
};

 struct alignas(16) FCbChangesOnResize
{
    FMatrix ProjectionMatrix;
};


struct alignas(16) FCbChangesEveryFrame
{
    FMatrix ViewMatrix;
};

struct alignas(16)  FCbChangesEveryObject
{
    FMatrix WorldMatrix;
};

struct alignas(16) FMVP
{
    FMatrix MVP;
};

struct FCbLine
{
    FVector Position;
    float R, G, B, A;
};

struct FRay {
    FVector Origin;
    FVector Direction;
    FRay(FVector origin, FVector direction) : Origin(origin), Direction(direction) {};
};

struct FAABB {
    FVector min;
    FVector max;
    FAABB(FVector min, FVector max) : min(min), max(max) {};
};