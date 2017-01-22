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
		T* m_pData;									//	Entspricht dem Attribut "daten" aus dem Infounterricht

	public:
		Node(T* pData);								//	Sack
		~Node();									//	Konstruktor
		//Node(const Node<T>&)						//	Kopierkonstruktor

		StackElement<T>* push(T*);					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)

		//	getter-Funktionen
		StackElement<T>* getSuccessor();			//	Gibt seinen Nachfolger zurück
		T* getData();								//	Gibt sein Datenelement zurück

		//	setter-Funktionen
		void setSuccessor(StackElement<T>*);

		//	killer-Funktionen
		void destroy();				//	#selbstmord
		void destroyAll();								//	#massenselbstmord
	};
}