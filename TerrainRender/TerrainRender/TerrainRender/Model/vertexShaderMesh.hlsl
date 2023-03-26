cbuffer MatrixBuffer : register(b0)
{
    float4x4    worldMat;
    float4x4    viewMat;
    float4x4    projectionMat;

    float4x4    lightViewMatrix;
    float4x4    lightProjectionMatrix;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float4 color    : COLOR;
};

struct VS_OUTPUT
{
    float4 position             : SV_POSITION;
    float4 normal               : NORMAL;
    float4 color                : COLOR;
    float4 lightViewPosition    : TEXCOORD;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.color = input.color;

    float4 inputPosition = float4(input.position, 1.0f);

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(inputPosition, worldMat);
    output.position = mul(output.position, viewMat);
    output.position = mul(output.position, projectionMat);

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(inputPosition, worldMat);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);


    // Calculate the normal vector against the world matrix only.
    output.normal = float4(input.normal, 0.0f);
    output.normal = mul(output.normal, worldMat);
    output.normal = normalize(output.normal);

    return output;
}