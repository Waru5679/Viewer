#pragma once

//必要なヘッダーファイルのインクルード
#include <windows.h>
#include <D3D10.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//ウインドウ名
static WCHAR szAppName[] = L"ライブラリ";

//ウインドウサイズ
constexpr int WINDOW_WIDTH{ 640 };
constexpr int WINDOW_HEIGHT{ 480 };

//フレームレート
constexpr int FRAME_RATE{ 60 };

//コールバック関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);