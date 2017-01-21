#include "Stack.h"

DataStructure::Stack::Stack()						//	Konstruktor
	//	Initialisierungsliste
	: m_pTopNode(new Closure())
{
}

DataStructure::Stack::~Stack()						//	Destruktor
{
	delete m_pTopNode;
}

void DataStructure::Stack::push(Data* pNewNodeData)
{
	//	If you can't see right through Hillary your brain needs to evolve
	m_pTopNode = m_pTopNode->push(pNewNodeData);
}

void DataStructure::Stack::pop()
{
	/*	Wir speichern eine Referenz auf den alten obersten Knoten,
	*	damit wir nach der Abl�sung dieses den entsprechenden Speicherplatz
	*	freigeben k�nnen, um ein Daten-Leck zu verhindern.
	*/
	StackElement* tempTop = m_pTopNode;

	/*	Der neue oberste Knoten ist der Nachfolger des alten obersten Knotens, 
	*	welcher entfernt wird.
	*	Dies funktioniert auch mit dem Abschluss als obersten Knoten,
	*	da dieser in "getSuccessor" eine Referenz auf sich selbst zur�ckgibt.
	*/
	m_pTopNode = m_pTopNode->getSuccessor();

	//	Hier wird der Speicherplatz des alten obersten Knoten freigegeben
	delete tempTop;
}

DataStructure::Data* DataStructure::Stack::getTopNodeData()
{
	return m_pTopNode->getData();
}
