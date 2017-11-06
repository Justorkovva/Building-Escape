
#include "OpenDoor.h"
#include "Gameframework/Actor.h" 


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	//ActorThatOpens = GetWorld() -> GetFirstPlayerController()->GetPawn(); // returns our character ONLY
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

	if (GetTotalMassOfActorsOnPlate() > 0.f) //PressurePlate->IsOverlappingActor(ActorThatOpens) poprzednie sprawdzenie
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); // krzesla nie dzialaly, bo we wlasciwosciach nie dodalam im generate overlap events

	for (const auto* Actor : OverlappingActors) {

		TotalMass+=Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;

}
