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
	/*	!!!
	*	
	*	Unser Stapel wird als Klassen-Template (Klassen-Vorlage) implementiert. 
	*	Dadurch ist unser Stapel typsicher. Er kann verschiedene Datentypen managen/stapeln, man kann aber nie mehrere Datentypen in einen einzigen Stapel einf�gen.
	*	Dies ist wichtig f�r uns, da wir den Stapel ben�tigen, um GameStates und Maps getrennt voneinander zu stapeln und zu verwalten.
	*	Es steht uns eine Reihe von Standardfunktionen zur Verf�gung, unabh�ngig davon, welcher Datentyp (Argumenttyp in den spitzen Klammern) angegeben wird.
	*	Damit das funktioniert m�ssen auch alle weiteren Klassen, die mit dem in Verbindung stehen (Stapelelement, Abschluss und Knoten) als Klassen-Template implementiert werden.
	*	
	*	Weitere Erkl�rung anhand eines Beispiels:
	*	Die C++-Standardbibliothek enth�t einen Container, der eine verkettete Liste implementiert. 
	*	Um eine verkettete Liste von int zu erstellen, schreibt man std::list<int>. 
	*	Eine verkettete Liste von Objekten des Datentypes std::string wird zu std::list<std::string>. 
	*	Mit list ist ein Satz von Standardfunktionen definiert, die immer verf�gbar sind, unabh�ngig davon, was man als Argumenttyp in den spitzen Klammern angibt. 
	*	Die Werte in spitzen Klammern werden Parameter genannt. Wenn ein Klassen-Template mit seinen Parametern dem Compiler �bergeben wird, so kann dieser das Template auspr�gen. 
	*	Er erzeugt hierbei zu jedem Parametertyp eine eigene Template-Klasse. Diese ist eine gew�hnliche Klasse, wie jede andere auch. 
	*	Die Begriffe Klassen-Template und Template-Klasse sind hier voneinander zu unterscheiden. 
	*	Wie Objekt und Klasse ist die Template-Klasse eine Auspr�gung eines Klassen-Templates.
	*
	*	!!!
	*/	

	//	TODO: Die Klasse f�r alle Datentypen (auch f�r int) sicher machen (und optimieren)

	template <typename T>							//	"T" ist nun Platzhalter f�r den Datentyp
	class Stack
	{
	private:
		StackElement<T>* m_pTopNode;				//	Zeigt auf den obersten Knoten im Stapel. Das Ende brauchen wir ja nicht mehr als Attribut - Romolus ;)

	public:
		Stack();									//	Konstruktor
		~Stack();									//	Destruktor

		void push(T*);								//	Um einen Knoten aufzustapeln (entspricht "einf�gen" aus dem Infounterricht)			
		void pop();									//	Um einen Knoten vom Stapel zu nehmen - Immer den ersten (entspricht "entfernen" aus dem Infounterricht)


		//	getter-Funktionen
		T* getTopNodeData()							//	Gibt das Datenelement des obersten Knotens zur�ck
		{
			return m_pTopNode->getData();
		}
		
	};

}

