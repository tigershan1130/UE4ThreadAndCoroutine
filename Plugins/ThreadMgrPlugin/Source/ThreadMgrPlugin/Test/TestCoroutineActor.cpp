// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCoroutineActor.h"

// Sets default values
ATestCoroutineActor::ATestCoroutineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Instance = this;
}

// Called when the game starts or when spawned
void ATestCoroutineActor::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), AThreadMgr::StaticClass());
	ThreadMgr = Cast<AThreadMgr>(Actor);
	UE_LOG(LogTemp, Log, TEXT("[ThreadMgrPlugin] Actor:%p ThreadMgr:%p"), Actor, ThreadMgr);

	//CoroutineActor->StartCoroutine(TaskA());

}

// Called every frame
void ATestCoroutineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



Enumerator ATestCoroutineActor::TaskA()
{
	return [=](CoroPush& yield_return)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "TaskA A1");
		yield_return(nullptr);
		UKismetSystemLibrary::PrintString(GetWorld(), "TaskA A2");
	};
}

Enumerator ATestCoroutineActor::TaskB(float Param)
{
	return [=](CoroPush& yield_return)
	{
		while (true) {
			PrintString("TaskB B1", 5.f, FLinearColor::Red);
			yield_return(new WaitForSeconds(Param));

			PrintString("TaskB B2", 5.f, FLinearColor::Red);
			yield_return(new WaitForSeconds(Param));

			PrintString("TaskB B3", 5.f, FLinearColor::Red);
		}
	};
}

Enumerator ATestCoroutineActor::EnumeratorCube(float Seconds)
{
	return [=](CoroPush& yield_return)
	{
		while (true) {
			AActor* InterCube = GetWorld()->SpawnActor(CubeClass);
			float xPos = FMath::FRandRange(-400.f, 500.f);
			float yPos = FMath::FRandRange(-400.f, 400.f);
			float zPos = FMath::FRandRange(120.f, 400.f);
			InterCube->SetActorLocation(FVector(xPos, yPos, zPos));
			InterCube->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
			PrintString("CoroutineCube SetActorScale3D", 5.f);
			yield_return(new WaitForSeconds(Seconds));
			InterCube->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
			InterCube->SetActorRotation(FRotator(0, 60.0f, 0));
			PrintString("CoroutineCube SetActorRotation", 5.f);
			yield_return(new WaitForSeconds(Seconds));
			InterCube->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
			yield_return(new WaitForSeconds(Seconds));
			InterCube->Destroy();
			yield_return(new WaitForSeconds(Seconds));
		}
	};
}

void ATestCoroutineActor::StartCoroutineCube()
{
	CoroutineCube = ThreadMgr->StartCoroutine(EnumeratorCube(1.0f));
}

void ATestCoroutineActor::StopCoroutineCube()
{
	ThreadMgr->StopCoroutine(CoroutineCube);
}

void ATestCoroutineActor::StopCoroutineTaskB()
{
	//UE_LOG(LogTemp, Log, TEXT("[ThreadMgrPlugin] DeleteCoroutineB %p"), CoroutineB);
	ThreadMgr->StopCoroutine(CoroutineTaskB);
}

void ATestCoroutineActor::StartCoroutineTaskB()
{
	CoroutineTaskB = ThreadMgr->StartCoroutine(TaskB(1.0f));
}

void ATestCoroutineActor::StopAllCoroutine()
{
	ThreadMgr->StopAllCoroutines();
}

bool ATestCoroutineActor::IsCoroutineRunning()
{
	return ThreadMgr->HasCoroutines();
}