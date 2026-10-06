#ifndef MULT_H
#define MULT_H
#include "BaseOperator.h"
#include <iostream>
#include <utility>

#define TITLE template <class T1, class T2, class T3>

TITLE
class mult : public BaseOperator {

public:
	mult() { std::cout << "mult()" << std::endl; }
	mult(T1 lhs, T2 rhs);
	mult(const mult &other) = delete;

	mult(mult &&other) noexcept
	    : BaseOperator(std::move(other))
	    , lhs_(std::move(other.lhs_))
	    , rhs_(std::move(other.rhs_))
	    , answer_(std::move(other.answer_))
	{
		std::cout << "mult(mult&&)" << std::endl;
	}

	mult &operator=(const mult &other)
	{
		if(this != &other) {
			lhs_    = other.lhs_;
			rhs_    = other.rhs_;
			answer_ = other.answer_;
		}
		std::cout << "operator=(const mult&)" << std::endl;
		return *this;
	}

	mult &operator=(mult &&other) noexcept
	{
		if(this != &other) {
			lhs_    = std::move(other.lhs_);
			rhs_    = std::move(other.rhs_);
			answer_ = std::move(other.answer_);
		}
		std::cout << "operator=(mult&&)" << std::endl;
		return *this;
	}

	~mult();

	virtual void reslut() override;

private:
	/* data */
	T1 lhs_;
	T2 rhs_;
	T3 answer_;
};

TITLE
mult<T1, T2, T3>::mult(T1 lhs, T2 rhs)
    : lhs_(lhs)
    , rhs_(rhs)
{
	std::cout << "mult(T1, T2)" << std::endl;
}

TITLE
mult<T1, T2, T3>::~mult()
{
	std::cout << "~mult()" << std::endl;
}

TITLE
void mult<T1, T2, T3>::reslut()
{
	answer_ = lhs_ * rhs_;
	std::cout << answer_ << std::endl;
}

#endif
