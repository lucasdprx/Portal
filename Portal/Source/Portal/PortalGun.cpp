// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGun.h"

#include "Camera/CameraComponent.h"

// Sets default values
APortalGun::APortalGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector APortalGun::ShootPortal(AActor *Portal, UCameraComponent *Camera, AActor *Player)
{
	FVector StartLocation = Camera->GetComponentTransform().GetLocation();
	FVector rotation = Player->GetActorRotation().Vector() + Camera->GetComponentTransform().GetRotation().Rotator().Vector();
	FVector EndLocation = StartLocation + Camera->GetComponentTransform().GetLocation().ForwardVector * 100000.0f;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	UWorld*World = GetWorld();
	if (World == nullptr)
	{
		return FVector::ZeroVector;
	}
	GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(1.0f), QueryParams);
	
	if (HitResult.bBlockingHit && Portal != nullptr)
	{
		Portal->SetActorLocation(HitResult.ImpactPoint);
	}
	return StartLocation;
}



void APortalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

