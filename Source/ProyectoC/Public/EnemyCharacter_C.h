// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "EnumEnemyState.h"
#include "EnemyCharacter_C.generated.h"

UCLASS()
class PROYECTOC_API AEnemyCharacter_C : public ACharacter
{
	GENERATED_BODY()

	/*Montaje de reaccion a los ataques*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ReactionMontageAnimation;

	/*Sonido de golpe*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* HurtSoundCue;

	/* Colisiones de las manos*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HandCollisionRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HandCollisionLeft;

public:
	// Sets default values for this character's properties
	AEnemyCharacter_C();

	/*Salud del enemigo*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		int Health;

	/*Enum de estado del enemigo*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EEnemyState> MyState;

	/*Inicio de collision*/
	UFUNCTION()
		void OnHurtOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*Funcion para disminuir la vida*/
	UFUNCTION()
		void DecreaseHealth(int Damage);

	/*Funcion cuando el ataque empieza*/
	UFUNCTION()
		void StartAttack();

	/*Funcion cuando el ataque termina*/
	UFUNCTION()
		void EndAttack();

	/*Funcion para iniciar la animacion de ataque*/
	UFUNCTION()
	void StartAnimAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Componenete de audio donde se guarda el sonido de herido*/
	UAudioComponent* HurtAudioComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
