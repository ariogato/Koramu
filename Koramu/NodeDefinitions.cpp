#include "NodeDeclarations.h"

template <typename T>
DataStructure::Node<T>::Node(T* pData)							//	Konstruktor
	: m_pData(pData), m_pSuccessor(nullptr)						//	Elementinitialisierer
{}

template <typename T>
DataStructure::Node<T>::~Node()									//	Destruktor
{	
	/*	Hier wird nur das Datenelement des Knotens gelöscht,
	*	da das Löschen von "m_pSuccessor" den Nachfolger
	*	ebenfalls löschen würde und somit die Struktur des Stapels
	*	komplett zerstört werden würde.
	*/
	delete m_pData;
}

template <typename T>
DataStructure::StackElement<T>* DataStructure::Node<T>::push(T* pNewNodeData)
{
	//	Neuen Knoten mit den einzufügenden Daten erstellen
	Node<T>* newNode = new Node<T>(pNewNodeData);

	//	Dieser Knoten (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zurückgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}

template <typename T>
DataStructure::StackElement<T>* DataStructure::Node<T>::getSuccessor()
{
	return m_pSuccessor;
}

template <typename T>
T* DataStructure::Node<T>::getData()
{
	return m_pData;
}

template <typename T>
void DataStructure::Node<T>::setSuccessor(StackElement<T>* pNewSuccessor)
{
	m_pSuccessor = pNewSuccessor;
}

template <typename T>
void DataStructure::Node<T>::destroy()
{
	delete this;
}

template <typename T>
void DataStructure::Node<T>::destroyAll()
{
	//	rekursiver Aufruf der Methode bei allen Stapelelementen (Aufruf zum Selbstmord)
	m_pSuccessor->destroyAll();

	//	der Knoten begeht Selbstmord 	
	delete this;
}
