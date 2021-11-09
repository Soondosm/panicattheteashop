// Fill out your copyright notice in the Description page of Project Settings.
#include "CollidingPawnMovementComponent.h"
#include "EngineUtils.h"
#include "PlanetOne.h"
#include "ToolBuilderUtil.h"
#include "Kismet/GameplayStatics.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//make sure everything is still valid and that we can actually move still
	if(!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	TArray<APlanetOne*> Planets;
	// ToolBuilderUtil::FindAllActors(GetWorld(), Planets);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanetOne::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT("My Class is: %s"), *FoundActors[0]->GetFName().ToString());
	// Get (then clear) movement vector we set in ACollidingPawn::Tick
	FVector PlanetLoc = (FoundActors[0]->GetActorLocation() - GetActorLocation()).GetClampedToMaxSize(1.0f) + ConsumeInputVector();
	FVector DesiredMovementThisFrame = PlanetLoc.GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if(Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}

template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out)
{
	for (TActorIterator<T> It(World); It; ++It)
	{
		Out.Add(*It);
	}
}

