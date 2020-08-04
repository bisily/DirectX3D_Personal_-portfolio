texture			g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

	minfilter = linear;
	magfilter = linear;

};

texture			g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	minfilter = linear;
	magfilter = linear;

};

texture			g_SpecularTexture;

sampler SpercularSampler = sampler_state
{
	texture = g_SpecularTexture;
	
	minfilter = linear;
	magfilter = linear;

};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpercularSampler, In.vTexUV);


	Out.vColor = vAlbedo * vShade + vSpecular;

	return Out;
}

technique Default_Device
{
	pass Blend
	{
		ZwriteEnable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}


};