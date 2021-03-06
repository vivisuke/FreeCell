﻿#pragma once

#include <string>
#include <vector>

#define		N_FREECELL		4			//	一時置き場の数
#define		N_COLUMN		8
#define		N_HOME			4
#define		SPADE				0x00
#define		CLUB					0x10
#define		HEART				0x20
#define		DIAMOND			0x30
#define		COL_MASK			0xf0
#define		BR_MASK			0x20
#define		NUM_BITS			4
#define		NUM_MASK		0x0f
#define		IX_SPADE			(SPADE>>NUM_BITS)
#define		IX_CLUB			(CLUB>>NUM_BITS)
#define		IX_HEART			(HEART>>NUM_BITS)
#define		IX_DIAMOND		(DIAMOND>>NUM_BITS)
#define		N_CARD			(4*13)
#define		N_COL				4
#define		N_NUM				13

typedef const char cchar;
typedef unsigned char uint8;
typedef unsigned char card_t;

uint8 cardNum(card_t c);
uint8 cardCol(card_t c);
uint8 cardColIX(card_t c);

//	位置：
//		ゴール：			'G'
//		フリーセル：	移動先の場合：'F'、移動元の場合： 'A'～'D'
//		カスケード：	'0'～'7'
struct Move {
public:
	Move(char src = 0, char dst = 0, char n = 1)
		: m_src(src)
		, m_dst(dst)
		, m_n(n)
	{
	}
public:
	bool	operator==(const Move& x) const
	{ return m_src == x.m_src && m_dst == x.m_dst && m_n == x.m_n; }
	std::string	text() const;
public:
	char		m_src;		//	移動元
	char		m_dst;		//	移動先
	char		m_n;			//	移動枚数（列→列の場合のみ有効）
};

typedef std::vector<Move> Moves;

//	盤面？状態クラス
class Board {
public:
	Board();
	Board(const Board&);
public:
	bool	operator==(const Board&) const;
	bool	operator!=(const Board& x) const { return !operator==(x); }
	std::string	text() const;
	std::string	hkeyText() const;			//	ハッシュキーテキスト
	std::string	hkeyHex() const;			//	ハッシュキー１６進数テキスト
	bool	checkNCard() const;				//	カード数チェック
	int	nCardHome() const;
	int	nCardFreeCell() const { return m_nCardFreeCell; }
	bool	isDescSeq(int clmn, int row) const;		//	clmn の row から末尾までは降順列か？
	int	nEmptyColumns() const;			//	空欄カラム数を返す
	int	nMobableDesc() const;			//	移動可能降順列数を返す
	int	nMobableDescToEmpty() const;	//	空列への移動可能降順列数を返す
	int	eval() const;							//	評価値を返す（移動可能枚数・ホームカード数・列評価）
	int	eval2() const;						//	評価値を返す（ホームカード数・列評価）
	card_t	getAt(char pos) const;
	bool	canMoveToHome(card_t) const;
	bool	canMoveTo(char pos, card_t) const;
	void	canPushBackList(std::vector<int>&, card_t cd, bool=true) const;		//	指定カードを追加可能なカラム番号リスト生成
	void	genMoves(Moves&) const;		//	可能着手生成
	void	genMoves1(Moves& /*, bool=true*/) const;		//	１枚のみ移動する可能着手生成
	bool	genSafeMove(Move&) const;		//	安全にホーム移動できる着手生成
	bool	isSafeToHome(card_t) const;		//	カードを安全にホーム移動できるか？
	int	genOpenClmnMoves(Moves&, int depth=10) const;		//	列を空ける着手を生成
	const std::vector<card_t>	getColumn(int ix) const { return m_column[ix]; }
public:
	void	init0();		//	初期化
	void	init();			//	初期化・カードを配る
	void	initMS(int);		//	MS#n 初期化・カードを配る
	void	initNoShuffle();		//	初期化・カードを配る
	card_t	popFrom(char pos);
	void	putTo(char pos, card_t cd);
	void	set(const std::string&);
	void	doMove(const Move&);
	void	unMove(const Move&);
private:
	int			m_nCardFreeCell;		//	フリーセルのカード数
	card_t		m_freeCell[N_FREECELL+1];		//	+1 for 番人
	card_t		m_homeCell[N_HOME];
	std::vector<card_t>	m_column[N_COLUMN];
};
