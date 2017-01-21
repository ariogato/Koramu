#pragma once

#include "Data.h"

/*	Teil unseres Stapels. Entspricht dem "Stapelelement" aus dem Infounterricht.
*	
*	Von dieser Klasse erben "Abschluss"("Closure") und "Knoten"("Node").
*	Wir verwenden hier also das Entwurfsmuster "Kompositum". 
*	Für weiteres siehe Klassendiagramm.
*/

namespace DataStructure
{
	class StackElement
	{
	public:
		virtual StackElement* push(Data*) = 0;			//	Abstrakte Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)

		//	getter-Funktionen
		virtual StackElement* getSuccessor() = 0;	//	Gibt den Nachfolger eines Knotens zurück
		virtual Data* getData() = 0;				//	Gibt das Datenelement eines Knotens zurück
	};
}