// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraDirector.generated.h"

UCLASS()
class PANICATTHETEASHOP_API ACameraDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere) //UPROPERTY macro makes variables visible to UE. vals set will not reset in future sess.
	AActor* CameraOne; // EditAnywhere lets us set cameraone and cameratwo in UE

	UPROPERTY(EditAnywhere)
	AActor* CameraTwo;

	float TimeToNextCameraChange;
};
