#pragma once

#include "StackElement.h"
#include "Node.h"
#include "Data.h"

/*	Teil unseres Stapels. Entspricht dem "Abschluss" aus dem Infounterricht.
*	Erbt von StackElement.
*	Ist Teil unseres Kompositums.
*/

namespace DataStructure
{
	class Closure : public StackElement
	{
	public:
		Closure();
		~Closure();

		StackElement* push(Data*);					//	Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)

		//	getter-Funktionen
		StackElement* getSuccessor() { return this; }		//	Gibt sich selber zurück (Bei Fragen: An Romolus oder Ario wenden)
		Data* getData() { return nullptr; }					//	Der Abschluss hat keine Daten, deshalb nullptr
	};
}