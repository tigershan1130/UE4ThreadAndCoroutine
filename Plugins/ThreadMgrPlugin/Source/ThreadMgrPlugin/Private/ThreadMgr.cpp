// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadMgr.h"

// Sets default values
AThreadMgr::AThreadMgr()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AThreadMgr::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThreadMgr::BeginDestroy()
{
	Super::BeginDestroy();
	ClearThreads();
}

// Called every frame
void AThreadMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasCoroutines()) {
		CoroutimeMgr.ResumeCoroutines();
	}
}

#pragma region Coroutine

Coroutine* AThreadMgr::StartCoroutine(Enumerator InEnumerator)
{
	return CoroutimeMgr.StartCoroutine(InEnumerator);
}

void AThreadMgr::StopCoroutine(Coroutine* CoroutinePtr)
{
	CoroutimeMgr.StopCoroutine(CoroutinePtr);
}

void AThreadMgr::StopAllCoroutines()
{
	CoroutimeMgr.StopAllCoroutines();
}

bool AThreadMgr::HasCoroutines()
{
	return CoroutimeMgr.HasCoroutines();
}

#pragma endregion Coroutine

#pragma region Thread

FThreadProxy* AThreadMgr::CreateThread(const FString& InThreadName, FThreadDelegate InThreadDelegate)
{
	if (!ThreadMap.Contains(InThreadName)) {
		auto ThreadProxy = new FThreadProxy(InThreadName, InThreadDelegate);
		ThreadMap.Emplace(InThreadName, ThreadProxy);
		UE_LOG(LogTemp, Log, TEXT("[ThreadMgr] CreateThread ThreadName:%s success..."), *InThreadName);
		return ThreadProxy;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[ThreadMgr] ThreadName:%s already exists..."), *InThreadName);
		return nullptr;
	}
}

FThreadProxy* AThreadMgr::CreateThread(const FString& InThreadName, FThreadLambda InThreadLambda)
{
	if (!ThreadMap.Contains(InThreadName)) {
		auto ThreadProxy = new FThreadProxy(InThreadName, InThreadLambda);
		ThreadMap.Emplace(InThreadName, ThreadProxy);
		UE_LOG(LogTemp, Log, TEXT("[ThreadMgr] CreateThread ThreadName:%s success..."), *InThreadName);
		return ThreadProxy;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[ThreadMgr] CreateThread ThreadName:%s already exists..."), *InThreadName);
		return nullptr;
	}
}

void AThreadMgr::PauseThread(const FString& InThreadName)
{
	if (auto ThreadProxy = ThreadMap.FindRef(InThreadName)) {
		ThreadProxy->PauseThread();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[ThreadMgr] PauseThread ThreadName:%s not exists..."), *InThreadName);
	}
}

void AThreadMgr::ContinueThread(const FString& InThreadName)
{
	if (auto ThreadProxy = ThreadMap.FindRef(InThreadName)) {
		ThreadProxy->ContinueThread();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[ThreadMgr] ContinueThread ThreadName:%s not exists..."), *InThreadName);
	}
}

void AThreadMgr::StopThread(const FString& InThreadName)
{
	if (auto ThreadProxy = ThreadMap.FindRef(InThreadName)) {
		//与delete一起会重复调用Stop，delete会自动调用Stop
		//ThreadProxy->StopThread();

		ThreadMap.Remove(InThreadName);
		delete ThreadProxy;
		ThreadProxy = nullptr;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[ThreadMgr] StopThread ThreadName:%s not exists..."), *InThreadName);
	}
}

FThreadProxy* AThreadMgr::GetThreadProxy(const FString& InThreadName)
{
	return ThreadMap.FindRef(InThreadName);
}

void AThreadMgr::ClearThreads()
{
	for (auto ele : ThreadMap) {
		ele.Value->StopThread();
	}
}

#pragma endregion Thread