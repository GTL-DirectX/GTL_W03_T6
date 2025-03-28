#include "MVPShader.hlsli"

// Texture 슬롯.
Texture2D TexAmbientMap : register(t0);
Texture2D TexDiffuseMap : register(t1);
Texture2D TexSpecularMap : register(t2);
Texture2D TexEmissiveMap : register(t3);
Texture2D TexBumpMap : register(t4);
Texture2D TexDisplacementMap : register(t5);
Texture2D TexNormalMap : register(t6);

SamplerState MatSampler : register(s0);

cbuffer MaterialCB : register(b4)
{
    float3 Ambient;
    float Opacity;
    
    
    float3 Diffuse;
    float Transparency;
    
    
    float3 Specular;
    float RefractiveIndex;
    
    
    float3 Emissive;
    float SpecularExponent;
    

    
    int IlluminationModel;
};

// StaticMeshShader
struct VS_INPUT
{
    float4 Pos : POSITION; // Input position from vertex buffer
    float3 Nor : NORMAL;
    float4 Color : COLOR; // Input color from vertex buffer
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Transformed position to pass to the pixel shader
    float3 Nor : NORMAL;
    float4 Color : COLOR; // Color to pass to the pixel shader
    float2 Tex : TEXCOORD;
};

PS_INPUT mainVS(VS_INPUT Input)
{
    Input.Pos.w = 1.f;
    
    PS_INPUT Output = (PS_INPUT) 0.0f;
    // Output.Pos = mul(Input.Pos, MVP); // 오브젝트 스페이스 -> NDC
    
    Output.Pos = Input.Pos;
    Output.Pos = mul(Output.Pos, WorldMatrix); // 오브젝트 스페이스 -> 월드 스페이스
    Output.Pos = mul(Output.Pos, ViewMatrix); // 월드 스페이스 -> 뷰 스페이스
    Output.Pos = mul(Output.Pos, ProjectionMatrix); // 뷰 스페이스 -> NDC
    
    Output.Tex = Input.Tex;
    Output.Color = Input.Color;

    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    float4 col = TexDiffuseMap.Sample(MatSampler, Input.Tex);
    
    return col;

}