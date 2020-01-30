#pragma once

//RECT構造体
struct RECT_F
{
	//コンストラクタ
	RECT_F() {}
	RECT_F(float Top, float Left, float Width, float Height)
	{
		m_Top = Top;
		m_Left = Left;
		m_Right = Left + Width;
		m_Bottom = Top + Height;
	}

	float m_Top;	//上
	float m_Left;	//左
	float m_Right;	//右
	float m_Bottom;	//下
};

//色データ
class CColorData
{
public:
	//コンストラクタ
	CColorData() {}
	CColorData(float R, float G, float B, float A)
	{
		m_Red = R;
		m_Green = G;
		m_Blue = B;
		m_Alpha = A;
	}

	//開放
	void Release() {};
	
	float m_Red;	//赤
	float m_Green;	//緑
	float m_Blue;	//青
	float m_Alpha;	//透過
};