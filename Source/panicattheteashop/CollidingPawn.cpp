// Fill out your copyright notice in the Description page of Project Settings.
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CollidingPawn.h"
#include "PlanetOne.h"
#include "Math/Vector.h"
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))
#include "UObject/ObjectMacros.h"

#include "Animation/AnimInstanceProxy.h"

// APlanetOne* OurFirstPlanet;
// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//root component will be a sphere that reacts to physics which is cool
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	// // SphereComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	// SphereComp->SetSphereRadius(200);
	
	/* create and attach visible sphere from static mesh asset with radius of 50.
	 * This does not line up with 40 radius sphere component we created, so we
	 * scale the asset down to 80%. Also move down by 40 units to have center line up w center of sphere comp.
	 */
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f)); //scale down
	}

	// attach inactive particle system component to hierarchy. Can manipulate component in code, later setting up
	// an input to turn it on or off.
	// particle system component is attached directly to static mesh component, NOT to root.
	//slightly offset from bottom-center of mesh so more visible during play.
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	bIsPawnOnPlanet = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if(ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	/* 7. Spring Arm Component. smoother attachment point for camera by allowing camera to accelerate and decelerate
	 * more slowly than pawn it is following. Also has built-in feature that prevents camera from going thru
	 * solid objects, for situations such as when player backs into corner in 3rd person game. While not necessary,
	 * the spring arm component can be a very fast and easy way to get smoother camera work into game.
	 */
	// Use spring arm to give camera smooth, natural-feeling motion.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.5f, 0.0f, 0.f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f; // speed of camera lag

	/* Create actual camera component. spring arm component has special built-in socket we can attach to,
	 * instead of attaching directly to component's base
	 */
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // ATTACH

	/* SET PAWN TO BE CONTROLLED BY DEFAULT PLAYER.*/
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// create instance of movement component, tell it to update root
	OurMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// if(!bIsPawnOnPlanet)
	// {
		// if(PlanetLocation != NULL)
		// {
			// FVector NewVec = PlanetLocation - GetActorLocation();
			// OurMovementComponent->AddInputVector(NewVec);
			// UE_LOG(LogTemp, Warning, TEXT("My Variable Vector is: %s"), *NewVec.ToString());
		
		// }
	// }
	// OurMovementComponent->AddInputVector(GetGravityDirection());
	// OurMovementComponent->AddInputVector(GetActorLocation()); // test???
	// UE_LOG(LogTemp, Warning, TEXT("Your message, %f"), GetGravityDirection());
}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward); // bind functions to input events
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);
	PlayerInputComponent->BindAxis("Tilt", this, &ACollidingPawn::Tilt);
	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ParticleToggle);
	

}

void ACollidingPawn::MoveForward(float AxisValue)
{
	if(OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() *AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	if(OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() *AxisValue);
	}
}

void ACollidingPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::Tilt(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::ParticleToggle()
{
	if(OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}

void ACollidingPawn::SetbIsOnPlanet(bool value)
{
	bIsPawnOnPlanet = value;
}

void ACollidingPawn::ReceivePlanetLocation(FVector loc)
{
	PlanetLocation = loc;
}