// ------------------------------------------------------------------
// Lukas Beinlich, 22.09.2014
// Font2.cpp
// Meine eigene Font-Engine
// ------------------------------------------------------------------

#include "src\Graphics\Font\Font2.h"
//#include "Game\Options.h"

#include <fstream>
using namespace std;


CFont2::CFont2(void)
{
	m_pFont=NULL;
	m_pTexture=NULL;
}

CFont2::~CFont2(void)
{
}

bool CFont2::LoadFontData(char *fontfilename)
{
	ifstream fin;
	char temp;
	int count=0;

	fin.open(fontfilename);
	if(fin.fail())
		return false;
	fin>>count;

	m_pFont=new FontType[count];
	if(!m_pFont)
		return false;

	for(int i=0;i<count;i++)
	{
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);
		fin>>m_pFont[i].left;
		fin>>m_pFont[i].right;
		fin>>m_pFont[i].size;
	}
	fin.close();
	return true;
}

bool CFont2::LoadTexture(ID3D11Device *device,WCHAR *texturefilename)
{
	m_pTexture=new CTexture;
	if(!m_pTexture)
		return false;
	if(!m_pTexture->Init(device,texturefilename))
		return false;
	return true;
}

void CFont2::ReleaseTexture(void)
{
	if(m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture=NULL;
	}
}


void CFont2::ReleaseFontData(void)
{
	if(m_pFont)
	{
		delete[] m_pFont;
		m_pFont=NULL;
	}
}

bool CFont2::Init(ID3D11Device *device,ID3D11DeviceContext *devicecontext,HWND hWnd,XMMATRIX baseviewmatrix,char *fontfilename,WCHAR *texturefilename)
{
	m_BaseViewMatrix=baseviewmatrix;
	m_pDevice = device;
	m_pDeviceContext = devicecontext;
	if(!LoadFontData(fontfilename))
		return false;
	if(!LoadTexture(device,texturefilename))
		return false;
	m_pShader=new CSRecolorTexture();
	if(!m_pShader)
		return false;
	if(!m_pShader->Init(device,hWnd))
		return false;
	return true;
}

void CFont2::Shutdown(void)
{
	ReleaseTexture();
	ReleaseFontData();
}

int CFont2::GetChar(char c)
{
	int letter=((int)c);
	if(letter == 'Ä')
		letter = 127;
	else if(letter == 'Ö')
		letter = 128;
	else if(letter == 'Ü')
		letter = 129;
	else if(letter == 'ä')
		letter = 130;
	else if(letter == 'ö')
		letter = 131;
	else if(letter == 'ü')
		letter = 132;
	else if(letter == 'ß')
		letter = 133;
	letter -= 32;
	return letter;
}

