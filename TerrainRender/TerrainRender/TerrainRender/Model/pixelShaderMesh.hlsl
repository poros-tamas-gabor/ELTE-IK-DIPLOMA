struct PS_INPUT // == VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float4 color    : COLOR;
};

cbuffer LightBuffer : register(b0)
{
    float4  ambientColor;
    float4  diffuseColor;
    float4  inverseLightDirection;
    float4  isShadingOn;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color;
    float diffuseBrightness;
    float4 ambient;
    float3 lightDir;
    float3 normal;

    if (isShadingOn.x > 0)
    {
        lightDir = normalize(inverseLightDirection.xyz);
        normal = normalize(input.normal);

        //to test normal vectors
       // return color = float4((normal * 0.5) + 0.5, 1.0f);

        color = ambientColor;
        diffuseBrightness = saturate(dot(normal, lightDir));

        if (diffuseBrightness > 0.0f)
        {
            color += (diffuseColor * diffuseBrightness);
            color = saturate(color);
        }

        color *= input.color;
        color = saturate(color);
    }
    else
    {
        color = input.color;
    }

    return color;
}