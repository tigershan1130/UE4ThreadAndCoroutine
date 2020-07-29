// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoroDefinition.h"

/**
 * 
 */
class THREADMGRPLUGIN_API FCoroutineMgr
{
public:
	FCoroutineMgr();
	~FCoroutineMgr();


private:
	//std::set<Coroutine*> Coroutines;
	TSet<Coroutine*> Coroutines;
	//std::set<Coroutine*> Coroutines2;

public:
	Coroutine* StartCoroutine(Enumerator InEnumerator);
	void StopCoroutine(Coroutine* CoroutinePtr);
	void StopAllCoroutines();
	bool HasCoroutines();
	void ResumeCoroutines();

private:
	Coroutine* CastAsCoroutine(YieldInstruction* YieldInstructionPtr);
	bool YieldCoroutine(Coroutine* CoroutinePtr);
	Coroutine* PushCoroutine(Coroutine* CoroutinePtr);
	Coroutine* PopCoroutine(Coroutine* CoroutinePtr);

};
