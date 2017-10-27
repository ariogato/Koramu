#include "ClosureDeclarations.h"
#include "Node.h"

template <typename T>
DataStructure::StackElement<T>* DataStructure::Closure<T>::push(T pNewNodeData)
{
	//	Neuen Knoten mit den einzufügenden Daten erstellen
	Node<T>* newNode = new Node<T>(pNewNodeData);

	//	Der Abschluss (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zurückgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}

template <typename T>
DataStructure::StackElement<T>* DataStructure::Closure<T>::getElement(int index)
{
	return this;
}

template <typename T>
T& DataStructure::Closure<T>::getData()
{
	//	Wirft eine Exception
	throw std::out_of_range("stack out of range");
}
