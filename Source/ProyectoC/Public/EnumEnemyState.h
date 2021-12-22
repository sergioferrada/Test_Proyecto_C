// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEnemyState: uint8 {

	 IDLE,
	 HURT,
	 ATTACKING,
	 DEATH,
 };

