#include <iostream>
#include "Board.h"
#include "utils.h"

using namespace std;

int main()
{
	Board bd;
	cout << bd.text() << "\n";
	//
	for (int i = 0; i < 100; ++i) {
		Moves lst;
		bd.genMoves(lst);
		if( lst.empty() ) break;
		for(const auto& mv: lst) cout << mv.m_src << mv.m_dst << " ";
		cout << "\n\n";
		Move mv = lst[g_mt() % lst.size()];
		cout << (i+1) << ": Move " << mv.m_src << mv.m_dst << "\n";
		bd.doMove(mv);
		cout << bd.text() << "\n";
	}
	//
    std::cout << "OK\n";
}
