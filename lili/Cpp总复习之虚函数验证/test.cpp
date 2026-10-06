#include <iostream>

class A {
public:
	virtual void show()
	{
		std::cout << "A show" << std::endl;
	}
};

class B {
public:
	void show()
	{
		std::cout << "B show" << std::endl;
	}
};

class C : public A, public B {
public:
	// virtual void show() override
	// {
	// 	std::cout << "C virtual show" << std::endl;
	// }

	// void show()
	// {
	// 	std::cout << "C  show" << std::endl;
	// }
};

class D : public C {
public:
	void show() override
	{
		std::cout << "D show" << std::endl;
	}
};

int main()
{
	C *cPtr = nullptr;
	D  d;
	cPtr = &d;
	cPtr->show();
	C c;
	c.A::show();
	c.B::show();
	// c.show();

	return 0;
}