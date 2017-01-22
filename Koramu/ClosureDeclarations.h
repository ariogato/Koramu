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
		Closure() {}								//	Konstruktor
		~Closure() {}								//	Destruktor

		StackElement<T>* push(T*);					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)


		//	getter-Funktionen
		StackElement<T>* getSuccessor() { return this; }		//	Gibt sich selber zurück (Bei Fragen: An Romolus oder Ario wenden)
		T* getData() { return nullptr; }						//	Der Abschluss hat keine Daten, deshalb nullptr

		//	killer-Funktionen
		void destroy() {}
		void destroyAll() { delete this; }
	};
}