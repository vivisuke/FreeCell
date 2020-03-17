#pragma once

#include <string>
#include <vector>

#define		N_FREECELL		4			//	一時置き場の数
#define		N_CASCADE		8
#define		N_GOAL				4
#define		SPADE				0x00
#define		CLUB					0x10
#define		HEART				0x20
#define		DIAMOND			0x30
#define		COL_MASK			0xf0
#define		NUM_MASK		0x0f
#define		N_CARD			(4*13)
#define		N_COL				4
#define		N_NUM				13

typedef unsigned char uint8;

//	盤面？状態クラス
class Board {
public:
	Board();
public:
	std::string	text() const;
public:
	void	init();		//	初期化・カードを配る
private:
	uint8		m_freeCell[N_FREECELL];
	uint8		m_goal[N_GOAL];
	std::vector<uint8>	m_cascade[N_CASCADE];
};
