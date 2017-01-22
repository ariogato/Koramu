#pragma once

#include "StackElement.h"

/*	Teil unseres Stapels. Entspricht dem "Knoten" aus dem Infounterricht.
*	Erbt von StackElement.
*	Ist Teil unseres Kompositums.
*
*	Hier spielt sich fast die ganze Stapel-Magie ab.
*/

namespace DataStructure
{
	template <typename T>
	class Node : public StackElement<T>
	{
	private:
		StackElement<T>* m_pSuccessor;				//	Entspricht dem Attribut "nachfolger" aus dem Infounterricht
		T* m_pData;							//	Entspricht dem Attribut "daten" aus dem Infounterricht

	public:
		Node(T* pData)								//	Sack
			: m_pData(pData), m_pSuccessor(nullptr)
		{}
		~Node()										//	Konstruktor
		{
			/*	Hier wird nur das Datenelement des Knotens gelöscht,
			*	da das Löschen von "m_pSuccessor" den Nachfolger
			*	ebenfalls löschen würde und somit die Struktur des Stapels
			*	komplett zerstört werden würde.
			*/
			delete m_pData;
		}

		StackElement<T>* push(T* pNewNodeData)					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)
		{
			//	Neuen Knoten mit den einzufügenden Daten erstellen
			Node<T>* newNode = new Node<T>(pNewNodeData);

			//	Dieser Knoten (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
			newNode->setSuccessor(this);

			//	Der neue Knoten wird zurückgegeben und damit zum obersten Stapelelement (m_pTopNode)
			return newNode;
		}

		//	getter-Funktionen
		StackElement<T>* getSuccessor() { return m_pSuccessor; }		//	Gibt seinen Nachfolger zurück
		T* getData() { return m_pData; }							//	Gibt sein Datenelement zurück

		//	setter-Funktionen
		void setSuccessor(StackElement<T>* pNewSuccessor) { m_pSuccessor = pNewSuccessor; }

	};
}