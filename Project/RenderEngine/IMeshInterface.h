#pragma once


class IMeshInterface
{
public:
	virtual size_t GetVertexCount() = 0;
	virtual unsigned int GetVAO() = 0;
	virtual size_t GetIndexSize() = 0;
	virtual unsigned int GetType() = 0;
	virtual size_t GetTextureSize() = 0;
	virtual const char* GetTextureName(size_t index) = 0;
	virtual unsigned int GetTextureId(size_t index) = 0;
	virtual bool IsPositionEnable() = 0;
	virtual bool IsColorEnable() = 0;
	virtual bool IsNormalEnable() = 0;
	virtual bool IsTexCoordsEnable() = 0;
	virtual bool IsTangentEnable() = 0;
	virtual bool IsBitangentEnable() = 0;
};