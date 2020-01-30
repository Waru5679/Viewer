#pragma once
#include <vector>
#include <list>
using namespace std;

//�x�N�^�[���
template <typename T>
static void VectorRelease(vector<T>& vec)
{
	vec.shrink_to_fit();
	vec.resize(0);
}

//�|�C���^���
static void PointerRelease(void* p)
{
	delete p;
	p = nullptr;
}