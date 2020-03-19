#include <iostream>
#include <unordered_map>
#include <chrono>
#include <assert.h>
#include "Board.h"
#include "utils.h"

using namespace std;

unordered_map<string, int> g_map;

int main()
{
	//cout << "♠♣♥◆\n";
	Board bd;
	assert( bd.checkNCard() );
	cout << bd.text() << "\n";
	//
	auto start = std::chrono::system_clock::now();
	auto hktxt = bd.hkeyText();
	int mxnm = 0;		//	最大移動可能降順列数
	g_map.clear();
	g_map[hktxt] = 0;
	vector<string> lst, lst2;
	lst.push_back(hktxt);
	for (int n = 1; n <= 10; ++n) {		//	手数
		lst2.clear();	//	末端ノード
		for(const auto& txt: lst) {
			bd.set(txt);
			assert(bd.checkNCard());
			Moves lst;
			bd.genMoves(lst);
			for(const auto& mv: lst) {
				//if (mv.m_src == '0' && mv.m_dst == 'F')
				//	cout << bd.text() << "\n";
				bd.doMove(mv);
				if( !bd.checkNCard() )
					cout << bd.text() << "\n";
				assert( bd.checkNCard() );
				auto hk = bd.hkeyText();
				if( g_map.find(hk) == g_map.end() ) {
					//mxnm = max(mxnm, bd.nMobableDesc());
					auto nm = bd.nMobableDesc();
					if( nm > mxnm ) {
						mxnm = nm;
						if( mxnm > 5 )
							cout << "nm = " << mxnm << "\n" << bd.text() << "\n";
					}
					g_map[hk] = n;
					lst2.push_back(hk);
				}
				//if( mv.m_src == 'F' && mv.m_dst == '6' && bd.nFreeCell() > 0 )
				//	cout << bd.text() << "\n";
				//if (!bd.checkNCard())
				//	cout << bd.text() << "\n";
				bd.unMove(mv);
				//if( !bd.checkNCard() )
				//	cout << bd.text() << "\n";
				assert( bd.checkNCard() );
			}
		}
		lst.swap(lst2);		//	末端ノードリストを lst に転送
		cout << n << ": lst.size() = " << lst.size() << ", mxnm = " << mxnm << "\n";
		if( mxnm > 5 ) break;
	}
	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = end - start;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "dur: " << msec << "msec\n";
    //
#if	0
	auto start = std::chrono::system_clock::now();
	auto hktxt = bd.hkeyText();
	g_map.clear();
	g_map[hktxt] = 0;
	vector<string> lst, lst2;
	lst.push_back(hktxt);
	for (int n = 1; n <= 10; ++n) {		//	手数
		lst2.clear();	//	末端ノード
		for(const auto& txt: lst) {
			bd.set(txt);
			Moves lst;
			bd.genMoves(lst);
			for(const auto& mv: lst) {
				bd.doMove(mv);
				auto hk = bd.hkeyText();
				if( g_map.find(hk) == g_map.end() ) {
					g_map[hk] = n;
					lst2.push_back(hk);
				}
				bd.unMove(mv);
			}
		}
		lst.swap(lst2);		//	末端ノードリストを lst に転送
		cout << n << ": lst.size() = " << lst.size() << "\n";
	}
	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = end - start;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "dur: " << msec << "msec\n";
#endif
    //
#if	0
	Board b2(bd);
	auto hktxt = bd.hkeyText();
	bd.init();
	assert( bd != b2 );
	bd.set(hktxt);
	assert( bd == b2 );
#endif
#if	0
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
#endif
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
