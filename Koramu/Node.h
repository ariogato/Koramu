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
			/*	Hier wird nur das Datenelement des Knotens gel�scht,
			*	da das L�schen von "m_pSuccessor" den Nachfolger
			*	ebenfalls l�schen w�rde und somit die Struktur des Stapels
			*	komplett zerst�rt werden w�rde.
			*/
			delete m_pData;
		}

		StackElement<T>* push(T* pNewNodeData)					//	Methode zum aufstapeln von neuen Knoten (entspricht "einf�gen" aus dem Infounterricht)
		{
			//	Neuen Knoten mit den einzuf�genden Daten erstellen
			Node<T>* newNode = new Node<T>(pNewNodeData);

			//	Dieser Knoten (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
			newNode->setSuccessor(this);

			//	Der neue Knoten wird zur�ckgegeben und damit zum obersten Stapelelement (m_pTopNode)
			return newNode;
		}

		//	getter-Funktionen
		StackElement<T>* getSuccessor() { return m_pSuccessor; }		//	Gibt seinen Nachfolger zur�ck
		T* getData() { return m_pData; }							//	Gibt sein Datenelement zur�ck

		//	setter-Funktionen
		void setSuccessor(StackElement<T>* pNewSuccessor) { m_pSuccessor = pNewSuccessor; }

	};
}