#include "StackDeclarations.h"

template <typename T>
DataStructure::Stack<T>::Stack()
	: m_pTopNode(new Closure<T>())
{}

template <typename T>
DataStructure::Stack<T>::~Stack()
{
	m_pTopNode->destroyAll();
}

template <typename T>
void DataStructure::Stack<T>::push(T* pNewNodeData)
{
	//	If you can't see right through Hillary your brain needs to evolve
	m_pTopNode = m_pTopNode->push(pNewNodeData);
}

template <typename T>
void DataStructure::Stack<T>::pop()
{
	/*	Wir speichern eine Referenz auf den alten obersten Knoten,
	*	damit wir nach der Ablösung dieses den entsprechenden Speicherplatz
	*	freigeben können, um ein Daten-Leck zu verhindern.
	*/
	StackElement<T>* tempTop = m_pTopNode;

	/*	Der neue oberste Knoten ist der Nachfolger des alten obersten Knotens,
	*	welcher entfernt wird.
	*	Dies funktioniert auch mit dem Abschluss als obersten Knoten,
	*	da dieser in "getSuccessor" eine Referenz auf sich selbst zurückgibt.
	*/
	m_pTopNode = m_pTopNode->getSuccessor();

	/*	Hier wird der Speicherplatz des alten obersten Knoten freigegeben.
	*	Dies darf jedoch nur bei Objekten der Klasse "Node" (und nicht "Closure") geschehen,
	*	weshalb die Methode "destroy()", die bei "Node"s den #selbstmord auslöst
	*/
	tempTop->destroy();
}
