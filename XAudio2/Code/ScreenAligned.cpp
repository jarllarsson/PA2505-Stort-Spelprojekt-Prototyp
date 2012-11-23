#include "ScreenAligned.h"
#include "SA_event.h"

ScreenAligned::ScreenAligned(ID3D11DeviceContext* p_deviceContext, ID3D11ShaderResourceView* p_sampleTarget, 
	Shader* p_shader, Buffer* p_vertexBuffer, D3DXVECTOR4 p_activeSampleChannels, bool p_draw/* =false */, 
	int p_invertedSampling/* =0 */)
{
	m_shader = p_shader;
	m_vertexBuffer = p_vertexBuffer;
	m_deviceContext = p_deviceContext;
	m_sampleTarget = p_sampleTarget;
	m_activeSampleChannels = p_activeSampleChannels;
	m_inverted = p_invertedSampling;
	m_draw = p_draw;

	m_eventListener = new SA_event(this);
}

ScreenAligned::~ScreenAligned()
{
	SAFE_DELETE(m_vertexBuffer);
}

void ScreenAligned::SwitchRenderState()
{
	if(m_draw)
		m_draw = false;
	else
		m_draw = true;
}

HRESULT ScreenAligned::Update(float p_deltaTime)
{
	return S_OK;
}

HRESULT ScreenAligned::Render()
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_vertexBuffer->Apply(0);

	m_shader->SetResource("tex_sampleTexture", m_sampleTarget);
	m_shader->SetFloat("negateChannelSamp", (float)m_inverted);
	m_shader->SetFloat4("activeChannels", m_activeSampleChannels);
	m_shader->Apply(0);
		
	m_deviceContext->Draw(6,0);

	//m_shader->SetResource("tex_sampleTexture", NULL);
	return S_OK;
}

EventInterface* ScreenAligned::getSwitchRenderSE()
{
	return m_eventListener;
}

void ScreenAligned::UpdateShaderResource(ID3D11ShaderResourceView* p_sampleTarget)
{
	m_sampleTarget = p_sampleTarget;
}