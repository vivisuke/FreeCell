#include <random>
#include <algorithm>
#include <assert.h>
#include "Board.h"

using namespace std;

random_device g_rd;
#if	1
mt19937 g_mt(g_rd());
#else
mt19937 g_mt(0);
#endif

uint8 cardNum(card_t c) { return c&NUM_MASK; }
uint8 cardCol(card_t c) { return c&COL_MASK; }
uint8 cardColIX(card_t c) { return c>>4; }
bool isSameBlackRed(card_t c1, card_t c2)	//	c1, c2 が黒どうし or 赤どうしか？
{
	return ((c1 ^ c2) & BR_MASK) == 0;
}
bool canPushBack(card_t c1, card_t c2)	//	c1 の次に c2 を追加できるか？
{
	return cardNum(c1) - 1 == cardNum(c2) && !isSameBlackRed(c1, c2);
}

string to_card_string(card_t c)
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
	m_nFreeCell = 0;		//	フリーセルのカード数
	for(auto& x: m_freeCell) x = 0;
	for(auto& x: m_goal) x = 0;
	for(auto& x: m_cascade) x.clear();
	vector<card_t> deck;
	card_t col = 0;
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
void Board::genMoves(Moves& lst) const		//	可能着手生成
{
	lst.clear();
	for (int s = 0; s != N_CASCADE; ++s) {
		if( !m_cascade[s].empty() ) {				//	列[i] が空でない場合
			if( m_nFreeCell != N_FREECELL )
				lst.emplace_back('0'+s, 'F');			//	フリーセルへの移動
			card_t sc = m_cascade[s].back();		//	末尾カード
			//	別の列末尾への移動
			for (int d = 0; d != N_CASCADE; ++d) {
				if( s != d && !m_cascade[d].empty() && canPushBack(m_cascade[d].back(), sc) )
					lst.emplace_back('0'+s, '0'+d);			//	i から d への移動
			}
			//	ゴールへの移動
			auto gi = cardColIX(sc);
			if( m_goal[gi] == cardNum(sc) - 1 )
				lst.emplace_back('0'+s, 'G');			//	ゴールへの移動
		}
	}
	//	フリーセルから列・ゴールへの移動
	for (int s = 0; s != m_nFreeCell; ++s) {
		card_t sc = m_freeCell[s];
		//	別の列末尾への移動
		for (int d = 0; d != N_CASCADE; ++d) {
			if( !m_cascade[d].empty() && canPushBack(m_cascade[d].back(), sc) )
				lst.emplace_back('A'+s, '0'+d);			//	i から d への移動
		}
		//	ゴールへの移動
		auto gi = cardColIX(sc);
		if( m_goal[gi] == cardNum(sc) - 1 )
			lst.emplace_back('0'+s, 'G');			//	ゴールへの移動
	}
}
