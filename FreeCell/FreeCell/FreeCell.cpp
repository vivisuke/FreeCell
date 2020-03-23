#include <iostream>
#include <unordered_map>
#include <chrono>
#include <assert.h>
#include "Board.h"
#include "utils.h"

using namespace std;

//unordered_map<string, int> g_map;
unordered_map<string, Move> g_map;


void	test_genMove();			//	移動手生成
void	test_genMove1();		//	１枚移動手のみ生成
void	test_eval();
void test_120811();
void test_search();
void searchMovable6(Board& bd);		//	初期状態から降順列６枚以上移動可能状態を探す
bool searchHomePlusMovable6(Board& bd);		//	現状態から、降順列６枚以上移動可能 かつ Home枚数が増えた状態を探す
bool do_search(Board& bd, vector<string>& hist);		//	現状態から、評価値が増加する状態を探す

int main()
{
	//cout << "♠♣♥◆\n";
	//test_eval();
	//test_genMove();
	//test_genMove1();
	//test_120811();
	test_search();
	//
#if	0
	if (true) {
		Board bd;
		//string hk0 = {0x00,0x00,0x00,0x00,0x07,0x06,0x06,0x07,0x29,0x08,0x00,0x1d,0x3c,0x0b,0x2a,0x19,0x28,0x17,0x00,0x3d,
		//					0x1c,0x3b,0x0a,0x39,0x18,0x27,0x00,0x00,0x0d,0x2c,0x1b,0x3a,0x09,0x38,0x00,0x00,0x2d,0x0c,0x2b,0x00,0x1a,0x00};
		//string hk0 = {0x1d,0x3a,0x00,0x00,0x09,0x09,0x07,0x09,0x0d,0x3c,0x1b,0x2a,0x00,0x2d,0x00,0x0c,0x2b,0x1a,0x00,0x00,
		//					0x3d,0x1c,0x3b,0x0a,0x29,0x00,0x0b,0x00,0x00,0x28,0x2c,0x00,};		//	乱数シード１で不正になる局面
		//	列のKから始まる高順列は別の列に移動しない
		string hk0 = { 0x18,0x19,0x00,0x00,0x01,0x02,0x00,0x07,0x39,0x15,0x0d,0x27,0x08,0x1d,0x2c,0x0b,0x3a,0x09,0x28,0x17,
							0x26,0x05,0x00,0x16,0x13,0x06,0x25,0x04,0x23,0x00,0x3c,0x03,0x38,0x21,0x3d,0x1c,0x00,0x2d,0x0c,0x2b,
							0x0a,0x00,0x2a,0x29,0x1a,0x02,0x22,0x00,0x24,0x1b,0x07,0x3b,0x14,0x00,0x00,0x00, };
		bd.set(hk0);
		cout << bd.text() << "\n";
		Moves mvs;
		bd.genMoves(mvs);
		cout << "Moves: ";
		for (const auto& mv : mvs) cout << mv.text() << " ";
		cout << "\n\n";
	}
#endif
	//
#if	0
	Board bd;
	string hk0 = {0x25,0x00,0x00,0x00,0x01,0x01,0x01,0x05,0x0c,0x08,0x3c,0x17,0x06,0x05,0x27,0x00,0x0b,0x2d,0x22,0x04,
						 0x02,0x36,0x2a,0x19,0x28,0x00,0x00,0x07,0x1d,0x37,0x14,0x13,0x29,0x00,0x0d,0x2c,0x1b,0x3a,0x09,0x38,
						 0x00,0x00,0x23,0x24,0x2b,0x26,0x03,0x00,0x1a,0x16,0x15,0x18,0x12,0x3d,0x1c,0x3b,0x0a,0x39,0x00,};
	bd.set(hk0);
	cout << bd.text() << "\n";
	Moves mvs;
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.text() << " ";
	cout << "\n\n";
#endif
	//
#if	0
	Board bd;
	vector<string> hist;		//	中間目標リスト
	searchMovable6(bd);		//	初期状態から降順列６枚以上移動可能状態を探す
	hist.push_back(bd.hkeyText());
	while( bd.nCardHome() < 52 ) {
		//if( !searchHomePlusMovable6(bd) )		//	現状態から、降順列６枚以上移動可能 かつ Home枚数が増えた状態を探す
		Move mv;
		while( bd.genSafeMove(mv) ) {
			cout << "Move: " << mv.text() << "\n";
			bd.doMove(mv);
			cout << bd.text() << "\n";
			cout << "eval = " << bd.eval() << "\n";
		}
		cout << bd.hkeyHex() << "\n";
		if( !do_search(bd, hist) )		//	現状態から、評価値が増加する状態を探す
			break;
		cout << "eval = " << bd.eval() << "\n";
	}
#endif
	//
#if	0
	//cout << "♠♣♥◆\n";
	Board bd;
	assert( bd.checkNCard() );
	//
	bd.initNoShuffle();
	cout << bd.text() << "\n";
	Moves mvs;
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.text() << " ";
	cout << "\n\n";
	Move mv;
	while( bd.genSafeMove(mv) ) {
		cout << "Move: " << mv.text() << "\n";
		bd.doMove(mv);
		cout << bd.text() << "\n";
	}
#endif
	//
#if	0
	string hk0 = {0x0c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x15,0x11,0x3d,0x2c,0x3b,0x12,0x16,0x25,0x04,0x00,0x22,0x26,
						0x38,0x07,0x08,0x02,0x33,0x00,0x2b,0x3c,0x0d,0x17,0x36,0x1c,0x06,0x35,0x00,0x28,0x3a,0x39,0x09,0x01,
						0x2d,0x0a,0x00,0x00,0x29,0x0b,0x1a,0x18,0x23,0x05,0x34,0x00,0x13,0x21,0x1d,0x19,0x32,0x37,0x00,0x14,
						0x27,0x1b,0x31,0x24,0x2a,0x00,};
	bd.set(hk0);
	cout << bd.text() << "\n";
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
			Moves mvs;
			bd.genMoves(mvs);
			for(const auto& mv: mvs) {
				//if (mv.m_src == '0' && mv.m_dst == 'F')
				//	cout << bd.text() << "\n";
				bd.doMove(mv);
				if( !bd.checkNCard() )
					cout << bd.text() << "\n";
				assert( bd.checkNCard() );
				auto hk = bd.hkeyText();
				if( g_map.find(hk) == g_map.end() ) {
					auto nm = bd.nMobableDesc();
					if( nm > mxnm ) {
						mxnmhk = bd.hkeyText();
						mxnm = nm;
						if( mxnm > 5 )
							cout << "nm = " << mxnm << "\n" << bd.text() << "\n";
					}
					g_map[hk] = mv;
					lst2.push_back(hk);
				}
				bd.unMove(mv);
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
	    mvs.insert(mvs.begin(), mv);		//	手数は少ないのでおｋ
	    bd.unMove(mv);
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
	Move mv('0', '6', 3);
	bd.doMove(mv);
	cout << bd.text() << "\n";
	bd.unMove(mv);
	cout << bd.text() << "\n";
#endif
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
			Moves mvs;
			bd.genMoves(mvs);
			for(const auto& mv: mvs) {
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
			Moves mvs;
			bd.genMoves(mvs);
			for(const auto& mv: mvs) {
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
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.m_src << mv.m_dst << " ";
	cout << "\n\n";
	for(const auto& mv: mvs) {
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
		Moves mvs;
		bd.genMoves(mvs);
		if( lst.empty() ) break;
		cout << "Moves: ";
		for(const auto& mv: mvs) cout << mv.m_src << mv.m_dst << " ";
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
void searchMovable6(Board& bd)		//	初期状態から降順列６枚以上移動可能状態を探す
{
	//Board bd;
	bd.init();
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
			Moves mvs;
			bd.genMoves(mvs);
			for(const auto& mv: mvs) {
				bd.doMove(mv);
				if( !bd.checkNCard() )
					cout << bd.text() << "\n";
				assert( bd.checkNCard() );
				auto hk = bd.hkeyText();
				if( g_map.find(hk) == g_map.end() ) {
					auto nm = bd.nMobableDesc();
					if( nm > mxnm ) {
						mxnmhk = bd.hkeyText();
						mxnm = nm;
						if( mxnm > 5 )
							cout << "nm = " << mxnm << "\n" << bd.text() << "\n";
					}
					g_map[hk] = mv;
					lst2.push_back(hk);
				}
				bd.unMove(mv);
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
	    mvs.insert(mvs.begin(), mv);		//	手数は少ないのでおｋ
	    bd.unMove(mv);
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
}
//	現状態から、降順列６枚以上移動可能 かつ Home枚数が増えた状態を探す
bool searchHomePlusMovable6(Board& bd)
{
	const int N_MOVABLE = 6;
	const int nCardHome0 = bd.nCardHome();		//	現ホームカード枚数
	auto start = std::chrono::system_clock::now();
	auto hktxt = bd.hkeyText();
	bool found = false;
	string hkey;
	//int mxnm = 0;		//	最大移動可能降順列数
	//string mxnmhk;		//	最大移動可能降順列数を与える局面ハッシュテキスト
	g_map.clear();
	g_map[hktxt] = Move(0,0);
	vector<string> lst, lst2;
	lst.push_back(hktxt);
	for (int n = 1; n <= 10; ++n) {		//	手数
		lst2.clear();	//	末端ノード
		for(const auto& txt: lst) {
			bd.set(txt);
			assert(bd.checkNCard());
			Moves mvs;
			bd.genMoves(mvs);
			for(const auto& mv: mvs) {
				bd.doMove(mv);
				if( !bd.checkNCard() )
					cout << bd.text() << "\n";
				assert( bd.checkNCard() );
				auto hk = bd.hkeyText();
				if( g_map.find(hk) == g_map.end() ) {
					auto nm = bd.nMobableDesc();
					if( nm >= N_MOVABLE && bd.nCardHome() > nCardHome0 ) {
						found = true;
						hkey = bd.hkeyText();
					}
#if	0
					if( nm > mxnm ) {
						mxnmhk = bd.hkeyText();
						mxnm = nm;
						if( mxnm > 5 )
							cout << "nm = " << mxnm << "\n" << bd.text() << "\n";
					}
#endif
					g_map[hk] = mv;
					lst2.push_back(hk);
				}
				bd.unMove(mv);
				assert( bd.checkNCard() );
				if( found ) break;
			}
		}
		lst.swap(lst2);		//	末端ノードリストを lst に転送
		cout << n << ": lst.size() = " << lst.size() << "\n";
		if( found ) break;
	}
	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = end - start;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "dur: " << msec << "msec\n";
    if( !found ) return false;
    //	手順表示
    //string hk = mxnmhk;
	bd.set(hkey);
	Moves mvs;
    for (;;) {
    	Move mv = g_map[hkey];
	    if( mv == Move(0,0) ) break;
	    mvs.insert(mvs.begin(), mv);		//	手数は少ないのでおｋ
	    bd.unMove(mv);
		hkey = bd.hkeyText();
    }
	cout << bd.text() << "\n";
	int cnt = 0;
    for(const auto& mv: mvs) {
	    cout << "#" << ++cnt << " Move: " << mv.text() << "\n";
	    bd.doMove(mv);
		cout << bd.text() << "\n";
    }
#if	0
	cout << "hkey = " << bd.hkeyHex() << "\n";
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.text() << " ";
	cout << "\n\n";
#endif
	return true;
}
//	現状態から、評価値が増加する状態を探す
bool do_search(Board& bd, vector<string>& hist)
{
	if( bd.nCardHome() >= 52) return false;
	//const int N_MOVABLE = 6;
	//const int nCardHome0 = bd.nCardHome();		//	現ホームカード枚数
	const int ev0 = bd.eval();
	auto start = std::chrono::system_clock::now();
	auto hktxt = bd.hkeyText();
	bool found = false;
	string hkey;
	int mxev = 0;		//	最大評価値
	string mxhk;		//	最大評価値を与える局面ハッシュテキスト
	g_map.clear();
	g_map[hktxt] = Move(0,0);
	vector<string> lst, lst2;
	lst.push_back(hktxt);
	for (int n = 1; n <= 6; ++n) {		//	最大6手探索
		lst2.clear();	//	末端ノード
		for(const auto& txt: lst) {
			bd.set(txt);
			assert(bd.checkNCard());
#if	0
			Move mv;
			while( bd.genSafeMove(mv) )
				bd.doMove(mv);
#endif
			Moves mvs;
			bd.genMoves(mvs);
			if( lst.empty() ) continue;
			for(const auto& mv: mvs) {
				bd.doMove(mv);
				if( !bd.checkNCard() )
					cout << bd.text() << "\n";
				assert( bd.checkNCard() );
				auto hk = bd.hkeyText();
				if( g_map.find(hk) == g_map.end() ) {
					auto ev = bd.eval();
					if( ev > mxev && ev != ev0 ) {
						auto hk = bd.hkeyText();
						//if( hk != hist.back() )
						if( std::find(hist.begin(), hist.end(), hk) == hist.end() )
						{		//	ループ禁止
							//mxhk = bd.hkeyText();
							mxhk = hk;
							mxev = ev;
						}
					}
					g_map[hk] = mv;
					lst2.push_back(hk);
				}
				bd.unMove(mv);
				assert( bd.checkNCard() );
				//if( found ) break;
			}
		}
		lst.swap(lst2);		//	末端ノードリストを lst に転送
		cout << n << ": lst.size() = " << lst.size() << "\n";
		//if( found ) break;
		if( lst.size() >= 1024*512 ) break;		//	末端ノード数が0.5Mを超えた場合は探索終了
		//if( lst.size() >= 1024*1024 ) break;		//	末端ノード数が１Mを超えた場合は探索終了
	}
	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = end - start;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "dur: " << msec << "msec\n";
    //if( !found ) return false;
    //	手順表示
    hkey = mxhk;
	bd.set(mxhk);
	Moves mvs;
    for (;;) {
    	Move mv = g_map[hkey];
	    if( mv == Move(0,0) ) break;
	    mvs.insert(mvs.begin(), mv);		//	手数は少ないのでおｋ
	    bd.unMove(mv);
		hkey = bd.hkeyText();
    }
	cout << bd.text() << "\n";
	int cnt = 0;
    for(const auto& mv: mvs) {
	    cout << "#" << ++cnt << " Move: " << mv.text() << "\n";
	    bd.doMove(mv);
		cout << bd.text() << "\n";
    }
#if	0
	cout << "hkey = " << bd.hkeyHex() << "\n";
	bd.genMoves(mvs);
	cout << "Moves: ";
	for(const auto& mv: mvs) cout << mv.text() << " ";
	cout << "\n\n";
#endif
	//
	//cout << "hkey = " << bd.hkeyHex() << "\n";
	hist.push_back(bd.hkeyText());
	//
	return true;
}
void test_120811()
{
	Board bd;
	string hk0 = {0x25,0x00,0x00,0x00,0x01,0x01,0x01,0x05,0x0c,0x08,0x3c,0x17,0x06,0x05,0x27,0x00,0x0b,0x2d,0x22,0x04,
						 0x02,0x36,0x2a,0x19,0x28,0x00,0x00,0x07,0x1d,0x37,0x14,0x13,0x29,0x00,0x0d,0x2c,0x1b,0x3a,0x09,0x38,
						 0x00,0x00,0x23,0x24,0x2b,0x26,0x03,0x00,0x1a,0x16,0x15,0x18,0x12,0x3d,0x1c,0x3b,0x0a,0x39,0x00,};
	bd.set(hk0);
	cout << bd.text() << "\n";
	cout << "eval = " << bd.eval() << "\n\n";
	vector<string> hist;		//	中間目標リスト
	hist.push_back(hk0);
	for (int i = 0; i < 10; ++i) {
		//cout << "i = " << i << "\n";
		//cout << bd.hkeyHex() << "\n";
		do_search(bd, hist);
		cout << "eval = " << bd.eval() << "\n\n";
	}
}
void test_search()
{
	auto seed = g_rd();
	g_mt = mt19937{7};		//	乱数シード指定
	Board bd;
	vector<string> hist;		//	中間目標リスト
	searchMovable6(bd);		//	初期状態から降順列６枚以上移動可能状態を探す
	hist.push_back(bd.hkeyText());
	int cnt = 0;
	while( bd.nCardHome() < 52 && ++cnt < 200 ) {
		cout << "cnt = " << cnt << "\n";
		//if( !searchHomePlusMovable6(bd) )		//	現状態から、降順列６枚以上移動可能 かつ Home枚数が増えた状態を探す
		Move mv;
		while( bd.genSafeMove(mv) ) {
			cout << "Move: " << mv.text() << "\n";
			bd.doMove(mv);
			cout << bd.text() << "\n";
			cout << "eval = " << bd.eval() << "\n";
		}
		cout << bd.hkeyHex() << "\n";
		if( !do_search(bd, hist) )		//	現状態から、評価値が増加する状態を探す
			break;
		cout << "eval = " << bd.eval() << "\n";
	}
	cout << "seed = " << seed << "\n";
}
void	test_genMove()
{
	if (false) {
		g_mt = mt19937{1};
		Board bd;
		cout << bd.text() << "\n";
	}
	if (false) {
		g_mt = mt19937{1};
		Board bd;
		cout << bd.text() << "\n";
	}
	if (true) {
		Board bd;
		string hk0 = { 0x03,0x14,0x00,0x00,0x00,0x02,0x06,0x02,0x2b,0x3d,0x1d,0x01,0x2a,0x15,0x28,0x17,0x36,0x05,0x00,0x27,
							0x13,0x34,0x19,0x16,0x0c,0x00,0x00,0x0a,0x29,0x08,0x37,0x06,0x35,0x04,0x33,0x02,0x00,0x00,0x3c,0x0b,
							0x00,0x2d,0x1c,0x3b,0x1a,0x39,0x18,0x00,0x0d,0x2c,0x1b,0x3a,0x09,0x38,0x07,0x00, };
		bd.set(hk0);
		cout << bd.text() << "\n";
		Moves mvs;
		bd.genMoves(mvs);
		cout << "Moves: ";
		for (const auto& mv : mvs) cout << mv.text() << " ";
		cout << "\n\n";
	}
}
//	１枚移動手のみ生成
void	test_genMove1()
{
	if (true) {
		Board bd;
		string hk0 = {0x00,0x00,0x00,0x00,0x07,0x06,0x06,0x07,0x29,0x08,0x00,0x1d,0x3c,0x0b,0x2a,0x19,0x28,0x17,0x00,0x3d,
							0x1c,0x3b,0x0a,0x39,0x18,0x27,0x00,0x00,0x0d,0x2c,0x1b,0x3a,0x09,0x38,0x00,0x00,0x2d,0x0c,0x2b,0x00,0x1a,0x00};
		bd.set(hk0);
		cout << bd.text() << "\n";
		Moves mvs;
		bd.genMoves1(mvs);
		cout << "Moves: ";
		for (const auto& mv : mvs) cout << mv.text() << " ";
		cout << "\n\n";
	}
	if (true) {
		Board bd;
		string hk0 = {0x1d,0x3a,0x00,0x00,0x09,0x09,0x07,0x09,0x0d,0x3c,0x1b,0x2a,0x00,0x2d,0x00,0x0c,0x2b,0x1a,0x00,0x00,
							0x3d,0x1c,0x3b,0x0a,0x29,0x00,0x0b,0x00,0x00,0x28,0x2c,0x00,};		//	乱数シード１で不正になる局面
		bd.set(hk0);
		cout << bd.text() << "\n";
		Moves mvs;
		bd.genMoves1(mvs);
		cout << "Moves: ";
		for (const auto& mv : mvs) cout << mv.text() << " ";
		cout << "\n\n";
	}
	if (true) {
		Board bd;
		string hk0 = { 0x18,0x19,0x00,0x00,0x01,0x02,0x00,0x07,0x39,0x15,0x0d,0x27,0x08,0x1d,0x2c,0x0b,0x3a,0x09,0x28,0x17,
							0x26,0x05,0x00,0x16,0x13,0x06,0x25,0x04,0x23,0x00,0x3c,0x03,0x38,0x21,0x3d,0x1c,0x00,0x2d,0x0c,0x2b,
							0x0a,0x00,0x2a,0x29,0x1a,0x02,0x22,0x00,0x24,0x1b,0x07,0x3b,0x14,0x00,0x00,0x00, };
		bd.set(hk0);
		cout << bd.text() << "\n";
		Moves mvs;
		bd.genMoves1(mvs);
		cout << "Moves: ";
		for (const auto& mv : mvs) cout << mv.text() << " ";
		cout << "\n\n";
	}
}
void test_eval()
{
	if (true) {
		Board bd;
		string hk0 = {0x1d,0x3a,0x00,0x00,0x09,0x09,0x07,0x09,0x0d,0x3c,0x1b,0x2a,0x00,0x2d,0x00,0x0c,0x2b,0x1a,0x00,0x00,
							0x3d,0x1c,0x3b,0x0a,0x29,0x00,0x0b,0x00,0x00,0x28,0x2c,0x00,};		//	乱数シード１で不正になる局面
		bd.set(hk0);
		cout << bd.text() << "\n";
		cout << "eval = " << bd.eval() << "\n";
		cout << "\n";
	}
}
