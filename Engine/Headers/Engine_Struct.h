#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPosition;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong		VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef	struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTex;

	}VTXTEX;

	const _ulong		VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTex;

	}VTXCUBE;

	const _ulong		VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagIndex16
	{
		_ushort		_1, _2, _3;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong		_1, _2, _3;

	}INDEX32;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix				CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;

		LPD3DXMESH				pOriMesh;		//	������ ���¸� ������ �ִ� �޽�(�Һ�)

		_ulong					dwNumBones;		// ���� ����

		_matrix*				pFrameOffsetMatrix; // ���� �迭�� �Ҵ��� ����(���� ���� ���), 
													// �ִϸ��̼��� ������ ���·� �������� �� �� ���ʿ� ������� ������ �ִ� ��� 

		_matrix**				ppFrameCombinedMatrix;
		//	������ ������ �ִ� CombinedTransformationMatrix�� ��� ���ŵǱ� ������ �ƿ� �� ����� �ּҵ��� �����ϱ� ���� ������

		_matrix*				pRenderingMatices;

	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagSphereCol
	{
		_float		fScaling = 1.f;
		_float		fRadius = 1.f;

		_matrix*	pmatPos;
		_vec3*		pVertices;
		LPD3DXMESH	pMesh;

	}SPHERE_COL;

	typedef	struct tagVertexScreen
	{
		_vec4			vPosition;
		_vec2			vTexUV;

	}VTXSCREEN;

	const _ulong		VTXFVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;
	// ������ȯ�� ���� ������ ��ǥ ��(�� FVF�� ������ ���� ��ǥ�� �־��൵ �����ϴ�)
	// X, Y�� : ������ ��ǥ ��, Z : ���Ŀ뵵(��Դ� 0~1���� ��), W : ��ġ ��ǥ(��ġ�� ������ ä����, �Ϲ������� 1�� ä����)

	typedef struct tagVertexTrail
	{
		_vec3			vPos;
		_vec2			vTexUV;
	}VTXTRAIL;

	const DWORD VTXFVF_TRAIL = D3DFVF_XYZ | D3DFVF_TEX1;

}

#endif // Engine_Struct_h__
