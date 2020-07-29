#pragma once

#include <ctime>
#include <functional>

THIRD_PARTY_INCLUDES_START
#define BOOST_DISABLE_ABI_HEADERS
#include "boost/coroutine2/all.hpp"
THIRD_PARTY_INCLUDES_END

// typeid without rtti
using TYPE_INFO = void*;

template <typename T> TYPE_INFO _TYPEID_()
{
	return reinterpret_cast<TYPE_INFO>(&_TYPEID_<T>);
}

#define TYPEID(T) _TYPEID_<T>()

class YieldInstruction
{
	//friend class CoroBehaviour;

public:
	TYPE_INFO Type;

public:
	YieldInstruction(TYPE_INFO InType)
		: Type(InType)
	{
	}

	virtual ~YieldInstruction()
	{
	}

	virtual void End()
	{
	}

	virtual bool IsEnded()
	{
		return true;
	}
};

class WaitForSeconds : public YieldInstruction
{
private:
	float Time;

	float Now()
	{
		return std::clock() / static_cast<float>(CLOCKS_PER_SEC);
	}
	
public:
	WaitForSeconds(float Seconds)
		: YieldInstruction(TYPEID(WaitForSeconds))
		, Time(Now() + Seconds)
	{
	}

public:
	virtual void End() override
	{
		Time = Now();
	}

	virtual bool IsEnded() override
	{
		return Time <= Now();
	}
};

class WaitWhile : public YieldInstruction
{
private:
	std::function<bool ()> Condition;

public:
	WaitWhile(std::function<bool ()> InCondition)
		: YieldInstruction(TYPEID(WaitWhile))
		, Condition(InCondition)
	{
	}

public:
	virtual void End() override
	{
		Condition = []()
		{
			return false;
		};
	}

	virtual bool IsEnded() override
	{
		return !Condition();
	}
};

class WaitUntil : public YieldInstruction
{
private:
	std::function<bool ()> Condition;

public:
	WaitUntil(std::function<bool ()> InCondition)
		: YieldInstruction(TYPEID(WaitWhile))
		, Condition(InCondition)
	{
	}

public:
	virtual void End() override
	{
		Condition = []()
		{
			return true;
		};
	}

	virtual bool IsEnded() override
	{
		return Condition();
	}
};

using CoroBoost = boost::coroutines2::coroutine<class YieldInstruction*>;
using CoroPull = CoroBoost::pull_type;
using CoroPush = CoroBoost::push_type;
using Enumerator = std::function<void (CoroPush&)>;

class Coroutine : public YieldInstruction
{
	//friend class CoroBehaviour;

private:
	CoroPull Pull;
	bool bIsFinished;

public:
	Coroutine* NextPtr;

public:
	Coroutine(CoroPull& InPull)
		: YieldInstruction(TYPEID(Coroutine))
		, Pull(std::move(InPull))
		, bIsFinished(false)
		, NextPtr(nullptr)
	{
	}

public:
	virtual void End() override
	{
		//Pull = CoroPull(Enumerator());
		bIsFinished = true;
	}

	virtual bool IsEnded() override
	{
		if (bIsFinished) {
			return true;
		}
		return !Pull;
	}

	void Resume()
	{
		Pull();
	}

	YieldInstruction* GetYieldReturn()
	{
		return Pull.get();
	}
};
