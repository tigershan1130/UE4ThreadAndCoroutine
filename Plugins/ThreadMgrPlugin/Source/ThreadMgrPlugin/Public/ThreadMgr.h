// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoroDefinition.h"
#include "CoroutineMgr.h"
#include "ThreadProxy.h"

#include "ThreadMgr.generated.h"

UCLASS()
class THREADMGRPLUGIN_API AThreadMgr : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThreadMgr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

// Coroutine
private:
	FCoroutineMgr CoroutimeMgr;
public:
	Coroutine* StartCoroutine(Enumerator InEnumerator);
	void StopCoroutine(Coroutine* CoroutinePtr);
	void StopAllCoroutines();
	bool HasCoroutines();


// Thread
private:
	TMap<FString, FThreadProxy*> ThreadMap;
public:
	FThreadProxy* CreateThread(const FString& InThreadName, FThreadDelegate InThreadDelegate);
	FThreadProxy* CreateThread(const FString& InThreadName, FThreadLambda InThreadLambda);
	void PauseThread(const FString& InThreadName);
	void ContinueThread(const FString& InThreadName);
	void StopThread(const FString& InThreadName);
	FThreadProxy* GetThreadProxy(const FString& InThreadName);
private:
	void ClearThreads();
};
