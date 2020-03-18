#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
	Board bd;
	cout << bd.text() << "\n";
	//
	Moves lst;
	bd.genMoves(lst);
	for(const auto& mv: lst) cout << mv.m_src << mv.m_dst << " ";
	cout << "\n";
	//
    std::cout << "OK\n";
}
