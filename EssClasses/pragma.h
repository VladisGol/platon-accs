/*
software core of accounting system "Platon".
Copyright (C) 2005-2007 Borisenkov S., Golovyrin V.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation version 2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
Contacts: e-mail vladisgol@rambler.ru

ßäðî ñèñòåìû ó÷åòà "Ïëàòîí".
Àâòîðàìè ïðîãðàììû ÿâëÿþòñÿ Áîðèñåíêîâ Ñåðãåé Àëåêñàíäðîâè÷ è Ãîëîâûðèí Âëàäèñëàâ Âëàäèìèðîâè÷, 2005-2007ã.
Äàííàÿ áèáëèîòåêà ÿâëÿåòñÿ ñâîáîäíûì ïðîãðàììíûì îáåñïå÷åíèåì. Âû âïðàâå ðàñïðîñòðàíÿòü åå è/èëè ìîäèôèöèðîâàòü
â ñîîòâåòñòâèè ñ óñëîâèÿìè âåðñèè 2.1 Ñòàíäàðòíîé Îáùåñòâåííîé Ëèöåíçèè Îãðàíè÷åííîãî Ïðèìåíåíèÿ GNU,
îïóáëèêîâàííîé Free Software Foundation.
Ìû ðàñïðîñòðàíÿåì ýòó ïðîãðàììó â íàäåæäå íà òî, ÷òî îíà áóäåò âàì ïîëåçíîé,
îäíàêî ÍÅ ÏÐÅÄÎÑÒÀÂËßÅÌ ÍÀ ÍÅÅ ÍÈÊÀÊÈÕ ÃÀÐÀÍÒÈÉ, â òîì ÷èñëå ÃÀÐÀÍÒÈÈ ÒÎÂÀÐÍÎÃÎ ÑÎÑÒÎßÍÈß ÏÐÈ ÏÐÎÄÀÆÅ è
ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÈÑÏÎËÜÇÎÂÀÍÈß Â ÊÎÍÊÐÅÒÍÛÕ ÖÅËßÕ.
Äëÿ ïîëó÷åíèÿ áîëåå ïîäðîáíîé èíôîðìàöèè îçíàêîìüòåñü ñî Ñòàíäàðòíîé Îáùåñòâåííîé Ëèöåíçèåé Îãðàíè÷åííîãî
Ïðèìåíåíèé GNU.
Ñâÿçàòüñÿ ñ àâòîðàìè ïðîãðàììû âû ìîæåòå ïî e-mail vladisgol@rambler.ru
*/

#ifndef PragmaH
#define PragmaH

#include <string>

#include "hypotesis.h"

#include "ibpp.h"

namespace platon
{

//class Eidos;
//class ExtraAttribute;
//class Hypotesis;

//Êëàññ âîïëîùåíèé - îïèñûâàåò îáúåêòû ðåàëüíîãî ìèðà
	class Pragma : public Hypotesis
	{
		private:
			void SetHypotesName ( std::string value ) {;};
			std::string GetHypotesName() {return "";};

		public:
			Hypotesis* HostHypotesis;
			Pragma ( Hypotesis* MyType,IBPP::Timestamp DateCreation );
			Pragma ( Hypotesis* MyType, long ID_Copy );
			long Save();
			std::string GetLinkedPragmaSQL ( long IDLinkEA )const;
			std::string GetLinkedPragmaSQL ( std::string LinkEAName )const;
			static void GetEidosHypotesisIDS ( IBPP::Database MyDB, long ID_IN,long &ID_Eidos, long &ID_Hypotesis );
			std::string GetPragmaName();
	};
}
#endif
