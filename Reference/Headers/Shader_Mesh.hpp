#include "Shader_Base.hpp"

float	g_fPower = 1.f;

vector	g_vLightDir = (vector)0.f;

vector	g_vLightDiffuse;
vector	g_vLightAmbient;
vector	g_vLightSpecular;

vector	g_vMtrlDiffuse;
vector	g_vMtrlAmbient;
vector	g_vMtrlSpecular;

vector	g_vCamPosition;


struct	VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	vector vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);

	vector	vWorldLight = g_vLightDir * -1.f;

	// ³­¹æ»ç±¤
	float fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLight)), 0.f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	// Á¤¹Ý»ç±¤
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector  vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	vector	vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = pow(max(dot((normalize(vLook) * -1.f), normalize(vReflect)), 0.f), g_fPower);
	Out.vSpecular.a = 1.f;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector	  vShade : COLOR0;
	vector	  vSpecular : COLOR1;
	float2    vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	
	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse))
		* (In.vShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular));

	Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_MAIN_ALPHA_DISABLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse))
		* (In.vShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular));

	Out.vColor.a = 0.5f;

	return Out;
}


PS_OUT PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	pass	Default_Rendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AlphaTesting
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0xc0;
		cullmode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHA();
	}

	pass Default_Alpha_Rendering
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0xc0;
		cullmode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHA();
	}

};