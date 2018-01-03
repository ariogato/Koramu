#pragma once

#include "StackElement.h"

/*	Teil unseres Stapels. Entspricht dem "Abschluss" aus dem Infounterricht.
*	Erbt von StackElement.
*	Ist Teil unseres Kompositums.
*/

namespace DataStructure
{
	//	Klassen-Template. Für Erklärungen siehe "StackDeclarations.h"
	template <typename T>
	class Closure : public StackElement<T>
	{
	public:
		Closure() {}													//	Konstruktor
		~Closure() {}													//	Destruktor

		StackElement<T>* push(T) override;								//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)
		int size() override { return 0; }								//	Gibt die Anzahl der auf dem Stack befindlichen Knoten zurück


		//	getter-Funktionen
		StackElement<T>* getElement(int index) override;				//	Gibt das Element an 'index' zurück, wobei 0 das erste und Stack<T>::size() das letzte Element ist
		StackElement<T>* getSuccessor() override { return this; }		//	Gibt sich selber zurück (Bei Fragen: An Romolus oder Ario wenden)
		T& getData() override;											//	Der Abschluss hat keine Daten, deshalb wirft er eine exception

		//	killer-Funktionen
		void destroy() override {}										//	Hier soll nichts passieren. Der Abschluss darf nicht gelöscht werden, solange der Stapel an sich noch bestehen soll
		void destroyAll() override { delete this; }						//	#massenselbstmord	Der ganze Stapel wird gelöscht, also kann auch der Abschluss weg
	};
}