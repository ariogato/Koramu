#include "Closure.h"

DataStructure::Closure::Closure()
{
	//	Unnötiger Konstruktor
}

DataStructure::Closure::~Closure()
{
	//	Unnötiger Destruktor
}

DataStructure::StackElement* DataStructure::Closure::push(Data* pNewNodeData)
{
	//	Neuen Knoten mit den einzufügenden Daten erstellen
	Node* newNode = new Node(pNewNodeData);

	//	Der Abschluss (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zurückgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}
