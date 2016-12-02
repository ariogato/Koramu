#include "TextureManager.h"

//	Wichtig für Singleton-Klasse

TextureManager* TextureManager::s_pInstance = nullptr;

TextureManager::TextureManager()	//	Konstruktor
{
}

TextureManager::~TextureManager()	//	Destruktor
{
	/*
	*	Sobald man mindestens ein dynamisch alloziiertes Objekt
	*	(Schema: "pointer = new class()") als Member-Variable hat
	*	MUSS MUSS MUSS man einen Destruktor schreiben,
	*	der diese löscht. Sonst entsteht ein übles Speicherleck!!!
	*/

	//	Destruktor bitte befüllen, falls obiges zutrifft
}


bool TextureManager::load(std::string id, std::string fileName, SDL_Renderer* pRenderer)
{
	//	muss noch befüllt werden
	
	return false;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer)
{
	//	muss noch befüllt werden
}

void TextureManager::clearFromTextureMap(std::string id) 
{
	//	muss noch befüllt werden
}


//	Wichtig für Singleton Klasse
TextureManager* TextureManager::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (s_pInstance == nullptr)
		s_pInstance = new TextureManager();

	//	Instanz zurückgeben
	return s_pInstance;
}

//	Wichtig für Singleton-Klasse
void TextureManager::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}
