#include "NodeDeclarations.h"

template <typename T>
DataStructure::Node<T>::Node(T data)							//	Konstruktor
	: m_pSuccessor(nullptr), m_data(data)						//	Elementinitialisierer
{}

template <typename T>
DataStructure::Node<T>::~Node()									//	Destruktor
{	
	/*	Nicht zu löschende Pointer:
	 *		m_pSuccessor
	 *			Das Löschen von "m_pSuccessor" würde den Nachfolger
	 *			ebenfalls löschen und somit die Struktur des Stapels
	 *			komplett zerstören.
	 */
}

template <typename T>
DataStructure::StackElement<T>* DataStructure::Node<T>::push(T pNewNodeData)
{
	//	Neuen Knoten mit den einzufügenden Daten erstellen
	Node<T>* newNode = new Node<T>(pNewNodeData);

	//	Dieser Knoten (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
	newNode->setSuccessor(this);

	//	Der neue Knoten wird zurückgegeben und damit zum obersten Stapelelement (m_pTopNode)
	return newNode;
}

template <typename T>
unsigned DataStructure::Node<T>::size()
{
	/*	Der vom Nachfolger zurückgegebene Wert wird immer um 1 inkrementiert.
	 *	Mit dem rekursiven Aufruf dieser Methode inkrementiert jeder Knoten (außer der Abschluss)
	 *	auf dem Stapel den Wert um 1 und man kriegt am Ende die Anzahl der Knoten.
	 */
	return m_pSuccessor->size() + 1;
}

template <typename T>
DataStructure::StackElement<T>* DataStructure::Node<T>::getElement(int index)
{
	//	Sobald der index 0 ist, soll der Knoten zurückgegeben werden (siehe )
	if (index == 0)
		return this;

	//	Ansonsten soll der rekursive Aufruf weitergeführt werden
	return m_pSuccessor->getElement(index - 1);
}

template <typename T>
DataStructure::StackElement<T>* DataStructure::Node<T>::getSuccessor()
{
	return m_pSuccessor;
}

template <typename T>
T& DataStructure::Node<T>::getData()
{
	return m_data;
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
