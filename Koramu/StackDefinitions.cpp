#include "StackDeclarations.h"
#include "Node.h"
#include "Closure.h"

template <typename T>
DataStructure::Stack<T>::Stack()
	: m_pTopNode(new Closure<T>())				//	Elementinitialisierer 
{}

template <typename T>
DataStructure::Stack<T>::~Stack()
{
	//	Der gesamte Stapel soll gelöscht (nicht nur der oberste Knoten) und damit der Speicherplatz freigegeben werden
	m_pTopNode->destroyAll();	
}

template <typename T>
void DataStructure::Stack<T>::push(T newNodeData)
{
	//	If you can't see right through Hillary your brain needs to evolve
	m_pTopNode = m_pTopNode->push(newNodeData);
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

template <typename T>
void DataStructure::Stack<T>::clear()
{
	//	So lange der Stapel nicht leer ist soll gepoppt werden
	while (this->empty())
	{
		this->pop();
	}
}

template <typename T>
int DataStructure::Stack<T>::size()
{
	//	Rekursiver Aufruf der Methode
	return m_pTopNode->size();
}

template <typename T>
bool DataStructure::Stack<T>::empty()
{
	//	Sehr naiv...
	return !(m_pTopNode->size());
}

template <typename T>
T& DataStructure::Stack<T>::at(int index)
{
	/*	Ruft die Methode rekursiv bei allen Knoten auf.
	 *	Jeder Knoten verringert 'index' um eins, sodass - sobald index == 0 zutrifft -
	 *	der jeweilige Knoten sein Datenelement zurückgeben kann.
	 */
	return m_pTopNode->getElement(index)->getData();
}

template <typename T>
T& DataStructure::Stack<T>::operator[](int index)
{
	return this->at(index);
}
