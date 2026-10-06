#ifndef ADD_H
#define ADD_H
#include "BaseOperator.h"
#include <iostream>
#include <utility>

#define TITLE template <class T1, class T2, class T3>

TITLE
class add : public BaseOperator {

public:
	add() { std::cout << "add()" << std::endl; }
	add(T1 lhs, T2 rhs);
	add(const add &other) = delete;

	add(add &&other) noexcept
	    : BaseOperator(std::move(other))
	    , lhs_(std::move(other.lhs_))
	    , rhs_(std::move(other.rhs_))
	    , answer_(std::move(other.answer_))
	{
		std::cout << "add(add&&)" << std::endl;
	}

	add &operator=(const add &other)
	{
		if(this != &other) {
			lhs_    = other.lhs_;
			rhs_    = other.rhs_;
			answer_ = other.answer_;
		}
		std::cout << "operator=(const add&)" << std::endl;
		return *this;
	}

	add &operator=(add &&other) noexcept
	{
		if(this != &other) {
			lhs_    = std::move(other.lhs_);
			rhs_    = std::move(other.rhs_);
			answer_ = std::move(other.answer_);
		}
		std::cout << "operator=(add&&)" << std::endl;
		return *this;
	}

	~add();

	virtual void reslut() override;

private:
	/* data */
	T1 lhs_;
	T2 rhs_;
	T3 answer_;
};

TITLE
add<T1, T2, T3>::add(T1 lhs, T2 rhs)
    : lhs_(lhs)
    , rhs_(rhs)
{
	std::cout << "add(T1, T2)" << std::endl;
}

TITLE
add<T1, T2, T3>::~add()
{
	std::cout << "~add()" << std::endl;
}

TITLE
void add<T1, T2, T3>::reslut()
{
	answer_ = lhs_ + rhs_;
	std::cout << answer_ << std::endl;
}

#endif