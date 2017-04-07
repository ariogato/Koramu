#include "ClosureDeclarations.h"
#include "Node.h"

template <typename T>
DataStructure::StackElement<T>* DataStructure::Closure<T>::push(T* pNewNodeData)
{
	//	Neuen Knoten mit den einzuf�genden Daten erstellen
	Node<T>* newNode = new Node<T>(pNewNodeData);

	//	Der Abschluss (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zur�ckgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}