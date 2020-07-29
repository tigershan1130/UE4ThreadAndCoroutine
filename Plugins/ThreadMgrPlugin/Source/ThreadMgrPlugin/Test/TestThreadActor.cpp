// Fill out your copyright notice in the Description page of Project Settings.


#include "TestThreadActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATestThreadActor::ATestThreadActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestThreadActor::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), AThreadMgr::StaticClass());
	ThreadMgr = Cast<AThreadMgr>(Actor);
}

void ATestThreadActor::BeginDestroy()
{
	Super::BeginDestroy();
	
}

// Called every frame
void ATestThreadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestThreadActor::CreateThread()
{
	//FSimpleDelegate SimpleDelegate;
	//SimpleDelegate.BindUObject(this, &ATestThreadActor::ThreadRunFunc);

	auto lambda = [this]() {
		ThreadRunFunc();
	};

	ThreadMgr->CreateThread("TestRunnableActor", lambda);

}

void ATestThreadActor::PauseThread()
{
	ThreadMgr->PauseThread("TestRunnableActor");
}

void ATestThreadActor::ContineThread()
{
	ThreadMgr->ContinueThread("TestRunnableActor");
}

void ATestThreadActor::StopThread()
{
	ThreadMgr->StopThread("TestRunnableActor");
}

void ATestThreadActor::ThreadRunFunc()
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Delegate ATestRunnableActor::ThreadRunFunc... %d"), ++Counter));
	FPlatformProcess::Sleep(1);
}