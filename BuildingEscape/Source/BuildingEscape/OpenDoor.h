#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "BuildingEscape.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent); //makro do eventow, nasza nazwa w srodku dowolna

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOpenDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	AActor* Owner=nullptr;
	float TriggerMass;

public:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate=nullptr; // dodac w unrealu jaki trigger bedzie do tego przypisany, bo wyskoczy straszny blad i  w ogole

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable) //zeby je dodac w unrealu potrzebujemy skompilowac kod, a potem w components (po wejsciu w BP) klikn¹æ na klasê w tym przypadku open door. dopiero wtedy jesteœmy w stanie dodac te eventy
		FDoorEvent OnClose;

	float GetTotalMassOfActorsOnPlate();
};
