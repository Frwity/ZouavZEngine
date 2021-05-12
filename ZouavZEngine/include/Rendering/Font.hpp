#pragma once

#include <string>
#include <unordered_map>

class Character
{
private:
	int id;
	float xTextureCoord;
	float yTextureCoord;
	float xMaxTextureCoord;
	float yMaxTextureCoord;
	float xOffset;
	float yOffset;
	float sizeX;
	float sizeY;
	float xAdvance;

public:
	Character(int _id, float _xTextureCoord, float _yTextureCoord, float _xTexSize, float _yTexSize,
		float _xOffset, float _yOffset, float _sizeX, float _sizeY, float _xAdvance);

	int GetId() { return id; }
	float GetxTextureCoord() { return xTextureCoord; }
	float GetyTextureCoord() { return yTextureCoord; }
	float GetXMaxTextureCoord() { return xMaxTextureCoord; }
	float GetYMaxTextureCoord() { return yMaxTextureCoord; }
	float GetxOffset() { return xOffset; }
	float GetyOffset() { return yOffset; }
	float GetSizeX() { return sizeX; }
	float GetSizeY() { return sizeY; }
	float GetxAdvance() { return xAdvance; }
};

class MetaFile 
{
private:
	static int PAD_TOP;
	static int PAD_LEFT;
	static int PAD_BOTTOM;
	static int PAD_RIGHT;

	static int DESIRED_PADDING;

	float verticalPerPixelSize;
	float horizontalPerPixelSize;
	float spaceWidth;
	int padding[4];
	int paddingWidth;
	int paddingHeight;

	std::unordered_map<int, Character> metaData;

	bool processNextLine();
	void loadCharacter(const std::string& line, int _imageSize);

public:
	MetaFile(const char* _path);
	
	float GetSpaceWidth() { return spaceWidth; }

	Character GetCharacter(int _ascii) { return metaData.at(_ascii); }
};