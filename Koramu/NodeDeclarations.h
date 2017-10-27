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
	//	Klassen-Template. F�r Erkl�rungen siehe "StackDeclarations.h"
	template <typename T>
	class Node : public StackElement<T>
	{
	private:
		StackElement<T>* m_pSuccessor;							//	Entspricht dem Attribut "nachfolger" aus dem Infounterricht
		T m_data;												//	Entspricht dem Attribut "daten" aus dem Infounterricht 
																//	Dank der Implementierung des Stapels als Klassentemplate, k�nnen beliebige Datentypen verwaltet werden

	public:
		Node(T data);											//	Konstruktor (R.I.P Sack ... you won't be forgotten)
		~Node();												//	Destruktor

		StackElement<T>* push(T) override;						//	Methode zum aufstapeln von neuen Knoten (entspricht "einf�gen" aus dem Infounterricht)
		unsigned int size() override;							//	Gibt die Anzahl der auf dem Stack befindlichen Knoten zur�ck

		//	getter-Funktionen
		StackElement<T>* getElement(int index) override;		//	Gibt das Element an 'index' zur�ck, wobei 0 das erste und Stack<T>::size() das letzte Element ist
		StackElement<T>* getSuccessor() override;				//	Gibt den Nachfolger des Knotens zur�ck
		T& getData() override;									//	Gibt das Datenelement des Knotens zur�ck

		//	setter-Funktionen
		void setSuccessor(StackElement<T>* pNewSuccessor);		//	Den Nachfolger eines Knotens setzen

		//	killer-Funktionen
		void destroy() override;								//	#selbstmord
		void destroyAll() override;								//	#massenselbstmord
	};
}