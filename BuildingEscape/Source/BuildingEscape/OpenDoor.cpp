// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h" 
#include "BuildingEscape.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld() -> GetFirstPlayerController()->GetPawn(); // returns our character
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	

	FRotator rotation = Owner->GetActorRotation();
	rotation.Yaw = OpenAngle;
	Owner->SetActorRotation(rotation);
}

void UOpenDoor::CloseDoor()
{
	FRotator rotation = Owner->GetActorRotation();
	rotation.Yaw = 0;
	Owner->SetActorRotation(rotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

