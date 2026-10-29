
#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <memory>

using std::unique_ptr;
using std::cout;
using std::endl;
using std::list;
using std::find;
using std::string;
class Observer;
class Subject {
public:
	virtual void attach(Observer *pObserver) = 0;
	virtual void detach(Observer *pObserver) = 0;
	virtual void notify()                    = 0;
	virtual ~Subject() {}
};
class ConcreteSubject
    : public Subject {
public:
	void attach(Observer *pObserver) override;
	void detach(Observer *pObserver) override;
	void notify() override;
	void setStatus(int status)
	{
		_status = status;
	}
	int getStatus() const
	{
		return _status;
	}

private:
	list<Observer *> _obList;
	int              _status;
};
class Observer {
public:
	virtual void update(int) = 0;
	virtual ~Observer() {}
};
class ConcreteObserver
    : public Observer {
public:
	ConcreteObserver(const string &name)
	    : _name(name)
	{
	}
	void update(int value)
	{
		cout << "ConcreteObserver " << _name << ", value = " << value << endl;
	}

private:
	string _name;
};
class ConcreteObserver2
    : public Observer {
public:
	ConcreteObserver2(const string &name)
	    : _name(name)
	{
	}
	void update(int value)
	{
		cout << "ConcreteObserver2 " << _name << ", value = " << value << endl;
	}

private:
	string _name;
};
void ConcreteSubject::attach(Observer *pObserver)
{
	if(pObserver) {
		_obList.push_back(pObserver);
	}
}

void ConcreteSubject::detach(Observer *pObserver)
{
	if(pObserver) {
		_obList.remove(pObserver);
	}
}
void ConcreteSubject::notify()
{
	for(auto &ob : _obList) {
		ob->update(_status);
	}
}
void test()
{
	unique_ptr<ConcreteSubject> pSubject(new ConcreteSubject());
	unique_ptr<Observer>        pObserver(new ConcreteObserver("lili"));
	unique_ptr<Observer>        pObserver2(new ConcreteObserver2("lucy"));
	pSubject->setStatus(2);
	pSubject->attach(pObserver.get());
	pSubject->attach(pObserver2.get());
	pSubject->notify();
	pSubject->detach(pObserver2.get());
	pSubject->setStatus(3);
	pSubject->notify();
}

// 优点：
// 1、观察者和被观察者是抽象耦合的 2、建立一套触发机制。 缺点 1、如果一个被观察者对象有很多的直接和间接的观察者的话，将所有的观察者都通知到会花费很多时 间。 2、如果在观察者和观察目标之间有循环依赖的话，观察目标会触发它们之间进行循环调用，可能导致 系统崩溃。 3、观察者模式没有相应的机制让观察者知道所观察的目标对象是怎么发生变化的，而仅仅只是知道观 察目标发生了变化。
//     应用场景 1、一个抽象模型有两个方面，其中一个方面发依赖于另外一个方面。将这些方面封装在独立的对象中 使它们可以各自独立地改变和复用。 2、一个对象的改变将导致其它一个或多个对象发生改变，而不知道具体有多少对象将发生改变，可以 降低对象之间的耦合度。 3、一个对象必须通知其他对象，而并不知道这些对象是谁。 4、需要在系统中创建一个触发链，A对象的行为将影响B对象，B对象的行为将影响C对象……，可以使 用观察者模式创建一种链式触发机制