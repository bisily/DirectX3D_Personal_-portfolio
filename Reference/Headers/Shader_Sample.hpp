Matrix	g_matWorld;			// ������̺�
Matrix	g_matView;
Matrix	g_matProj;

texture g_BaseTexture;

float g_fAlph;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;			// ���÷��� ���� ������ �������� ���
	
	minfilter = linear;					// �ؽ����� ���͸��� ���̷�Ʈ ��ġ�� ������� �ʰ� ���̴����� ���(��� ��)		
	magfilter = linear;					// �ؽ����� ���͸��� ���̷�Ʈ ��ġ�� ������� �ʰ� ���̴����� ���(Ȯ�� ��)
};

struct VS_IN
{
	vector			vPosition : POSITION;		// symantic(�ݵ�� �빮�ڷ� ����)
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT								//  ���ؽ� ���̴� ����� ��� �ݵ�� POSITION�� �־�߸� �Ѵ�.
{											//  ��� ����ü�� ��� POSITION�� ������ ��� ���� �ϳ� �̻��� �ݵ�� �־�� �Ѵ�.
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)			// ���̴��� ���ڴ� ������ ��� ������ ���� ���� �ִ� �ݸ�, ��ȯ Ÿ���� �ݵ�� ����ü�� �����ؼ� ��ȯ ������ �������� �Ѵ�.
{
	VS_OUT		Out = (VS_OUT)0;		// ����ü �ʱ�ȭ ����


	// VS_IN�� �ִ� vPosition�̶�� ����� ���û��� ��ǥ�̴�.
	Matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);				// mul �Լ��� ���ϱⰡ �Ұ����� ��� ������ ���� ������ �߻���Ű�� ����� �� ��� ������ ����
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN			// �ȼ����̴� ����ü������ ���� POSITION �ø�ƽ ��� �Ұ�
{
	float2				vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector				vColor : COLOR0;	// ������ �����ϱ� ���� SYMANTIC
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  OUT = (PS_OUT)0;

	OUT.vColor = tex2D(BaseSampler, In.vTexUV); // 2������ �ش�Ǵ� �ؽ�ó uv��ǥ�� �ִ� ������ �ȼ��� color�� ä��ڴٴ� �ǹ�

	OUT.vColor.a = g_fAlph;

	return OUT;
}  

technique Default_Device			// ���̷�Ʈ���� �����ϴ� ���̴� ������ ����� �ǹ�
{
	pass  // ����� ĸ��ȭ
	{
		AlphaBlendEnable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();

	}
}