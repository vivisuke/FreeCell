#pragma once

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
#define		NUM_MASK		0x0f
#define		N_CARD			(4*13)
#define		N_COL				4
#define		N_NUM				13

typedef const char cchar;
typedef unsigned char uint8;
typedef unsigned char card_t;

//	位置：
//		ゴール：			'G'
//		フリーセル：	移動先の場合：'F'、移動元の場合： 'A'～'D'
//		カスケード：	'0'～'7'
struct Move {
public:
	Move(char src = 0, char dst = 0)
		: m_src(src)
		, m_dst(dst)
	{
	}
public:
	char		m_src;		//	移動元
	char		m_dst;		//	移動先
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
	bool	checkNCard() const;				//	カード数チェック
	int	nEmptyColumns() const;			//	空欄カラム数を返す
	int	nMobableDesc() const;			//	移動可能降順列数を返す
	void	genMoves(Moves&) const;		//	可能着手生成
public:
	void	init();		//	初期化・カードを配る
	void	set(const std::string&);
	void	doMove(const Move&);
	void	unMove(const Move&);
private:
	int			m_nFreeCell;		//	フリーセルのカード数
	card_t		m_freeCell[N_FREECELL+1];		//	+1 for 番人
	card_t		m_home[N_HOME];
	std::vector<card_t>	m_column[N_COLUMN];
};
