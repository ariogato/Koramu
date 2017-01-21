#pragma once

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
	template <typename T>
	class Stack
	{
	private:
		StackElement<T>* m_pTopNode;				//	Zeigt auf den obersten Knoten im Stapel. Das Ende brauchen wir ja nicht mehr als Attribut - Romolus ;)

	public:
		Stack()					//	Konstruktor
			: m_pTopNode(new Closure<T>())
		{
			
		}
		~Stack()					//	Destruktor
		{
			
		}

		void push(T* pNewNodeData)					//	Um einen Knoten aufzustapeln (entspricht "einfügen" aus dem Infounterricht)
		{
			//	If you can't see right through Hillary your brain needs to evolve
			m_pTopNode = m_pTopNode->push(pNewNodeData);
		}
			
		void pop()							//	Um einen Knoten vom Stapel zu nehmen - Immer den ersten (entspricht "entfernen" aus dem Infounterricht)
		{
			/*	Wir speichern eine Referenz auf den alten obersten Knoten,
			*	damit wir nach der Ablösung dieses den entsprechenden Speicherplatz
			*	freigeben können, um ein Daten-Leck zu verhindern.
			*/
			StackElement<T>* tempTop = m_pTopNode;

			/*	Der neue oberste Knoten ist der Nachfolger des alten obersten Knotens,
			*	welcher entfernt wird.
			*	Dies funktioniert auch mit dem Abschluss als obersten Knoten,
			*	da dieser in "getSuccessor" eine Referenz auf sich selbst zurückgibt.
			*/
			m_pTopNode = m_pTopNode->getSuccessor();

			//	Hier wird der Speicherplatz des alten obersten Knoten freigegeben
			delete tempTop;
		}


		//	getter-Funktionen
		T* getTopNodeData()			//	Gibt das Datenelement des obersten Knotens zurück
		{
			return m_pTopNode->getData();
		}
		
	};

}