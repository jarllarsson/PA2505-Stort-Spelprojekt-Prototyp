#include "SSAO.h"

SSAO::SSAO( ID3D11Device* p_deivce, ID3D11DeviceContext* p_deviceContext, Shader* p_shader )
{
	m_device		= p_deivce;
	m_deviceContext = p_deviceContext;
	m_shader		= p_shader;

	struct PTVertex 
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 texCoord;
	};

	PTVertex mesh[] = {
		{D3DXVECTOR3( 1, -1, 0),	D3DXVECTOR2(1,1)},
		{D3DXVECTOR3( -1, -1, 0),	D3DXVECTOR2(0,1)},
		{D3DXVECTOR3( 1, 1,	0),		D3DXVECTOR2(1,0)},

		{D3DXVECTOR3( -1, -1, 0),	D3DXVECTOR2(0,1)},
		{D3DXVECTOR3( 1, 1, 0),		D3DXVECTOR2(1,0)},
		{D3DXVECTOR3( -1, 1, 0),	D3DXVECTOR2(0,0)},
	};

	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(PTVertex);
	bufferDesc.InitData = mesh;
	bufferDesc.NumElements = 6;
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	m_buffer = new Buffer();
	HRESULT hr = m_buffer->Init(p_deivce,p_deviceContext,bufferDesc);

	hr = D3DX11CreateShaderResourceViewFromFile(p_deivce,"../Textures/randNormals.jpg",0,0,&m_randomNormals,0);

	m_SSAOInfo.gRandomTexSize	= 64;
	m_SSAOInfo.gScreenSize		= D3DXVECTOR2((float)SCREEN_WIDTH,(float)SCREEN_HEIGHT);
	m_SSAOInfo.gScale			= 1.5f;
	m_SSAOInfo.gSampleRadius	= 0.3f;
	m_SSAOInfo.gBias			= 0.2f;
	m_SSAOInfo.gIntensity		= 1.0f;
}

SSAO::~SSAO()
{
	SAFE_DELETE(m_buffer);
	SAFE_RELEASE(m_randomNormals);
}

void SSAO::RenderPostProcessing( D3DXMATRIX p_vp, D3DXMATRIX p_invP, ID3D11ShaderResourceView* p_normalBuffer, ID3D11ShaderResourceView* p_depthBuffer)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_shader->SetResource(	"tex_normalBuffer", p_normalBuffer);
	m_shader->SetResource(	"tex_depthBuffer", p_depthBuffer);
	m_shader->SetResource(	"tex_randNormals",m_randomNormals);
	m_shader->SetFloat4(	"gScreenSize", D3DXVECTOR4(m_SSAOInfo.gScreenSize));
	m_shader->SetFloat(		"gRandomTexSize", m_SSAOInfo.gRandomTexSize );
	m_shader->SetFloat(		"gScale", m_SSAOInfo.gScale);
	m_shader->SetFloat(		"gBias", m_SSAOInfo.gBias);
	m_shader->SetFloat(		"gIntensity", m_SSAOInfo.gIntensity);
	m_shader->SetFloat(		"gSampleRadius", m_SSAOInfo.gSampleRadius);
	m_shader->SetMatrix(	"gWVP", p_vp);
	m_shader->SetMatrix(	"gInvVP",p_invP);
	m_shader->Apply(0);

	m_buffer->Apply();

	m_deviceContext->Draw(6,0);
	m_shader->SetResource(	"tex_depthBuffer", NULL);
	m_shader->SetResource(	"tex_randNormals", NULL);
	m_shader->SetResource(	"tex_normalBuffer", NULL);
	m_shader->Apply(0);
}

SSAO_parms* SSAO::GetSSAOParms()
{
	return &m_SSAOInfo;
}