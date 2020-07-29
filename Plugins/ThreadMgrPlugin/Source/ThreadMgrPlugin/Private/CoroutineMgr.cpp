// Fill out your copyright notice in the Description page of Project Settings.


#include "CoroutineMgr.h"

FCoroutineMgr::FCoroutineMgr()
{

}

FCoroutineMgr::~FCoroutineMgr()
{
	StopAllCoroutines();
}

Coroutine* FCoroutineMgr::StartCoroutine(Enumerator InEnumerator)
{
	CoroPull Pull(InEnumerator);
	if (Pull) {
		return PushCoroutine(new Coroutine(Pull));
	}
	return nullptr;
}

void FCoroutineMgr::StopCoroutine(Coroutine* CoroutinePtr)
{
	if (Coroutines.Contains(CoroutinePtr)) {
		YieldInstruction* YieldReturnPtr = nullptr;
		while (CoroutinePtr != nullptr)
		{
			YieldReturnPtr = CoroutinePtr->GetYieldReturn();
			CoroutinePtr->End();
			CoroutinePtr = CastAsCoroutine(YieldReturnPtr);
		}
		if (YieldReturnPtr != nullptr)
		{
			YieldReturnPtr->End();
		}
	}
}

void FCoroutineMgr::StopAllCoroutines()
{
	for (Coroutine* CoroutinePtr : Coroutines)
	{
		StopCoroutine(CoroutinePtr);
	}
}

bool FCoroutineMgr::HasCoroutines()
{
	return Coroutines.Num() > 0;
}

Coroutine* FCoroutineMgr::CastAsCoroutine(YieldInstruction* YieldInstructionPtr)
{
	if (YieldInstructionPtr == nullptr || YieldInstructionPtr->Type != TYPEID(Coroutine))
	{
		return nullptr;
	}
	return static_cast<Coroutine*>(YieldInstructionPtr);
}

bool FCoroutineMgr::YieldCoroutine(Coroutine* CoroutinePtr)
{
	YieldInstruction* YieldReturnPtr = CoroutinePtr->GetYieldReturn();
	if (YieldReturnPtr != nullptr)
	{
		if (YieldReturnPtr->IsEnded())
		{
			delete YieldReturnPtr;
		}
		else {
			Coroutines.Emplace(CoroutinePtr);
			return true;
		}
	}
	return false;
}

Coroutine* FCoroutineMgr::PushCoroutine(Coroutine* CoroutinePtr)
{
	Coroutine* YieldReturnPtr = CastAsCoroutine(CoroutinePtr->GetYieldReturn());
	if (YieldReturnPtr == nullptr)
	{
		Coroutines.Emplace(CoroutinePtr);
	}
	else {
		YieldReturnPtr->NextPtr = CoroutinePtr;
	}
	return CoroutinePtr;
}

Coroutine* FCoroutineMgr::PopCoroutine(Coroutine* CoroutinePtr)
{
	//Coroutine* NextPtr = CoroutinePtr->NextPtr;
	//delete CoroutinePtr;
	//return NextPtr;
	Coroutines.Remove(CoroutinePtr);
	delete CoroutinePtr;
	return nullptr;
}

void FCoroutineMgr::ResumeCoroutines()
{
	TSet<Coroutine*> CoroutinesCopy;
	CoroutinesCopy = Coroutines;
	Coroutines.Empty();

	for (Coroutine* CoroutinePtr : CoroutinesCopy)
	{
		if (YieldCoroutine(CoroutinePtr))
		{
			continue;
		}
		while (CoroutinePtr != nullptr)
		{
			// 否则StopCoroutine后继续执行一次
			if (!CoroutinePtr->IsEnded()) {
				CoroutinePtr->Resume();
			}
			if (!CoroutinePtr->IsEnded())
			{
				PushCoroutine(CoroutinePtr);
				break;
			}
			CoroutinePtr = PopCoroutine(CoroutinePtr);
		}
	}
}