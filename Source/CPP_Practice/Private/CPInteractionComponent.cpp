// Fill out your copyright notice in the Description page of Project Settings.


#include "CPInteractionComponent.h"

#include "CPGameplayInterface.h"
#include "Blueprint/UserWidget.h"

UCPInteractionComponent::UCPInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UCPInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Owner = GetOwner();
    APawn* MyPawn = Cast<APawn>(Owner);
    if (!MyPawn) return;

    APlayerController* PC = Cast<APlayerController>(MyPawn->GetController());
    if (!PC) return;

    // 屏幕中心点
    int32 ViewportSizeX, ViewportSizeY;
    PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
    float ScreenX = ViewportSizeX / 2.f;
    float ScreenY = ViewportSizeY / 2.f;

    FVector WorldLocation;
    FVector WorldDirection;

    // 从屏幕中心点反投影到世界空间射线
    if (!PC->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection))
    {
        return;
    }

    FVector TraceStart = WorldLocation;
    float TraceDistance = 300.f;
    FVector TraceEnd = TraceStart + WorldDirection * TraceDistance;

    float SweepRadius = 60.f;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(SweepRadius);

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MyPawn);

    TArray<FHitResult> Hits;
    bool bHit = GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Sphere, QueryParams);

    AActor* BestHitActor = nullptr;

    for (const FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->Implements<UCPGameplayInterface>())
        {
            if (ICPGameplayInterface::Execute_CanInteract(HitActor, MyPawn))
            {
                BestHitActor = HitActor;
                break;
            }
        }
    }

    if (BestHitActor != FocusedActor)
    {
        if (InteractionWidgetInstance)
        {
            InteractionWidgetInstance->RemoveFromParent();
            InteractionWidgetInstance = nullptr;
        }

        FocusedActor = BestHitActor;

        if (FocusedActor && DefaultInteractionWidgetClass)
        {
            InteractionWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), DefaultInteractionWidgetClass);
            if (InteractionWidgetInstance)
            {
                InteractionWidgetInstance->AddToViewport();
            }
        }
    }
    
}
void UCPInteractionComponent::PrimaryInteract()
{
	if (!FocusedActor) return;

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (!MyPawn) return;

	if (FocusedActor->Implements<UCPGameplayInterface>())
	{
		ICPGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
	}
}