#pragma once

#include <iostream>
#include "StackElement.h"
#include "Node.h"
#include "Closure.h"

/*	Ein Stapel wie im Unterricht. Bei Fragen bitte an Herrn Kutschenreuter wenden ;)
*
*	Von dieser Klasse erben all jene Klassen, die verschiedene Sachen stapeln wollen.
*
*	Der Namesraum "DataStructure" ist der Struktur vorbehalten, da wir Struktur von Inhalt trennen wollen.
*/

namespace DataStructure
{
	//	TODO: Die Klasse für alle Datentypen sicher machen (und optimieren)

	template <typename T>
	class Stack
	{
	private:
		StackElement<T>* m_pTopNode;				//	Zeigt auf den obersten Knoten im Stapel. Das Ende brauchen wir ja nicht mehr als Attribut - Romolus ;)

	public:
		Stack();									//	Konstruktor
		~Stack();									//	Destruktor

		void push(T*);								//	Um einen Knoten aufzustapeln (entspricht "einfügen" aus dem Infounterricht)			
		void pop();									//	Um einen Knoten vom Stapel zu nehmen - Immer den ersten (entspricht "entfernen" aus dem Infounterricht)


		//	getter-Funktionen
		T* getTopNodeData()							//	Gibt das Datenelement des obersten Knotens zurück
		{
			return m_pTopNode->getData();
		}
		
	};

}

