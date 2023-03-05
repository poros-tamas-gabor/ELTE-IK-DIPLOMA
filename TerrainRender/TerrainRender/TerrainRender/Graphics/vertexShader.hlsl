cbuffer cBuffer : register(b0)
{
    float4x4 worldMat;
    float4x4 viewMat;
    float4x4 projectioMat;
}


struct VS_INPUT
{
    float3 position : POSITION;
    //float3 normal   : NORMAL;
    float4 color    : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    //float4 normal   : NORMAL;
    float4 color    : COLOR;
};


VS_OUTPUT main(VS_INPUT input) 
{
    VS_OUTPUT output;

    output.position = float4(input.position, 1.0f);
    output.position = mul(output.position, worldMat);
    output.position = mul(output.position, viewMat);
    output.position = mul(output.position, projectioMat);
    //output.normal = float4(input.normal, 1.0f);
    output.color = input.color;

    return output;
}
