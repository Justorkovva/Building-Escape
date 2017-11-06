
#include "Grabber.h"
#include "Gameframework/Actor.h" 

#define OUT

UGrabber::UGrabber()
{
	//bWantsBeginPlay = true; // w nowszych wersjach unreala nie trzeba tej linijki pisac
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent(); //bez tego crashuje, wyszukuje potrzebny komponent
	SetupInputComponent(); //co sie dzieje jak nacisniemy klawisz
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) //jesli cos zlapalismy to to przenosimy
			PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Player pos : %s,  player rotation:  %s "), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(), GetLineTraceStart(), GetLineTraceEnd(),
		FColor(0, 250, 50), false, 0.f, 0.f, 3.f
	);


	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit : %s "), *(ActorHit->GetName()));
	}
	return Hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	auto HitResult=GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab, //co lapie
			NAME_None, //nie mamy szkieletu
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent() {

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Yey, physics handle component works"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found"));
	}
}

//Zanim tu to napiszemy, to w Project Settings -> Engine - > Input -> Action Mappings piszemy dokladnie taka sama nazwe funkcji i przyciski ktore cos beda robic 
// w tym przypadku funkcja to Grab
void UGrabber::SetupInputComponent() //ustawiamy co sie ma dziac jak nacisniemy/puscimy przycisk
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Yey, input component works"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No input component found"));

	}
}

FVector UGrabber::GetLineTraceEnd() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Player pos : %s,  player rotation:  %s "), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}

FVector UGrabber::GetLineTraceStart() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Player pos : %s,  player rotation:  %s "), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation;
	return LineTraceEnd;
}