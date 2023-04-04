struct PS_INPUT // == VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 normal   : NORMAL;
    float4 color    : COLOR;
};

cbuffer LightBuffer : register(b0)
{
    float4  ambientColor;
    float4  diffuseColor;
    float4  inverseLightDirection;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color;
    float diffuseBrightness;
    float4 ambient;
    float3 lightDir = normalize(inverseLightDirection.xyz);
    float3 normal = normalize(input.normal);

    color = ambientColor;
    diffuseBrightness = saturate(dot(normal, lightDir));

    if (diffuseBrightness > 0.0f)
    {
        color += (diffuseColor * diffuseBrightness);
        color = saturate(color);
    }

    color *= input.color;
    color = saturate(color);
    return color;
}