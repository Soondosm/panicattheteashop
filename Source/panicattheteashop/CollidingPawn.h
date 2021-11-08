// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class PANICATTHETEASHOP_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PullPawn(); // pull into planet
	bool bIsPawnOnPlanet; // initializing to false

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override; //overriding getmovementcomponent function to return custom pawn movement component

	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem; // keeps track of a particle system component
	// that we create in .cpp

	UPROPERTY()
	class UCollidingPawnMovementComponent* OurMovementComponent;


	// FUNCTIONS
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void Tilt(float AxisValue);
	void ParticleToggle();

};
