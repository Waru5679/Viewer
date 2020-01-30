#pragma once
#include <vector>
#include <list>
using namespace std;

//ベクター解放
template <typename T>
static void VectorRelease(vector<T>& vec)
{
	vec.shrink_to_fit();
	vec.resize(0);
}

//ポインタ解放
static void PointerRelease(void* p)
{
	delete p;
	p = nullptr;
}