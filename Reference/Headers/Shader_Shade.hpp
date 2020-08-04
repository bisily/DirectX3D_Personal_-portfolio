vector		g_vLightDir;

vector		g_vLightPos;


vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;
vector		g_vCamPosition;

Matrix		g_matViewInv;
Matrix		g_matProjInv;

float		g_fPower = 10.f;
float		g_fRange;




texture			g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
};

PS_OUT		PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) 
		* (g_vLightDiffuse * g_vMtrlDiffuse) 
		+ (g_vLightAmbient * g_vMtrlAmbient);


	vector		vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	float		vViewZ = vDepth.y * 1000.f;

	vector		vPosition;

	vPosition.x = vViewZ * (In.vTexUV * 2.f - 1.f);
	vPosition.y = vViewZ * (In.vTexUV * -2.f + 1.f);
	vPosition.z = vViewZ * vDepth.x;
	vPosition.w = vViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);
	
	// 카메라가 픽셀을 바라보는 방향벡터
	vector		vLook = normalize(g_vCamPosition - vPosition);

	Out.vSpecular = pow(max(dot(vLook, normalize(vReflect)), 0.f), g_fPower);

	
	return Out;
}

PS_OUT		PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);
	
	vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	float		vViewZ = vDepth.y * 1000.f;
	
	vector		vPosition;

	vPosition.x = vViewZ * (In.vTexUV * 2.f - 1.f);
	vPosition.y = vViewZ * (In.vTexUV * -2.f + 1.f);
	vPosition.z = vViewZ * vDepth.x;
	vPosition.w = vViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector vLightDir = vPosition - g_vLightPos;
	float  fDistance = length(vLightDir);

	float  fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);
	
	Out.vShade = (max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f)
		* (g_vLightDiffuse * g_vMtrlDiffuse)
		+ (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vector(vLightDir.xyz, 0.f)), vNormal);

	// 카메라가 픽셀을 바라보는 방향벡터
	vector		vLook = normalize(g_vCamPosition - vPosition);

	Out.vSpecular = (pow(max(dot(vLook, normalize(vReflect)), 0.f), g_fPower)) * fAtt;


	return Out;
}

technique Default_Device
{
	pass Direction
	{
		ZwriteEnable = false;

		Alphablendenable = true;
		srcblend = one;
		destblend = one;		

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Point
	{
		ZwriteEnable = false;

		Alphablendenable = true;
		srcblend = one;
		destblend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
};