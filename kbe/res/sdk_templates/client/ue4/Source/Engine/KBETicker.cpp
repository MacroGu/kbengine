// Fill out your copyright notice in the Description page of Project Settings.

#include "KBETicker.h"
#include "Engine.h"
#include "KBDebug.h"
#include "Entity.h"
#include "KBEngine.h"
#include "KBECommon.h"

#include "Camera/CameraComponent.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UKBETicker::UKBETicker()
{
#if WITH_EDITOR
	FEditorDelegates::EndPIE.AddUObject(this, &UKBETicker::OnEndPIE);
#endif
}

UKBETicker::~UKBETicker()
{
}

void UKBETicker::Tick(float DeltaTime)
{
	KBEvent::processOutEvents();

	UpdateUEPawnLocationToKBEEntity();

	if (InitLocationTime < InitLocationTimeInterval)
	{
		InitLocationTime = InitLocationTime + DeltaTime;
	}

	KBEngine::KBEngineApp::getSingleton().process();
}

bool UKBETicker::IsTickable() const
{
	return true;
}

TStatId UKBETicker::GetStatId() const
{
	return TStatId();
}

void UKBETicker::UpdateUEPawnLocationToKBEEntity()
{
	if (!SelfPawn.IsValid() || !PawnCameraComp.IsValid())
	{
		if (!UpdatePawnAndCameraComp())
		{
			return;
		}
	}

	KBEngine::Entity* kbe_player = KBEngine::KBEngineApp::getSingleton().player();
	if (!kbe_player)
	{
		return;
	}

	FVector NewLocation = PawnCameraComp->GetComponentLocation();
	NewLocation.Z = 0;

	if (InitLocationTime < InitLocationTimeInterval)
	{
		FString ClientID;
		if (!FParse::Value(FCommandLine::Get(), TEXT("ClientID="), ClientID))
		{
			return;
		}

		if (ClientID.Equals("1"))
		{
			NewLocation.X = 6.423156f;
			NewLocation.Y = 6.259491;
		}
		else if (ClientID.Equals("2"))
		{
			NewLocation.X = -168.955229f;
			NewLocation.Y = -101.064663f;
		}

	}

	UE_LOG(KBEngine::LogKBEngine, Error, TEXT("%s(): %d  InitLocationTime: %f, x: %f, y: %f "), *FString(__FUNCTION__), __LINE__, InitLocationTime,
		NewLocation.X, NewLocation.Y);

	PawnCameraComp->SetWorldLocation(NewLocation);
	// 每个tick将UE4的玩家坐标写入到KBE插件中的玩家实体坐标，插件会定期同步给服务器
	UE4Pos2KBPos(kbe_player->position, NewLocation);
	// UE4Pos2KBPos(kbe_player->position, ue4_player->GetActorLocation());
	UE4Dir2KBDir(kbe_player->direction, PawnCameraComp->GetComponentRotation());

	// kbe_player->isOnGround(SelfPawn->GetMovementComponent() && SelfPawn->GetMovementComponent()->IsMovingOnGround());
}

UWorld* UKBETicker::GetWorld() const
{ 
	UWorld* World = (GetOuter() != nullptr) ? GetOuter()->GetWorld() : GWorld;	
	if (World == nullptr)
	{
		World = GWorld;
	}
	return World; 
}

bool UKBETicker::IsTickableWhenPaused() const
{
	return false;
}

bool UKBETicker::IsTickableInEditor() const
{
	return false;
}

UWorld* UKBETicker::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

void UKBETicker::OnEndPIE(const bool data)
{
#if WITH_EDITOR
	KBEngine::KBEngineApp::destroyKBEngineApp();
#endif
}

bool UKBETicker::UpdatePawnAndCameraComp()
{
	if (SelfPawn.IsValid() && PawnCameraComp.IsValid())
	{
		return true;
	}

	if (!SelfPawn.IsValid())
	{
		SelfPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!SelfPawn.IsValid())
		{
			return false;
		}

		PawnCameraComp = SelfPawn->FindComponentByClass<UCameraComponent>();
		if (!PawnCameraComp.IsValid())
		{
			return false;
		}
	}
	else
	{
		PawnCameraComp = SelfPawn->FindComponentByClass<UCameraComponent>();
		if (!PawnCameraComp.IsValid())
		{
			return false;
		}
	}

	return true;
}
