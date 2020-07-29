// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ThreadProxy.h"
#include "ThreadMgr.h"

#include "TestThreadActor.generated.h"

UCLASS()
class THREADMGRPLUGIN_API ATestThreadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestThreadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;


	UFUNCTION(BlueprintCallable, Category = "TestThread")
	void CreateThread();
	UFUNCTION(BlueprintCallable, Category = "TestThread")
	void PauseThread();
	UFUNCTION(BlueprintCallable, Category = "TestThread")
	void ContineThread();
	UFUNCTION(BlueprintCallable, Category = "TestThread")
	void StopThread();
private:
	AThreadMgr* ThreadMgr;
	int32 Counter;

	void ThreadRunFunc();
};
