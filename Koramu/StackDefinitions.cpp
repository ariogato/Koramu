#include "StackDeclarations.h"
#include "Node.h"
#include "Closure.h"
#include <iostream>

template <typename T>
DataStructure::Stack<T>::Stack()
	: m_pTopNode(new Closure<T>())				//	Elementinitialisierer 
{}

template <typename T>
DataStructure::Stack<T>::~Stack()
{
	//	Der gesamte Stapel soll gel�scht (nicht nur der oberste Knoten) und damit der Speicherplatz freigegeben werden
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
	*	damit wir nach der Abl�sung dieses den entsprechenden Speicherplatz
	*	freigeben k�nnen, um ein Daten-Leck zu verhindern.
	*/
	StackElement<T>* tempTop = m_pTopNode;

	/*	Der neue oberste Knoten ist der Nachfolger des alten obersten Knotens,
	*	welcher entfernt wird.
	*	Dies funktioniert auch mit dem Abschluss als obersten Knoten,
	*	da dieser in "getSuccessor" eine Referenz auf sich selbst zur�ckgibt.
	*/
	m_pTopNode = m_pTopNode->getSuccessor();

	/*	Hier wird der Speicherplatz des alten obersten Knoten freigegeben.
	*	Dies darf jedoch nur bei Objekten der Klasse "Node" (und nicht "Closure") geschehen,
	*	weshalb die Methode "destroy()", die bei "Node"s den #selbstmord ausl�st
	*/
	tempTop->destroy();
}
