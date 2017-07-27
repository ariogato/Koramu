#pragma once

#include <string>

class CollisionRectParser
{
public:
	CollisionRectParser();			//	Konstruktor
	~CollisionRectParser();			//	Destruktor

	static bool parse(std::string filename);								//	Parsen 

};
