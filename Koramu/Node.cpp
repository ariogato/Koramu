#include "Node.h"

DataStructure::Node::Node(Data* pData)
	//	Initialisierungsliste
	: m_pData(pData), m_pSuccessor(nullptr)
{
}

DataStructure::Node::~Node()
{
	/*	Hier wird nur das Datenelement des Knotens gelöscht,
	*	da das Löschen von "m_pSuccessor" den Nachfolger 
	*	ebenfalls löschen würde und somit die Struktur des Stapels
	*	komplett zerstört werden würde.
	*/

	delete m_pData;
}

DataStructure::StackElement * DataStructure::Node::push(Data* pNewNodeData)
{
	//	Neuen Knoten mit den einzufügenden Daten erstellen
	Node* newNode = new Node(pNewNodeData);

	//	Dieser Knoten (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zurückgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}
