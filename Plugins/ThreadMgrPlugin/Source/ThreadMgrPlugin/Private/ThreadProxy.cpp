// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadProxy.h"
#include "Engine/Engine.h"

FThreadProxy::FThreadProxy(FString InThreadName, FThreadDelegate InThreadDelegate)
	: ThreadName(InThreadName)
	, ThreadDelegate(InThreadDelegate)
{
	bKill = false;
	bPause = false;
	Thread = FRunnableThread::Create(this, *ThreadName, 0, TPri_BelowNormal);
}

FThreadProxy::FThreadProxy(FString InThreadName, FThreadLambda InThreadLambda)
	: ThreadName(InThreadName)
	, ThreadLambda(InThreadLambda)
{
	bKill = false;
	bPause = false;
	Thread = FRunnableThread::Create(this, *ThreadName, 0, TPri_BelowNormal);
}

FThreadProxy::~FThreadProxy()
{
	if (Thread) {
		delete Thread;
		Thread = nullptr;
	}
}

void FThreadProxy::PauseThread()
{
	bPause = true;
}

void FThreadProxy::ContinueThread()
{
	bPause = false;
}

void FThreadProxy::StopThread()
{
	Stop();
	if (Thread)
	{
		Thread->WaitForCompletion();
	}
}

//需要注意的是在其他线程不能对UObject进行操作，不能使用TimerManager，不能使用DrawDebugLine。

bool FThreadProxy::IsPaused()
{
	return (bool)bPause;
}

bool FThreadProxy::IsKilled()
{
	return (bool)bKill;
}

bool FThreadProxy::Init()
{
	GLog->Log("FThreadProxy Init...");
	return true;
}

uint32 FThreadProxy::Run()
{
	while (!bKill)
	{
		if (bPause)
		{
			//GLog->Log("FThreadProxy Pause...");
			if (bKill) {
				return 0;
			}
		}
		else {
			//GLog->Log("FThreadProxy Loop2...");

			Mutex.Lock();
			// sync...
			if (ThreadDelegate.IsBound()) {
				ThreadDelegate.Execute();
			}
			else if (ThreadLambda) {
				ThreadLambda();
			}
			Mutex.Unlock();

		}
		//FPlatformProcess::Sleep(1);
	}
	return 0;
}

void FThreadProxy::Stop()
{
	bKill = true;
	bPause = false;
	GLog->Log("FThreadProxy Stop...");
}

void FThreadProxy::Exit()
{
	GLog->Log("FThreadProxy Exit...");
}