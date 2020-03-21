#include <iostream>
#include <random>
#include <algorithm>
#include <assert.h>
#include "Board.h"
#include "utils.h"

using namespace std;

#if	0
random_device g_rd;
#if	1
mt19937 g_mt(g_rd());
#else
mt19937 g_mt(0);
#endif
#endif

uint8 cardNum(card_t c) { return c&NUM_MASK; }
uint8 cardCol(card_t c) { return c&COL_MASK; }
uint8 cardColIX(card_t c) { return c>>NUM_BITS; }
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
	if( !(c&NUM_MASK) ) return " . ";
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
//----------------------------------------------------------------------
string Move::text() const
{
	string txt;
	txt += m_src;
	txt += m_dst;
	if( m_n != 1 )
		txt += "*" + to_string((int)m_n);
	return txt;
}
//----------------------------------------------------------------------
Board::Board()
{
	init();
}
Board::Board(const Board& x)
{
	m_nCardFreeCell = x.m_nCardFreeCell;
	for (int i = 0; i < N_FREECELL+1; ++i) {
		m_freeCell[i] = x.m_freeCell[i];
	}
	for (int i = 0; i < N_HOME; ++i) {
		m_home[i] = x.m_home[i];
	}
	for (int i = 0; i < N_COLUMN; ++i) {
		m_column[i] = x.m_column[i];
	}
}
bool Board::operator==(const Board& x) const
{
	if( m_nCardFreeCell != x.m_nCardFreeCell ) return false;
	for (int i = 0; i < N_FREECELL+1; ++i) {
		if( m_freeCell[i] != x.m_freeCell[i] ) return false;
	}
	for (int i = 0; i < N_HOME; ++i) {
		if( m_home[i] != x.m_home[i] ) return false;
	}
	for (int i = 0; i < N_COLUMN; ++i) {
		if( m_column[i] != x.m_column[i] ) return false;
	}
	return true;
}
void Board::init()		//	初期化・カードを配る
{
	m_nCardFreeCell = 0;		//	フリーセルのカード数
	for(auto& x: m_freeCell) x = 0;
	for(auto& x: m_home) x = 0;
	for(auto& x: m_column) x.clear();
	vector<card_t> deck;
	card_t col = 0;
	for (int c = 0; c != N_COL; ++c, col+=0x10) {
		for (int n = 1; n <= N_NUM; ++n) {
			deck.push_back(col+n);		//	52枚のカードをいったんデックに入れる
		}
	}
	shuffle(deck.begin(), deck.end(), g_mt);		//	デックをシャフル
	for (int i = 0; i !=deck.size(); ++i) {
		m_column[i%N_COLUMN].push_back(deck[i]);	//	デックのカードを順に m_column[] に格納
	}
}
void Board::initNoShuffle()		//	初期化・カードを配る
{
	m_nCardFreeCell = 0;		//	フリーセルのカード数
	for(auto& x: m_freeCell) x = 0;
	for(auto& x: m_home) x = 0;
	for(auto& x: m_column) x.clear();
	vector<card_t> deck;
	card_t col = 0;
	for (int c = 0; c != N_COL; ++c, col+=0x10) {
		for (int n = N_NUM; n >= 1; --n) {
			deck.push_back(col+n);		//	52枚のカードをいったんデックに入れる
		}
	}
	//shuffle(deck.begin(), deck.end(), g_mt);		//	デックをシャフル
	for (int i = 0; i !=deck.size(); ++i) {
		m_column[i%N_COLUMN].push_back(deck[i]);	//	デックのカードを順に m_column[] に格納
	}
}
std::string Board::text() const
{
	string txt;
	txt += "A: ";
	//for(auto x: m_home) txt += to_card_string(x);
	for (int i = 0; i < N_COL; ++i) {
		txt += to_card_string(m_home[i] + (i<<NUM_BITS));
	}
	txt += "\n";
	txt += "F: ";
	for (int i = 0; i != N_FREECELL; ++i)
		txt += to_card_string(m_freeCell[i]);
	//for(auto x: m_freeCell) txt += to_card_string(x);
	txt += " (nCardFreeCell = " + to_string(m_nCardFreeCell) + ")\n";
	//txt += "\n";
	//
	int c = 0;
	for(const auto& lst: m_column) {
		txt += to_string(c++) + ": ";
		for(auto x: lst)
			txt += to_card_string(x);
		txt += "\n";
	}
	return txt;
}
std::string Board::hkeyHex() const			//	ハッシュキー１６進数テキスト
{
	string hk = hkeyText();
	string txt;
	txt.resize(hk.size()*5+1);
	int ix = 0;
#if	1
	for (int i = 0; i != hk.size(); ++i) {
		sprintf_s(&txt[ix], txt.size()-ix, "0x%02x,", (int)hk[i]);
		ix += 5;
	}
#else
	for(auto ch: hk) {
		sprintf_s(&txt[ix], txt.size()-ix, "0x%02x,", (int)ch);
		ix += 5;
	}
#endif
	return txt;
}
std::string Board::hkeyText() const			//	ハッシュキーテキスト
{
	string txt;
	int sz = N_FREECELL + N_HOME;
	for(const auto& lst: m_column) sz += lst.size() + 1;		//	+1 for '\0'
	txt.resize(sz);
	memcpy(&txt[0], (cchar*)&m_freeCell[0], N_FREECELL);		//	フリーセル状態
	memcpy(&txt[N_FREECELL], (cchar*)&m_home[0], N_HOME);	//	ゴール状態
	int ix = N_FREECELL + N_HOME;
	for(const auto& lst: m_column) {
		if( lst.empty() ) {
			txt[ix++] = '\0';		//	ヌルターミネート
		} else {
			memcpy(&txt[ix], (cchar*)&lst[0], lst.size());		//	各列状態
			txt[ix+=lst.size()] = '\0';		//	ヌルターミネート
			++ix;
		}
	}
	return txt;
}
void Board::set(const std::string& txt)
{
	memcpy(&m_freeCell[0], (cchar*)&txt[0], N_FREECELL);		//	フリーセル状態
	for(m_nCardFreeCell = 0; m_freeCell[m_nCardFreeCell] != 0; ++m_nCardFreeCell) {}
	memcpy(&m_home[0], (cchar*)&txt[N_FREECELL], N_HOME);	//	ゴール状態
	int ix = N_FREECELL + N_HOME;
	for(auto& lst: m_column) {
		lst.clear();
		while( txt[ix] != '\0' ) lst.push_back(txt[ix++]);
		++ix;
	}
}
int Board::nCardHome() const
{
	return m_home[0] + m_home[1] + m_home[2] + m_home[3];
}
int Board::nEmptyColumns() const			//	空欄カラム数を返す
{
	int n = 0;
	for(const auto& lst: m_column)
		if( lst.empty() ) ++n;
	return n;
}
int Board::nMobableDesc() const			//	移動可能降順列数を返す
{
	return (nEmptyColumns() + 1) * (N_FREECELL + 1 - m_nCardFreeCell);
}
int Board::nMobableDescToEmpty() const			//	空列への移動可能降順列数を返す
{
	return nEmptyColumns() * (N_FREECELL + 1 - m_nCardFreeCell);
}
bool Board::checkNCard() const				//	カード数チェック
{
	int n = m_nCardFreeCell;
	for (int i = 0; i < m_nCardFreeCell; ++i) {
		if( m_freeCell[i] == 0 )
			return false;
	}
	for (int i = m_nCardFreeCell; i < N_FREECELL; ++i) {
		if( m_freeCell[i] != 0 )
			return false;
	}
	for(auto c: m_home) n += c;
	for(const auto& lst: m_column) n += lst.size();
	return n == N_CARD;
}
//	評価値を返す
//		移動可能数*10000 + ホーム枚数*100 + 列末尾降順列枚数
int Board::eval() const
{
	int ev = nMobableDesc() * 10000 + nCardHome() * 100;
	for(const auto& lst: m_column) {
		if( lst.empty() ) continue;
		for (int i = lst.size() - 1; --i >= 0; ) {
			if( !canPushBack(lst[i], lst[i+1]) )
				break;
			ev += 1;
		}
	}
	return ev;
}
void Board::genMoves(Moves& mvs) const		//	可能着手生成
{
	mvs.clear();
	//	列からの移動
	for (int s = 0; s != N_COLUMN; ++s) {
		if( !m_column[s].empty() ) {				//	列[i] が空でない場合
			if( m_nCardFreeCell != N_FREECELL ) {
				mvs.emplace_back('0'+s, 'F'+m_nCardFreeCell);			//	フリーセルへの移動
			}
			card_t sc = m_column[s].back();		//	末尾カード
			//	別の列末尾への移動
			for (int d = 0; d != N_COLUMN; ++d) {
				if( s != d ) {
					if( m_column[d].empty() ) {
						mvs.emplace_back('0'+s, '0'+d);			//	i から d への移動
					} else if( canPushBack(m_column[d].back(), sc) )
						mvs.emplace_back('0'+s, '0'+d);			//	i から d への移動
				}
			}
			//	ゴールへの移動
			auto gi = cardColIX(sc);
			if( m_home[gi] == cardNum(sc) - 1 )
				mvs.emplace_back('0'+s, 'A'+gi);			//	ゴールへの移動
		}
	}
	//	フリーセルから列・ゴールへの移動
	for (int s = 0; s != m_nCardFreeCell; ++s) {
		card_t sc = m_freeCell[s];
		//	別の列末尾への移動
		for (int d = 0; d != N_COLUMN; ++d) {
			if( m_column[d].empty() ) {
				mvs.emplace_back('F'+s, '0'+d);			//	i から d への移動
			} else if( canPushBack(m_column[d].back(), sc) )
				mvs.emplace_back('F'+s, '0'+d);			//	i から d への移動
		}
		//	ゴールへの移動
		auto gi = cardColIX(sc);
		if( m_home[gi] == cardNum(sc) - 1 )
			mvs.emplace_back('F'+s, 'A'+gi);			//	ゴールへの移動
	}
	//	列→列 降順列移動、空列への移動も生成（ただし、列全体の空列移動は不可）
	const int mxnm = nMobableDesc();
	const int mxnm2 = nMobableDescToEmpty();		//	空列への移動可能枚数
	for (int s = 0; s != N_COLUMN; ++s) {
		const auto& lst = m_column[s];
		if( lst.size() > 1 ) {
			const int SZ = lst.size();
			int n = 1;		//	降順列枚数
			while( SZ-n-1 >= 0 && n < mxnm && canPushBack(lst[SZ-n-1], lst[SZ-n]) ) ++n;
			if( n > 1 ) {
				auto top = lst[SZ-n];
				for (int d = 0; d != N_COLUMN; ++d) {
					if( d == s ) continue;
					if( m_column[d].empty() ) {		//	空列への移動
						int n2 = min(n, mxnm2);	//	移動可能枚数
						if( n2 < lst.size() )				//	列全体を空列に移動するのは不可
							mvs.emplace_back('0'+s, '0'+d, (char)n2);
					} else {
						if( canPushBack(m_column[d].back(), top) )
							mvs.emplace_back('0'+s, '0'+d, (char)n);
					}
				}
			}
		}
	}
}
bool Board::isSafeToHome(card_t cd) const		//	カードを安全にホーム移動できるか？
{
	int num = (int)cardNum(cd);
	auto ix = cardColIX(cd);
	if( m_home[ix] != num - 1 )
		return false;
	num -= 2;
	//auto col = cardCol(cd);
	switch( ix ) {
	case IX_SPADE:
	case IX_CLUB:
		return m_home[IX_HEART] >= num && m_home[IX_DIAMOND] >= num;
	case IX_HEART:
	case IX_DIAMOND:
		return m_home[IX_SPADE] >= num && m_home[IX_CLUB] >= num;
	}
	assert(0);
	return false;
}
bool Board::genSafeMove(Move& mv) const				//	安全にホーム移動できる着手生成
{
	for (int s = 0; s != N_COLUMN; ++s) {
		if( m_column[s].empty() ) continue;
		auto cd = m_column[s].back();
		if( isSafeToHome(cd) ) {
			mv = Move('0'+s, 'A'+cardColIX(cd));
			return true;
		}
	}
	return false;
}
void Board::doMove(const Move& mv)
{
	card_t cd = 0;		//	移動カード
	if( isdigit(mv.m_src) ) {		//	列からの移動
		int ix = mv.m_src - '0';
		if( mv.m_n == 1 ) {
			cd = m_column[ix].back();
			m_column[ix].pop_back();
		} else {		//	列→列 の降順列移動
			assert( isdigit(mv.m_dst) );
			int dst = mv.m_dst - '0';
			m_column[dst].insert(m_column[dst].end(), m_column[ix].end() - mv.m_n, m_column[ix].end());
			m_column[ix].erase(m_column[ix].end() - mv.m_n, m_column[ix].end());
			return;
		}
	} else {		//	フリーセルからの移動
		int ix = mv.m_src - 'F';
		//if( !(ix >= 0 && ix < m_nCardFreeCell) ) {
		//	cout << "???\n";
		//}
		assert( ix >= 0 && ix < m_nCardFreeCell );
		cd = m_freeCell[ix];
		while( (m_freeCell[ix] = m_freeCell[ix+1]) != 0 ) {
			++ix;
		}
		m_nCardFreeCell -= 1;
	}
	if( isdigit(mv.m_dst) ) {		//	列への移動
		int ix = mv.m_dst - '0';
		m_column[ix].push_back(cd);
	} else if( mv.m_dst >= 'F' ) {		//	フリーセルへの移動
		m_freeCell[m_nCardFreeCell++] = cd;
	} else if( mv.m_dst <= 'D' ) {		//	ゴールへの移動
		int ix = cardColIX(cd);
		assert( ix == mv.m_dst - 'A' );
		m_home[ix] += 1;
	}
}
void Board::unMove(const Move& mv)
{
	card_t cd = 0;		//	移動カード
	if( isdigit(mv.m_dst) ) {		//	列への移動
		int dst = mv.m_dst - '0';
		if( mv.m_n == 1 ) {
			cd = m_column[dst].back();
			m_column[dst].pop_back();
		} else {
			assert( isdigit(mv.m_dst) );
			int src = mv.m_src - '0';
			m_column[src].insert(m_column[src].end(), m_column[dst].end() - mv.m_n, m_column[dst].end());
			m_column[dst].erase(m_column[dst].end() - mv.m_n, m_column[dst].end());
			return;
		}
	} else if( mv.m_dst >= 'F' ) {		//	フリーセルへの移動
		int ix = mv.m_dst - 'F';
		assert( ix >= 0 && ix < m_nCardFreeCell );
		cd = m_freeCell[ix];
		m_freeCell[ix] = 0;
		m_nCardFreeCell -= 1;
	} else if( mv.m_dst <= 'D' ) {		//	ゴールへの移動
		int ix = mv.m_dst - 'A';
		cd = (ix << NUM_BITS) + m_home[ix];
		m_home[ix] -= 1;
	}
	if( isdigit(mv.m_src) ) {		//	列からの移動
		int ix = mv.m_src - '0';
		m_column[ix].push_back(cd);
	} else if( mv.m_src >= 'F' ) {		//	フリーセルからの移動
		int ix = mv.m_src - 'F';
		for (int i = N_FREECELL-1; --i >= ix; ) {
			m_freeCell[i+1] = m_freeCell[i];
		}
		m_freeCell[ix] = cd;
		m_nCardFreeCell += 1;
	}
}
