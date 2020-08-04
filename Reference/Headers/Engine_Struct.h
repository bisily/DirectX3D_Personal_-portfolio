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

		LPD3DXMESH				pOriMesh;		//	최초의 상태를 가지고 있는 메시(불변)

		_ulong					dwNumBones;		// 뼈의 개수

		_matrix*				pFrameOffsetMatrix; // 동적 배열로 할당할 예정(여러 개의 행렬), 
													// 애니메이션을 포함한 형태로 렌더링을 할 때 최초에 뼈대들이 가지고 있는 행렬 

		_matrix**				ppFrameCombinedMatrix;
		//	뼈들이 가지고 있는 CombinedTransformationMatrix가 계속 갱신되기 때문에 아예 그 행렬의 주소들을 보관하기 위한 포인터

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
	// 정점변환이 끝난 상태의 좌표 값(이 FVF는 윈도우 상의 좌표를 넣어줘도 무관하다)
	// X, Y값 : 순수한 좌표 값, Z : 정렬용도(대게는 0~1사이 값), W : 동치 좌표(장치가 스스로 채워줌, 일반적으론 1을 채워줌)

	typedef struct tagVertexTrail
	{
		_vec3			vPos;
		_vec2			vTexUV;
	}VTXTRAIL;

	const DWORD VTXFVF_TRAIL = D3DFVF_XYZ | D3DFVF_TEX1;

}

#endif // Engine_Struct_h__
