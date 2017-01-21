#include "Node.h"

DataStructure::Node::Node(Data* pData)
	//	Initialisierungsliste
	: m_pData(pData), m_pSuccessor(nullptr)
{
}

DataStructure::Node::~Node()
{
	/*	Hier wird nur das Datenelement des Knotens gel�scht,
	*	da das L�schen von "m_pSuccessor" den Nachfolger 
	*	ebenfalls l�schen w�rde und somit die Struktur des Stapels
	*	komplett zerst�rt werden w�rde.
	*/

	delete m_pData;
}

DataStructure::StackElement * DataStructure::Node::push(Data* pNewNodeData)
{
	//	Neuen Knoten mit den einzuf�genden Daten erstellen
	Node* newNode = new Node(pNewNodeData);

	//	Dieser Knoten (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zur�ckgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}
