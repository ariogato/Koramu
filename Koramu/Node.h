#pragma once

#include "StackElement.h"
#include "Data.h"

/*	Teil unseres Stapels. Entspricht dem "Knoten" aus dem Infounterricht.
*	Erbt von StackElement.
*	Ist Teil unseres Kompositums.
*
*	Hier spielt sich fast die ganze Stapel-Magie ab.
*/

namespace DataStructure
{
	class Node : public StackElement
	{
	private:
		StackElement* m_pSuccessor;				//	Entspricht dem Attribut "nachfolger" aus dem Infounterricht
		Data* m_pData;							//	Entspricht dem Attribut "daten" aus dem Infounterricht

	public:
		Node(Data*);				//	Sack
		~Node();					//	Konstruktor

		StackElement* push(Data*);					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)

		//	getter-Funktionen
		StackElement* getSuccessor() { return m_pSuccessor; }		//	Gibt seinen Nachfolger zurück
		Data* getData() { return m_pData; }							//	Gibt sein Datenelement zurück

		//	setter-Funktionen
		void setSuccessor(StackElement* pNewSuccessor) { m_pSuccessor = pNewSuccessor; }
	};
}