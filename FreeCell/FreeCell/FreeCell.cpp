#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "Board.h"
#include "utils.h"

using namespace std;

unordered_map<string, int> g_map;

int main()
{
	Board bd;
	cout << bd.text() << "\n";
	Board b2(bd);
	assert(b2 == bd);
	//
	Moves lst;
	bd.genMoves(lst);
	cout << "Moves: ";
	for(const auto& mv: lst) cout << mv.m_src << mv.m_dst << " ";
	cout << "\n\n";
	for(const auto& mv: lst) {
		cout << "Move " << mv.m_src << mv.m_dst << "\n";
		bd.doMove(mv);
		cout << bd.text() << "\n";
		bd.unMove(mv);
		cout << bd.text() << "\n";
		assert( b2 == bd );
	}
	//
#if	0
	for (int i = 0; i < 1; ++i) {
		auto hk = bd.hkeyText();
		if( g_map.find(hk) != g_map.end() )
			break;
		g_map[hk] = i;
		Moves lst;
		bd.genMoves(lst);
		if( lst.empty() ) break;
		cout << "Moves: ";
		for(const auto& mv: lst) cout << mv.m_src << mv.m_dst << " ";
		cout << "\n\n";
		Move mv = lst[g_mt() % lst.size()];
		cout << (i+1) << ") Move " << mv.m_src << mv.m_dst << "\n";
		bd.doMove(mv);
		cout << bd.text() << "\n";
	}
#endif
	//
    std::cout << "OK\n";
}
