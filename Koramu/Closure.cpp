#include "Closure.h"

DataStructure::Closure::Closure()
{
	//	Unn�tiger Konstruktor
}

DataStructure::Closure::~Closure()
{
	//	Unn�tiger Destruktor
}

DataStructure::StackElement* DataStructure::Closure::push(Data* pNewNodeData)
{
	//	Neuen Knoten mit den einzuf�genden Daten erstellen
	Node* newNode = new Node(pNewNodeData);

	//	Der Abschluss (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zur�ckgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}
