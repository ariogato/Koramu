#pragma once

/*	Teil unseres Stapels. Entspricht dem "Stapelelement" aus dem Infounterricht.
*	
*	Von dieser Klasse erben "Abschluss"("Closure") und "Knoten"("Node").
*	Wir verwenden hier also das Entwurfsmuster "Kompositum". 
*	Für weiteres siehe Klassendiagramm.
*/

namespace DataStructure
{
	//	Klassen-Template. Für Erklärungen siehe "StackDeclarations.h"
	template <typename T>
	class StackElement
	{
	public:
		StackElement() {}								//	Konstruktor
		virtual ~StackElement() {}						//	Destruktor

		virtual StackElement<T>* push(T) = 0;			//	Abstrakte Methode zum aufstapeln von neuen Knoten (entspricht "einfügen" aus dem Infounterricht)
		virtual unsigned int size() = 0;				//	Gibt die Anzahl der auf dem Stack befindlichen Knoten zurück

		//	getter-Funktionen
		virtual StackElement<T>* getElement(int) = 0;	//	Gibt das Element an 'index' zurück, wobei 0 das erste und Stack<T>::size() das letzte Element ist
		virtual StackElement<T>* getSuccessor() = 0;	//	Gibt den Nachfolger eines Knotens zurück
		virtual T& getData() = 0;						//	Gibt das Datenelement eines Knotens zurück
	
		//	killer-Funktionen
		virtual void destroy() = 0;						//	Zerstört/löscht einen einzelnen Knoten (der Abschluss soll hier nicht gelöscht werden können)	#selbstmord
		virtual void destroyAll() = 0;					//	Zerstört/löscht alle Stapelemente	#massenselbstmord
	};
}