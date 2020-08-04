Matrix		g_matWorld;
Matrix		g_matView;
Matrix		g_matProj;

texture		g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	
	minfilter = linear;
	magfilter = linear;

};