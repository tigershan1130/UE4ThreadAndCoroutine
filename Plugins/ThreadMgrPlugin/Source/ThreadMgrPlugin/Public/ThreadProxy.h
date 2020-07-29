// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runnable.h"
#include "RunnableThread.h"
#include "ThreadSafeBool.h"

DECLARE_DELEGATE(FThreadDelegate);
typedef TFunction<void()> FThreadLambda;

/**
 * 
 */
class THREADMGRPLUGIN_API FThreadProxy : public FRunnable
{
public:
	FThreadProxy(FString InThreadName, FThreadDelegate InThreadDelegate);
	FThreadProxy(FString InThreadName, FThreadLambda InThreadLambda);
	~FThreadProxy();

	void PauseThread();
	void ContinueThread();
	void StopThread();

	bool IsPaused();
	bool IsKilled();

public:
	FString ThreadName;

private:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool bSelfCtrl; // 是否自己控制while
	FThreadSafeBool bKill; 
	FThreadSafeBool bPause;

	FThreadDelegate ThreadDelegate;
	FThreadLambda ThreadLambda;

	FRunnableThread* Thread;
	FCriticalSection Mutex;

};
