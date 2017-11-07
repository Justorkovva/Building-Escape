
#include "OpenDoor.h"
#include "Gameframework/Actor.h" 

UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	//ActorThatOpens = GetWorld() -> GetFirstPlayerController()->GetPawn(); // returns our character ONLY
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s - No pressure plate"),*GetOwner()->GetName());
	}
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > TriggerMass) //PressurePlate->IsOverlappingActor(ActorThatOpens) poprzednie sprawdzenie
	{
		OnOpen.Broadcast();
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	//GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() { //czytam masy wszystkich aktorów znajduj¹cych siê na triggerze

	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); // krzesla nie dzialaly, bo we wlasciwosciach nie dodalam im generate overlap events

	for (const auto* Actor : OverlappingActors) {

		TotalMass+=Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

	/*
	FRotator rotation = Owner->GetActorRotation();
	rotation.Yaw = OpenAngle;
	Owner->SetActorRotation(rotation);
	*/

