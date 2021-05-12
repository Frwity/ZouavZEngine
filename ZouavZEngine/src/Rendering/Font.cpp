#include "Rendering/Font.hpp"

#include <fstream>
#include <iostream>

int MetaFile::PAD_TOP = 0;
int MetaFile::PAD_LEFT = 1;
int MetaFile::PAD_BOTTOM = 2;
int MetaFile::PAD_RIGHT = 3;
int MetaFile::DESIRED_PADDING = 3;

Character::Character(int _id, float _xTextureCoord, float _yTextureCoord, float _xTexSize, float _yTexSize,
	float _xOffset, float _yOffset, float _sizeX, float _sizeY, float _xAdvance)
	: id(_id), xTextureCoord(_xTextureCoord), yTextureCoord(_yTextureCoord), xOffset(_xOffset), yOffset(_yOffset),
	sizeX(_sizeX), sizeY(_sizeY), xMaxTextureCoord(_xTexSize + xTextureCoord), yMaxTextureCoord(_yTexSize + yTextureCoord), xAdvance(_xAdvance)
{}

int GetValueOf(const std::string& _line, const std::string& _str, const char _separator)
{
	size_t startPos = _line.find(_str, 0) + _str.size() + 1;
	size_t endPos = _line.find(_separator, startPos);
	return std::stoi(_line.substr(startPos, endPos - startPos));
}

MetaFile::MetaFile(const char* _path)
{
	std::fstream file(_path);
	std::string line;
	std::getline(file >> std::ws, line);

	padding[PAD_TOP] = GetValueOf(line, "padding", ',');
	std::string _tempString = std::to_string(padding[PAD_TOP]);
	padding[PAD_LEFT] = GetValueOf(line, "padding=" + _tempString, ',');
	_tempString += "," + std::to_string(padding[PAD_LEFT]);
	padding[PAD_BOTTOM] = GetValueOf(line, "padding=" + _tempString, ',');
	_tempString += "," + std::to_string(padding[PAD_BOTTOM]);
	padding[PAD_RIGHT] = GetValueOf(line, "padding", ' ');

	paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
	paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];

	std::getline(file >> std::ws, line);

	int lineHeightPixels = GetValueOf(line, "lineHeight", ' ') - paddingHeight;
	verticalPerPixelSize = 0.03f / lineHeightPixels; // TODO: look into the 0.03f
	horizontalPerPixelSize = verticalPerPixelSize / 1.f; // todo aspect ratio replace the 1.f

	int imageWidth = GetValueOf(line, "scaleW", ' ');

	std::getline(file >> std::ws, line);// skip useless line
	std::getline(file >> std::ws, line);
	int count = GetValueOf(line, "count", '\n');
	for (int i = 0; i < count; ++i)
	{
		getline(file >> std::ws, line);
		loadCharacter(line, imageWidth);
	}

	file.close();

	for (auto& cha : metaData)
		std::cout << cha.second.GetId() << " " << cha.second.GetSizeX() << " " << cha.second.GetSizeY() << " " << cha.second.GetyTextureCoord() << std::endl;

}

void MetaFile::loadCharacter(const std::string& _line, int _imageSize)
{
	int id = GetValueOf(_line, "id", ' ');
	
	if (id == 32)
	{
		spaceWidth = (GetValueOf(_line, "xadvance", ' ') - paddingWidth) * horizontalPerPixelSize;
		return;
	}
	
	float xTex = ((float)GetValueOf(_line, "x", ' ') + (padding[PAD_LEFT] - DESIRED_PADDING)) / _imageSize;
	float yTex = ((float)GetValueOf(_line, "y", ' ') + (padding[PAD_TOP] - DESIRED_PADDING)) / _imageSize;
	int width = GetValueOf(_line, "width", ' ') - (paddingWidth - (2 * DESIRED_PADDING));
	int height = GetValueOf(_line, "height", ' ') - ((paddingHeight)-(2 * DESIRED_PADDING));
	float quadWidth = width * horizontalPerPixelSize;
	float quadHeight = height * verticalPerPixelSize;
	float xTexSize = (float)width / _imageSize;
	float yTexSize = (float)height / _imageSize;
	float xOff = (GetValueOf(_line, "xoffset", ' ') + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
	float yOff = (GetValueOf(_line, "yoffset", ' ') + (padding[PAD_TOP] - DESIRED_PADDING)) * verticalPerPixelSize;
	float xAdvance = (GetValueOf(_line, "xadvance", ' ') - paddingWidth) * horizontalPerPixelSize;

	metaData.emplace(id, Character(id, xTex, yTex, xTexSize, yTexSize, xOff, yOff, quadWidth, quadHeight, xAdvance));
}
