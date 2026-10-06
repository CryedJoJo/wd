#ifndef SUB_H
#define SUB_H
#include "BaseOperator.h"
#include <iostream>
#include <utility>

#define TITLE template <class T1, class T2, class T3>

TITLE
class sub : public BaseOperator {

public:
	sub() { std::cout << "sub()" << std::endl; }
	sub(T1 lhs, T2 rhs);
	sub(const sub &other) = delete;

	sub(sub &&other) noexcept
	    : BaseOperator(std::move(other))
	    , lhs_(std::move(other.lhs_))
	    , rhs_(std::move(other.rhs_))
	    , answer_(std::move(other.answer_))
	{
		std::cout << "sub(sub&&)" << std::endl;
	}

	sub &operator=(const sub &other)
	{
		if(this != &other) {
			lhs_    = other.lhs_;
			rhs_    = other.rhs_;
			answer_ = other.answer_;
		}
		std::cout << "operator=(const sub&)" << std::endl;
		return *this;
	}

	sub &operator=(sub &&other) noexcept
	{
		if(this != &other) {
			lhs_    = std::move(other.lhs_);
			rhs_    = std::move(other.rhs_);
			answer_ = std::move(other.answer_);
		}
		std::cout << "operator=(sub&&)" << std::endl;
		return *this;
	}

	~sub();

	virtual void reslut() override;

private:
	/* data */
	T1 lhs_;
	T2 rhs_;
	T3 answer_;
};

TITLE
sub<T1, T2, T3>::sub(T1 lhs, T2 rhs)
    : lhs_(lhs)
    , rhs_(rhs)
{
	std::cout << "sub(T1, T2)" << std::endl;
}

TITLE
sub<T1, T2, T3>::~sub()
{
	std::cout << "~sub()" << std::endl;
}

TITLE
void sub<T1, T2, T3>::reslut()
{
	answer_ = lhs_ - rhs_;
	std::cout << answer_ << std::endl;
}

#endif
