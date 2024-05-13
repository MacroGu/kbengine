// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "KBECommon.h"
#include "KBEvent.h"
#include "Tickable.h"
#include "KBETicker.generated.h"

#define InitLocationTimeInterval 10

/**
 * 
 */
UCLASS()
class KBENGINEPLUGINS_API UKBETicker : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UKBETicker();
	~UKBETicker();

	UWorld* GetWorld() const override;

	virtual bool IsTickableWhenPaused() const override;	
	virtual bool IsTickableInEditor() const override;	
	virtual UWorld* GetTickableGameObjectWorld() const override;	

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	virtual void UpdateUEPawnLocationToKBEEntity();

	void OnEndPIE(const bool);

	float InitLocationTime = 0.f;

	// VR 游戏， 以 camera 位置 为准， 普通游戏则是 pawn 的位置
	virtual bool UpdatePawnAndCameraComp();

private:

	TWeakObjectPtr<class APawn> SelfPawn = nullptr;
	TWeakObjectPtr<class UCameraComponent> PawnCameraComp = nullptr;

};
