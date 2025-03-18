// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGun.h"

#include "Camera/CameraComponent.h"

// Sets default values
APortalGun::APortalGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AActor* APortalGun::ShootPortal(AActor *Portal, UCameraComponent *Camera) const
{
	const FVector StartLocation = Camera->GetComponentLocation();
	const FVector EndLocation = StartLocation + Camera->GetForwardVector() * 100000.0f;
	const UWorld* World = GetWorld();
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Portal);
	
	if (World == nullptr || Portal == nullptr)
	{
		return nullptr;
	}
	GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(1.0f), QueryParams);
	
	if (!HitResult.bBlockingHit)
	{
		return nullptr;
	}
	auto NewRotation = HitResult.ImpactNormal.Rotation();
	auto NewLocation = HitResult.ImpactPoint + HitResult.ImpactNormal * 2.0f;

	if (HitResult.ImpactNormal.Z >= 0.99f)
	{
		NewRotation += FRotator(0.0f, Camera->GetComponentRotation().Yaw, 180);
	}

	
	Portal->SetActorLocation(NewLocation);
	Portal->SetActorRotation(NewRotation.Quaternion());
	return HitResult.GetActor();
}



void APortalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

