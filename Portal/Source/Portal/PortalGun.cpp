#include "PortalGun.h"
#include "Camera/CameraComponent.h"

AActor* APortalGun::ShootPortal(AActor *Portal, UCameraComponent *Camera) const
{
	const FVector  StartLocation = Camera->GetComponentLocation();
	const FVector  InitLocation = Portal->GetActorLocation();
	const FRotator InitRotation = Portal->GetActorRotation();
	const FVector  EndLocation = StartLocation + Camera->GetForwardVector() * 100000.0f;
	const UWorld*  World = GetWorld();
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Portal);
	
	if (World == nullptr || Portal == nullptr)
	{
		return nullptr;
	}
	World->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(1.0f), QueryParams);
	AActor* Object = HitResult.GetActor();
	
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
	
	TArray<FVector> PointsCenter = GetPointCenter(Portal, HitResult.ImpactPoint);

	if (AllBottomIsOnWall(Portal, World))
	{
		return Object;
	}
	for(int i = 0; i < PointsCenter.Num(); i++)
	{
		FVector StartPoint = PointsCenter[i] - HitResult.ImpactNormal;

		if (World->LineTraceSingleByChannel(HitResult, StartPoint, HitResult.ImpactPoint, ECC_Visibility, QueryParams))
		{
			if (HitResult.GetActor() != Object)
			{
				QueryParams.AddIgnoredActor(HitResult.GetActor());
				i--;
				continue;
			}
			Portal->SetActorLocation(Portal->GetActorLocation() - HitResult.Normal * HitResult.Distance);
		}
	}

	if (!AllBottomIsOnWall(Portal, World))
	{
		Portal->SetActorLocation(InitLocation);
		Portal->SetActorRotation(InitRotation.Quaternion());
		return nullptr;
	}
	
	return Object;
}

TArray<FVector> APortalGun::GetBottomPortal(const AActor* Portal)
{
	const FVector SizePortal = Portal->GetActorScale3D(); 
	
	TArray<FVector> BottomPortal;
	
	BottomPortal.Add(Portal->GetActorLocation() + Portal->GetActorUpVector() * (SizePortal.X * 100) - Portal->GetActorRightVector() * (SizePortal.Y * 100 / 2));
	BottomPortal.Add(Portal->GetActorLocation() + Portal->GetActorUpVector() * (SizePortal.X * 100) + Portal->GetActorRightVector() * (SizePortal.Y * 100 / 2));
	BottomPortal.Add(Portal->GetActorLocation() - Portal->GetActorUpVector() * (SizePortal.X * 100) - Portal->GetActorRightVector() * (SizePortal.Y * 100 / 2));
	BottomPortal.Add(Portal->GetActorLocation() - Portal->GetActorUpVector() * (SizePortal.X * 100) + Portal->GetActorRightVector() * (SizePortal.Y * 100 / 2));
	
	return BottomPortal;
}

TArray<FVector> APortalGun::GetPointCenter(const AActor* Portal, const FVector& HitPosition)
{
	TArray<FVector> PointsCenter;
	const FVector SizePortal = Portal->GetActorScale3D(); 

	PointsCenter.Add(HitPosition + Portal->GetActorUpVector() * (SizePortal.X * 100));
	PointsCenter.Add(HitPosition - Portal->GetActorUpVector() * (SizePortal.X * 100));
	PointsCenter.Add(HitPosition + Portal->GetActorRightVector() * (SizePortal.Y * 100 / 2));
	PointsCenter.Add(HitPosition - Portal->GetActorRightVector() * (SizePortal.Y * 100 / 2));
	
	return PointsCenter;
}

bool APortalGun::AllBottomIsOnWall(const AActor* Portal, const UWorld*  World)
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Portal);
	TArray<FVector> BottomPortal = GetBottomPortal(Portal);
	
	for(int i = 0; i < BottomPortal.Num(); i++)
	{
		FVector StartPoint = BottomPortal[i];
		FVector EndPoint = StartPoint - Portal->GetActorForwardVector() * 5;

		if (!World->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_Visibility, Params))
		{
			return false;
		}
	}
	return true;
}

