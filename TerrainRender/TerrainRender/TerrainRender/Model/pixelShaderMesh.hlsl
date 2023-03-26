//////////////
// TEXTURES //
//////////////

//The depthMapTexture is the shadow map. This texture contains the scene depth buffer rendered from the light's perspective.
Texture2D depthMapTexture : register(t0);

///////////////////
// SAMPLE STATES //
///////////////////

//We require a clamp based sampler when sampling the depth buffer so that it doesn't wrap around and sample incorrect information.
SamplerState SampleTypeClamp : register(s0);

struct PS_INPUT // == VS_OUTPUT
{
    float4 position             : SV_POSITION;
    float4 normal               : NORMAL;
    float4 color                : COLOR;
    float4 lightViewPosition    : TEXCOORD;
};

cbuffer LightBuffer : register(b0)
{
    float4  ambientColor;
    float4  diffuseColor;
    float4  inverseLightDirection;
};


float4 main(PS_INPUT input) : SV_TARGET
{

   float bias;
   float lightDepthValue;
   float diffuseBrightness;
   float depthValue;

   float2  projectTexCoord;
   float4  ambient = { 0.0f, 0.0f, 0.0f, 0.0f };
   float4   finalColor = {0.0f, 0.0f, 0.0f, 0.0f};

   // Set the bias value for fixing the floating point precision issues.
   //Shadow mapping requires a bias adjustment when comparing the depth of the light and the depth of the object due to the low floating point precision of the depth map.
   bias = 0.001f;

   // Calculate the projected texture coordinates.
   //Calculate the projected texture coordinates for sampling the shadow map (depth buffer texture) based on the light's viewing position.
   projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
   projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;


   // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
   if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
   {
	   //Now that we are in the view of the light we will retrieve the depth value from the shadow map (depthMapTexture). We only sample the red component since this is a grey scale texture. 
	   // The depth value we get from the texture translates into the distance to the nearest object. This is important since objects are what cast the shadows and hence why it is called a shadow map.
	   // Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
	   depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

	   // Calculate the depth of the light.
	   lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

	   // Subtract the bias from the lightDepthValue.
	   lightDepthValue = lightDepthValue - bias;
	   // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	   // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
	   if (lightDepthValue < depthValue)
	   {
		   // Calculate the amount of light on this pixel.
		  diffuseBrightness = saturate(dot(input.normal, inverseLightDirection));

		  if (diffuseBrightness > 0.0f)
		  {
			  // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
			  finalColor = diffuseColor * diffuseBrightness;
			  finalColor = saturate(finalColor);
		  }
	   }
  }
  else
  {
	   // Calculate the amount of light on this pixel.
	   diffuseBrightness = saturate(dot(input.normal, inverseLightDirection));
	   if (diffuseBrightness > 0.0f)
	   {
		   // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		   finalColor = diffuseColor * diffuseBrightness;
		   finalColor = saturate(finalColor);
	   }
   }

   // Add ambient light to final color.
   ambient = input.color * ambientColor;
   finalColor += ambient;
   finalColor.a = input.color.a;

   return finalColor;

}



















	//
	//	float bias;
	//float4 color;
	//float2 projectTexCoord;
	//float depthValue;
	//float lightDepthValue;
	//float lightIntensity;
	//float4 textureColor;
	//float3 lightDir;
	//
	//
	//
	//// Set the bias value for fixing the floating point precision issues.
	//bias = 0.001f;
	//
	//// Set the default output color to the ambient light value for all pixels.
	//color = ambientColor;
	//
	//// Calculate the projected texture coordinates.
	//projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	//projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	//
	//// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	//if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	//{
	//	// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
	//	depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;
	//
	//	// Calculate the depth of the light.
	//	lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
	//
	//	// Subtract the bias from the lightDepthValue.
	//	lightDepthValue = lightDepthValue - bias;
	//
	//	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	//	// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
	//	if (lightDepthValue < depthValue)
	//	{
	//		// Calculate the amount of light on this pixel.
	//		lightIntensity = saturate(dot(input.normal, inverseLightDirection));
	//
	//		if (lightIntensity > 0.0f)
	//		{
	//			// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	//			color += (diffuseColor * lightIntensity);
	//
	//			// Saturate the final light color.
	//			color = saturate(color);
	//		}
	//	}
	//}
	//else
	//{
	//	// If this is outside the area of shadow map range then draw things normally with regular lighting.
	//	lightIntensity = saturate(dot(input.normal, inverseLightDirection));
	//	if (lightIntensity > 0.0f)
	//	{
	//		color += (diffuseColor * lightIntensity);
	//		color = saturate(color);
	//	}
	//}
	//
	//
	//return color;
