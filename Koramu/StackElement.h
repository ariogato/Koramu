#pragma once

/*	Teil unseres Stapels. Entspricht dem "Stapelelement" aus dem Infounterricht.
*	
*	Von dieser Klasse erben "Abschluss"("Closure") und "Knoten"("Node").
*	Wir verwenden hier also das Entwurfsmuster "Kompositum". 
*	F�r weiteres siehe Klassendiagramm.
*/

namespace DataStructure
{
	template <typename T>
	class StackElement
	{
	public:
		virtual StackElement<T>* push(T*) = 0;			//	Abstrakte Methode zum aufstapeln von neuen Knoten (entspricht "einf�gen" aus dem Infounterricht)

		//	getter-Funktionen
		virtual StackElement<T>* getSuccessor() = 0;	//	Gibt den Nachfolger eines Knotens zur�ck
		virtual T* getData() = 0;					//	Gibt das Datenelement eines Knotens zur�ck
	
		//	killer-Funktionen
		virtual void destroy() = 0;
		virtual void destroyAll() = 0;
	};
}