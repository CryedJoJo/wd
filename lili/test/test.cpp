#include <iostream>

class A {
public:
	A() = delete;
	A(int a)
    :pub_(a),
    protect_(a)
	{
		std::cout << "A("
		          << a << ")"
		          << std::endl;
        
                
	}

	~A()
	{
		std::cout << "~A()\n";
	}
public:
	int pub_;

protected:
	int protect_;

};

class B : private A {
public:
	B()
	    : A(1)
	{
		std::cout << "B()\n"
		          << pub_ << protect_ << std::endl;
	}

	~B()
	{
		std::cout << "~B()\n";
	}
};

class C : public B {
public:
	C()
	{
		std::cout << "C()\n"
		          << pub_ << protect_ << std::endl;
	}

	~C()
	{
		std::cout << "~C()\n";
	}
};

int main()
{
	C C;

	return 0;
}