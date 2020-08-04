Matrix	g_matWorld;			// 상수테이블
Matrix	g_matView;
Matrix	g_matProj;

texture g_BaseTexture;

float g_fAlph;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;			// 샘플러의 색상 정보를 가져오는 대상
	
	minfilter = linear;					// 텍스쳐의 필터링을 다이렉트 장치를 사용하지 않고 쉐이더에서 사용(축소 시)		
	magfilter = linear;					// 텍스쳐의 필터링을 다이렉트 장치를 사용하지 않고 쉐이더에서 사용(확대 시)
};

struct VS_IN
{
	vector			vPosition : POSITION;		// symantic(반드시 대문자로 정의)
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT								//  버텍스 쉐이더 출력의 경우 반드시 POSITION이 있어야만 한다.
{											//  출력 구조체의 경우 POSITION을 제외한 출력 값이 하나 이상은 반드시 있어야 한다.
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)			// 쉐이더의 인자는 일일이 멤버 변수로 받을 수도 있는 반면, 반환 타입은 반드시 구조체를 정의해서 반환 값으로 만들어줘야 한다.
{
	VS_OUT		Out = (VS_OUT)0;		// 구조체 초기화 문법


	// VS_IN에 있는 vPosition이라는 멤버는 로컬상의 좌표이다.
	Matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);				// mul 함수는 곱하기가 불가능할 행렬 상태일 때는 에러를 발생시키고 제대로 된 경우 곱셈을 수행
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN			// 픽셀쉐이더 구조체에서는 절대 POSITION 시맨틱 사용 불가
{
	float2				vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector				vColor : COLOR0;	// 색상을 설정하기 위한 SYMANTIC
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  OUT = (PS_OUT)0;

	OUT.vColor = tex2D(BaseSampler, In.vTexUV); // 2차원에 해당되는 텍스처 uv좌표에 있는 색상들로 픽셀의 color를 채우겠다는 의미

	OUT.vColor.a = g_fAlph;

	return OUT;
}  

technique Default_Device			// 다이렉트에서 지원하는 쉐이더 버전의 명명을 의미
{
	pass  // 기능의 캡슐화
	{
		AlphaBlendEnable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_MAIN();

	}
}