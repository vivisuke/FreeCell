#include <random>
#include <algorithm>
#include <assert.h>
#include "Board.h"

using namespace std;

random_device g_rd;
#if	0
mt19937 g_mt(g_rd());
#else
mt19937 g_mt(0);
#endif

string to_card_string(uint8 c)
{
	if( !c ) return " .";
	string txt;
	int col = c & COL_MASK;
	switch(c & COL_MASK) {
	case SPADE:	txt += "S";	break;
	case CLUB:	txt += "C";	break;
	case HEART:	txt += "H";	break;
	case DIAMOND:	txt += "D";	break;
	}
	int n = c & NUM_MASK;
	switch( n ) {
	case 1:	txt += "A";	break;
	case 10:	txt += "T";	break;
	case 11:	txt += "J";	break;
	case 12:	txt += "Q";	break;
	case 13:	txt += "K";	break;
	default:
		txt += to_string(n);
		break;
	}
	txt += " ";
	return txt;
}

Board::Board()
{
	init();
}
void Board::init()		//	初期化・カードを配る
{
	for(auto& x: m_freeCell) x = 0;
	for(auto& x: m_goal) x = 0;
	for(auto& x: m_cascade) x.clear();
	vector<uint8> deck;
	uint8 col = 0;
	for (int c = 0; c != N_COL; ++c, col+=0x10) {
		for (int n = 1; n <= N_NUM; ++n) {
			deck.push_back(col+n);
		}
	}
	shuffle(deck.begin(), deck.end(), g_mt);
	for (int i = 0; i !=deck.size(); ++i) {
		m_cascade[i%N_CASCADE].push_back(deck[i]);
	}
}
std::string Board::text() const
{
	string txt;
	txt += "G: ";
	for(auto x: m_goal) txt += to_card_string(x);
	txt += "\n";
	txt += "F: ";
	for(auto x: m_freeCell) txt += to_card_string(x);
	txt += "\n";
	//
	int c = 0;
	for(const auto& lst: m_cascade) {
		txt += to_string(c++) + ": ";
		for(auto x: lst)
			txt += to_card_string(x);
		txt += "\n";
	}
	return txt;
}
