#include "BaseOperator.h"
#include "add.h"
#include "divi.h"
#include "sub.h"
#include "mult.h"
#include <iostream>
#include <memory>
#include <vector>

#define TYPE int
#define OPEN_NUM1

// #define OPEN_NUM2

#define NUM num

using namespace std;
using sPtrA = shared_ptr<add<TYPE, TYPE, TYPE>>;
using sPtrD = shared_ptr<divi<TYPE, TYPE, TYPE>>;
using sPtrS = shared_ptr<sub<TYPE, TYPE, TYPE>>;
using sPtrM = shared_ptr<mult<TYPE, TYPE, TYPE>>;

void result(BaseOperator *p)
{
	p->reslut();
}

void test1()
{
	// shared_ptr<add<int, int, int>> spa = make_shared<add<int, int, int>>(add<int,int,int> a(1,1)); 这样写会多创建一个对象
	sPtrA spa = make_shared<add<TYPE, TYPE, TYPE>>(1, 1); //这样就只有一个对象
}

void test2()
{
	vector<TYPE> num{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

#ifdef OPEN_NUM2
	vector<TYPE> num2{1.1, 2.3, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
#endif
	vector<sPtrA> vspa;
	for(size_t i = 0; i < NUM.size() - 1; ++i) {
		vspa.emplace_back(make_shared<add<TYPE, TYPE, TYPE>>(NUM[i], NUM[i + 1]));
	}

	for(auto &elm : vspa) {
		result(elm.get());
	}
}

void test3()
{

	vector<TYPE> num{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#ifdef OPEN_NUM2
	vector<TYPE> num2{1.1, 2.3, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
#endif
	vector<sPtrD> vspd;
	for(size_t i = 0; i < NUM.size() - 1; ++i) {
		vspd.emplace_back(make_shared<divi<TYPE, TYPE, TYPE>>(NUM[i], NUM[i + 1]));
	}

	for(auto &elm : vspd) {
		result(elm.get());
	}
}

void test4()
{
	vector<TYPE> num{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#ifdef OPEN_NUM2
	vector<TYPE> num2{1.1, 2.3, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
#endif
	vector<sPtrS> vsps;
	for(size_t i = 0; i < NUM.size() - 1; ++i) {
		vsps.emplace_back(make_shared<sub<TYPE, TYPE, TYPE>>(NUM[i], NUM[i + 1]));
	}

	for(auto &elm : vsps) {
		result(elm.get());
	}
}

void test5()
{
	vector<TYPE> num{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#ifdef OPEN_NUM2
	vector<TYPE> num2{1.1, 2.3, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
#endif

	vector<sPtrM> vspm;
	for(size_t i = 0; i < NUM.size() - 1; ++i) {
		vspm.emplace_back(make_shared<mult<TYPE, TYPE, TYPE>>(NUM[i], NUM[i + 1]));
	}

	for(auto &elm : vspm) {
		result(elm.get());
	}
}

int main()
{
	// test1();
	test2();
	test3();
	test4();
	test5();

	return 0;
}
