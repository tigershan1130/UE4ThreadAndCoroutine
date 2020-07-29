// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ThreadMgr.h"

#include "TestCoroutineActor.generated.h"

UCLASS()
class THREADMGRPLUGIN_API ATestCoroutineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestCoroutineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CubeClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	Enumerator TaskA();
	Enumerator TaskB(float Param);
	Enumerator EnumeratorCube(float Seconds);

	
	UFUNCTION(BlueprintCallable, Category = "TestCoroutine")
	void StartCoroutineTaskB();
	UFUNCTION(BlueprintCallable, Category = "TestCoroutine")
	void StopCoroutineTaskB();
	UFUNCTION(BlueprintCallable, Category = "TestCoroutine")
	void StartCoroutineCube();
	UFUNCTION(BlueprintCallable, Category = "TestCoroutine")
	void StopCoroutineCube();
	UFUNCTION(BlueprintCallable, Category = "TestCoroutine")
	void StopAllCoroutine();
	UFUNCTION(BlueprintCallable, Category = "TestCoroutine")
	bool IsCoroutineRunning();

private:
	AThreadMgr* ThreadMgr;
	Coroutine* CoroutineCube;
	Coroutine* CoroutineTaskB;

	void PrintString(const FString& InString, float Duration = 2.f, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), InString, true, true, TextColor, Duration);
	}
};
