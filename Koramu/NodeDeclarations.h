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
	//	Klassen-Template. Für Erklärungen siehe "StackDeclarations.h"
	template <typename T>
	class Node : public StackElement<T>
	{
	private:
		StackElement<T>* m_pSuccessor;				//	Entspricht dem Attribut "nachfolger" aus dem Infounterricht
		T* m_pData;									//	Entspricht dem Attribut "daten" aus dem Infounterricht 
													//	Dank der Implementierung des Stapels als Klassentemplate, können beliebige Datentypen verwaltet werden

	public:
		Node(T* pData);								//	Konstruktor (R.I.P Sack ... you won't be forgotten)
		~Node();									//	Destruktor
		//Node(const Node<T>&)						//	Kopierkonstruktor

		StackElement<T>* push(T*);					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)

		//	getter-Funktionen
		StackElement<T>* getSuccessor();			//	Gibt den Nachfolger des Knotens zurück
		T* getData();								//	Gibt das Datenelement des Knotens zurück

		//	setter-Funktionen
		void setSuccessor(StackElement<T>* pNewSuccessor);		//	Den Nachfolger eines Knotens setzen

		//	killer-Funktionen
		void destroy();								//	#selbstmord
		void destroyAll();							//	#massenselbstmord
	};
}