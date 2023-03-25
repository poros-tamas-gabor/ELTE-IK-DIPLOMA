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
    float color;
    float diffuseBrightness;
    float4 ambient;

    color = input.color;
    diffuseBrightness = saturate(dot(input.normal, inverseLightDirection));
    color *= saturate(diffuseBrightness * diffuseColor);

    ambient = saturate(input.color * ambientColor);
    color += ambient;
    return color;
}