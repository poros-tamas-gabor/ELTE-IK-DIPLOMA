cbuffer MatrixBuffer : register(b0)
{
    float4x4 worldMat;
    float4x4 viewMat;
    float4x4 projectionMat;
    float4  color;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    //float4 color    : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float4 color    : COLOR;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.color = color;

    output.position = float4(input.position, 1.0f);
    output.position = mul(output.position, worldMat);
    output.position = mul(output.position, viewMat);
    output.position = mul(output.position, projectionMat);

    // Calculate the normal vector against the world matrix only.
    output.normal = input.normal;
    output.normal = mul(output.normal, (float3x3)worldMat);
    output.normal = normalize(output.normal);

    return output;
}