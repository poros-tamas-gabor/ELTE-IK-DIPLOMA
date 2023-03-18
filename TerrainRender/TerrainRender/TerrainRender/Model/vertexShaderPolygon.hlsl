cbuffer MatrixBuffer : register(b0)
{
    float4x4 worldMat;
    float4x4 viewMat;
    float4x4 projectionMat;
};


struct VS_INPUT
{
    float3 position : POSITION;
    float4 color    : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.color = input.color;
    output.position = float4(input.position, 1.0f);

    output.position = mul(output.position, worldMat);
    output.position = mul(output.position, viewMat);
    output.position = mul(output.position, projectionMat);

    return output;
}