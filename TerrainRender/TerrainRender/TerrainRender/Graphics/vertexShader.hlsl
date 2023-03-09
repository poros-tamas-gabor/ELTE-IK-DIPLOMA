cbuffer MatrixBuffer : register(b0)
{
    float4x4 worldMat;
    float4x4 viewMat;
    float4x4 projectionMat;
};

cbuffer LightBuffer : register(b1)
{
    float4  ambientColor;
    float4  diffuseColor;
    float4  lightDirection;
};


struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float4 color    : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 normal   : NORMAL;
    float4 color    : COLOR;
};


VS_OUTPUT main(VS_INPUT input) 
{
    VS_OUTPUT output;

    output.color    = input.color;
    output.position = float4(input.position, 1.0f);
    output.normal   = float4(input.normal, 0.0f);
    
    output.position = mul(output.position, worldMat);
    output.position = mul(output.position, viewMat);
    output.position = mul(output.position, projectionMat);


    float4 lightDir = lightDirection;
    lightDir *= -1;

    output.normal = mul(output.normal, worldMat);
    output.normal = normalize(output.normal);

    float diffuseBrightness = saturate(dot(output.normal, lightDir));
    output.color *= saturate(diffuseBrightness * diffuseColor);

    float4 ambient = saturate(input.color * ambientColor);
    output.color += ambient;


    return output;
}
