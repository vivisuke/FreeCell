#include <iostream>
#include <unordered_map>
#include <chrono>
#include <assert.h>
#include "Board.h"
#include "utils.h"

using namespace std;

//unordered_map<string, int> g_map;
unordered_map<string, Move> g_map;

int main()
{
	//cout << "♠♣♥◆\n";
	Board bd;
	assert( bd.checkNCard() );
	string hk0 = {0x0c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x15,0x11,0x3d,0x2c,0x3b,0x12,0x16,0x25,0x04,0x00,0x22,0x26,
						0x38,0x07,0x08,0x02,0x33,0x00,0x2b,0x3c,0x0d,0x17,0x36,0x1c,0x06,0x35,0x00,0x28,0x3a,0x39,0x09,0x01,
						0x2d,0x0a,0x00,0x00,0x29,0x0b,0x1a,0x18,0x23,0x05,0x34,0x00,0x13,0x21,0x1d,0x19,0x32,0x37,0x00,0x14,
						0x27,0x1b,0x31,0x24,0x2a,0x00,};
	bd.set(hk0);
	cout << bd.text() << "\n";
	Moves mvs;
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.text() << " ";
	cout << "\n\n";
	//
#if	0
	auto start = std::chrono::system_clock::now();
	auto hktxt = bd.hkeyText();
	int mxnm = 0;		//	最大移動可能降順列数
	string mxnmhk;		//	最大移動可能降順列数を与える局面ハッシュテキスト
	g_map.clear();
	g_map[hktxt] = Move(0,0);
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
						mxnmhk = bd.hkeyText();
						mxnm = nm;
						if( mxnm > 5 )
							cout << "nm = " << mxnm << "\n" << bd.text() << "\n";
					}
					//g_map[hk] = n;
					g_map[hk] = mv;
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
    //	手順表示
    string hk = mxnmhk;
	bd.set(hk);
	Moves mvs;
    for (;;) {
    	Move mv = g_map[hk];
	    if( mv == Move(0,0) ) break;
	    //mvs.push_back(mv);
	    mvs.insert(mvs.begin(), mv);		//	手数は少ないのでおｋ
	    bd.unMove(mv);
	    //cout << "Move: " << mv.text() << "\n";
		//cout << bd.text() << "\n";
		hk = bd.hkeyText();
    }
	cout << bd.text() << "\n";
	int cnt = 0;
    for(const auto& mv: mvs) {
	    cout << "#" << ++cnt << " Move: " << mv.text() << "\n";
	    bd.doMove(mv);
		cout << bd.text() << "\n";
    }
	cout << "hkey = " << bd.hkeyHex() << "\n";
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.text() << " ";
	cout << "\n\n";
#endif
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
