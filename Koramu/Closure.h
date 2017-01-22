#pragma once

#include "StackElement.h"
#include "Node.h"

/*	Teil unseres Stapels. Entspricht dem "Abschluss" aus dem Infounterricht.
*	Erbt von StackElement.
*	Ist Teil unseres Kompositums.
*/

namespace DataStructure
{
	template <typename T>
	class Closure : public StackElement<T>
	{
	public:
		Closure() {}
		~Closure() {}

		StackElement<T>* push(T* pNewNodeData)					//	Methode zum aufstapeln von neuen Knoten (entspricht "einf�gen" aus dem Infounterricht)
		{
			//	Neuen Knoten mit den einzuf�genden Daten erstellen
			Node<T>* newNode = new Node<T>(pNewNodeData);

			//	Der Abschluss (aktuell oberstes Stapelelement) wird als Nachfolger des neuen Knotens gesetzt
			newNode->setSuccessor(this);

			//	Der neue Knoten wird zur�ckgegeben und damit zum obersten Stapelelement (m_pTopNode)
			return newNode;
		}

		//	getter-Funktionen
		StackElement<T>* getSuccessor() { return this; }		//	Gibt sich selber zur�ck (Bei Fragen: An Romolus oder Ario wenden)
		T* getData() { return nullptr; }					//	Der Abschluss hat keine Daten, deshalb nullptr

		//	killer-Funktionen
		void destroy() {}
		void destroyAll() { delete this; }
	};
}