bool CFont2::CreateSentence(SSentence *sentence,int posx,int posy,float r,float g,float b,float height,const char* text,int format,int sizex,int sizey)
{
	sentence->maxLength = 256;

	D3D11_BUFFER_DESC positionBufferDesc;
	positionBufferDesc.Usage=D3D11_USAGE_DYNAMIC;
	positionBufferDesc.ByteWidth=sizeof(PositionBufferType);
	positionBufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	positionBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	positionBufferDesc.MiscFlags=0;
	positionBufferDesc.StructureByteStride=0;
	if(m_pDevice->CreateBuffer(&positionBufferDesc,NULL,&(sentence->PositionArrayBuffer)))
		return false;
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PositionBufferType *dataPtr;

	if(FAILED(m_pDeviceContext->Map(sentence->PositionArrayBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	
	dataPtr = (PositionBufferType*)mappedResource.pData;

	memset(dataPtr,0,sizeof(PositionBufferType));

	m_pDeviceContext->Unmap(sentence->PositionArrayBuffer,0);


	VertexType* vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	sentence->vertexCount=4;
	sentence->indexCount=6;

	vertices = new VertexType [sentence->vertexCount];
	if(!vertices)
		return false;

	indices = new unsigned long[sentence->indexCount];
	if(!indices)
		return false;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	float left,right,top,bottom;

//	left=(float)((g_Options.GetScreenWidth()/2)*-1)+(float)000;
//	right=left+(float)300;
//	top=(float)((g_Options.GetScreenHeight()/2))-(float)495;
//	bottom=top-(float)100;

	left = 0;
	right = 200;
	top = 0;
	bottom = 200;


	// oben rechts
	vertices[0].position=XMFLOAT3(right,top,0.0f);
	vertices[0].texture=XMFLOAT2(1.0f,0.0f);
	// unten rechts
	vertices[1].position=XMFLOAT3(right,bottom,0.0f);
	vertices[1].texture=XMFLOAT2(1.0f,1.0f);
	// oben links
	vertices[2].position=XMFLOAT3(left,top,0.0f);
	vertices[2].texture=XMFLOAT2(0.0f,0.0f);
	// unten links
	vertices[3].position=XMFLOAT3(left,bottom,0.0f);
	vertices[3].texture=XMFLOAT2(0.0f,1.0f);



	vertexBufferDesc.Usage=D3D11_USAGE_DEFAULT; // Ich denke nicht, dass das ding i-wie verändert werden, da das im shader passiert
	vertexBufferDesc.ByteWidth=sizeof(VertexType)*sentence->vertexCount;
	vertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags=0;
	vertexBufferDesc.MiscFlags=0;
	vertexBufferDesc.StructureByteStride=0;

	vertexData.pSysMem=vertices;
	vertexData.SysMemPitch=0;
	vertexData.SysMemSlicePitch=0;
	
	if(FAILED(m_pDevice->CreateBuffer(&vertexBufferDesc,&vertexData,&(sentence->vertexBuffer))))
		return false;

	indexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth=sizeof(unsigned long)*sentence->indexCount;
	indexBufferDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags=0;
	indexBufferDesc.MiscFlags=0;
	indexBufferDesc.StructureByteStride=0;

	indexData.pSysMem=indices;
	indexData.SysMemPitch=0;
	indexData.SysMemSlicePitch=0;
	if(FAILED(m_pDevice->CreateBuffer(&indexBufferDesc,&indexData,&(sentence->indexBuffer))))
		return false;

	delete[] vertices;
	delete[] indices;
	vertices = NULL;
	indices = NULL;

	return UpdateSentence(sentence,posx,posy,r,g,b,height,text,format,sizex,sizey);
}

bool CFont2::UpdateSentence(SSentence *sentence,int posx,int posy,float r,float g,float b,float height,const char* text,int format,int sizex,int sizey)
{
	sentence->red=r;
	sentence->blue=b;
	sentence->green=g;
	sentence->height=height;
	sentence->posx=posx;
	sentence->posy=posy;
	sentence->formatlr=format%10;
	sentence->formatou=(format%100)-sentence->formatlr;
	sentence->sizex=sizex;
	sentence->sizey=sizey;
	sentence->text=text;
	if(sizex == -1)
		sentence->formatlr = FONT2_FORMAT_LEFT;
	if(sizey == -1)
		sentence->formatou = FONT2_FORMAT_TOP;


	float maxwidth=0;
	int numLetters = (int) strlen(text);
	float standardheight = 47.0f; // sollte eig nicht hardcodiert sein, aber was solls :)
	float aspectratio;
	for(int i=0;i<numLetters;i++)
	{
		int letter=GetChar(text[i]);
		aspectratio = m_pFont[letter].size / standardheight; // Space ist 8 Pixel breit... sollte auch nicht hardcodiert sein... aber :D
		maxwidth += aspectratio * sentence->height;
	}
	// Eckpunkte für die Position des Rechtecks
	// posx,posy,sizex,sizey
	// Eigentliche Größe der Schrift
	// height und maxwidth

	// Dies bedeutet Postion ist bei lr:
	// links: Posx + 0
	// rechts: Posx + sizex - maxwidth (wenn maxwidth > sizex -> Pech oder abschneiden)
	// mitte: Posx + (sizex - maxwidth) / 2 (siehe rechts) 

	// Dies bedeutet Position bei ou (analog zu lr):
	// oben: Posy + 0
	// unten: Posy + sizey - height
	// mitte: Posy + (sizey - height) / 2

	float basex;
	float basey;
	switch(sentence->formatlr)
	{
	case FONT2_FORMAT_LEFT:
		basex = sentence->posx;
		break;
	case FONT2_FORMAT_RIGHT:
		basex = sentence->posx + sentence->sizex - maxwidth;
		break;
	case FONT2_FORMAT_CENTER:
		basex = sentence->posx + (sentence->sizex - maxwidth) / 2;
		break;
	}

	switch(sentence->formatou)
	{
	case FONT2_FORMAT_TOP:
		basey = sentence->posy;
		break;
	case FONT2_FORMAT_BOTTOM:
		basey = sentence->posy + (sentence->sizey - sentence->height);
		break;
	case FONT2_FORMAT_VCENTER:
		basey = sentence->posy + (sentence->sizey - sentence->height) / 2;
		break;
	}

	// jetzt den position array buffer füllen

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PositionBufferType *dataPtr;

	if(FAILED(m_pDeviceContext->Map(sentence->PositionArrayBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;

	dataPtr = (PositionBufferType*)mappedResource.pData;

	memset(dataPtr,0,sizeof(PositionBufferType));

	sentence->instanceCount=0;
	int letter;
	float posoffset = 0.0f;
	float width;

	float left,right,top,bottom;

	sentence->instanceCount=0;

	for(int i=0;i<numLetters;i++)
	{
		letter=GetChar(text[i]);
		if(letter==0) // Space
		{
			aspectratio = m_pFont[letter].size / standardheight; // Space ist 8 Pixel breit... sollte auch nicht hardcodiert sein... aber :D
			posoffset += aspectratio * sentence->height;
			continue;
		}
		else
		{
			aspectratio = m_pFont[letter].size / standardheight;
			width = aspectratio * sentence->height;
		}

		left=(float)((/*g_Options.GetScreenWidth()*/800/2)*-1)+basex + posoffset;
		right=left+(float)width;
		top=(float)((/*g_Options.GetScreenHeight()*/600/2))-basey;
		bottom=top-(float)sentence->height;

		// Oben links
		dataPtr->Pos[sentence->instanceCount*4] = XMFLOAT4(right,top,m_pFont[letter].right,0.0f);
		dataPtr->Pos[sentence->instanceCount*4+1] = XMFLOAT4(right,bottom,m_pFont[letter].right,1.0f);
		dataPtr->Pos[sentence->instanceCount*4+2] = XMFLOAT4(left,top,m_pFont[letter].left,0.0f);
		dataPtr->Pos[sentence->instanceCount*4+3] = XMFLOAT4(left,bottom,m_pFont[letter].left,1.0f);
		
		posoffset += width;
		aspectratio = 2 / standardheight;
		width = aspectratio * sentence->height;
		posoffset += width;
		sentence->instanceCount++;
		if(sentence->instanceCount> sentence->maxLength)
		{
			sentence->instanceCount--;
			break;
		}
	}
	m_pDeviceContext->Unmap(sentence->PositionArrayBuffer,0);
	return true;
}

void CFont2::SetSentenceColor(SSentence *sentence,float r,float g,float b)
{
	sentence->red=r;
	sentence->blue=b;
	sentence->green=g;
}

bool CFont2::RenderSentence(SSentence *sentence,XMMATRIX worldmat,XMMATRIX projmat)
{
	unsigned int stride;
	unsigned int offset;
	XMFLOAT4 pixelColor;

	stride = sizeof(VertexType);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0,1,&(sentence->vertexBuffer),&stride,&offset);
	m_pDeviceContext->IASetIndexBuffer(sentence->indexBuffer,DXGI_FORMAT_R32G32B32A32_UINT,0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pixelColor=XMFLOAT4(sentence->red,sentence->green,sentence->blue,1.0f);
	if(!m_pShader->Render(m_pDeviceContext,sentence->indexCount,worldmat,m_BaseViewMatrix,projmat,m_pTexture->GetTexture(),pixelColor,sentence->PositionArrayBuffer,sentence->instanceCount))
		return false;
	return true;
}

void CFont2::FreeSentence(SSentence *sentence)
{
	// Do stuff, but not yet :D
}
