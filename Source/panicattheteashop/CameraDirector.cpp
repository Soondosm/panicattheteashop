// Fill out your copyright notice in the Description page of Project Settings.
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CameraDirector.h"
// #include "CameraDirector.generated.h"

#include "CameraDirector.h"

// Sets default values
ACameraDirector::ACameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float TimeBetweenCameraChanges = 20.0f;
	const float  SmoothBlendTime = 0.75f; // who knows what this is?????
	TimeToNextCameraChange -= DeltaTime;
	if(TimeToNextCameraChange <= 0.0f)
	{
		TimeToNextCameraChange += TimeBetweenCameraChanges;
		// Find the actor that handles control for the local player ig
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (OurPlayerController)
		{
			if((OurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr))
			{
				// Cut instantly to camera one with some fuckin magic
				OurPlayerController -> SetViewTargetWithBlend(CameraOne, SmoothBlendTime);
			}
			else if ((OurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr))
			{
				// Blend smoothly to camera two
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
			}
		}
		
		
	}
	

}

