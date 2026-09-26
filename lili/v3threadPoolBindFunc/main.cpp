#include "threadPool.h"
#include "TK.h"
#include <unistd.h>

#define TIME 20

void proc(int i)
{
	std::cout << "proc " << i << "runing..." << std::endl;
}

void test()
{

	threadPool tp(4, 10);
	int i = 0;
	while(i != TIME) {
		tp.addTask(std::move(std::bind(proc, ++i)));//不合理。bind返回的本身就是给右值，move没意义。

		/*std::move 最主要的作用，就是把“本来是左值、但我已经决定可以把资源交出去”的对象，转换成右值，让后面的移动语义有机会被使用
		所以：
		1、不要move纯右值
		2、move非纯右值时，得看之后的操作是否支持移动语义，不支持移动语义，你move了也没有意义。
		*/
	}
	std::cout << "add proc " << i << std::endl;

}

int main()
{
	test();
	return 0;
}