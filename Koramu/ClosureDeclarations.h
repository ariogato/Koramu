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
		Closure() {}								//	Konstruktor
		~Closure() {}								//	Destruktor

		StackElement<T>* push(T*);					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)


		//	getter-Funktionen
		StackElement<T>* getSuccessor() { return this; }		//	Gibt sich selber zurück (Bei Fragen: An Romolus oder Ario wenden)
		T* getData() { return nullptr; }						//	Der Abschluss hat keine Daten, deshalb nullptr

		//	killer-Funktionen
		void destroy() {}										//	Hier soll nichts passieren. Der Abschluss darf nicht gelöscht werden, solange der Stapel an sich noch bestehen soll
		void destroyAll() { delete this; }						//	#massenselbstmord	Der ganze Stapel wird gelöscht, also kann auch der Abschluss weg
	};
}