struct PS_INPUT 
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	float4 color = float4(depthValue, depthValue, depthValue, 1.0f);
	return color;
}