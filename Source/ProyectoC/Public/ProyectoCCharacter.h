// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumPlayerState.h"
#include "ProyectoCCharacter.generated.h"

UCLASS(config=Game)
class AProyectoCCharacter : public ACharacter
{
	
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/* Colisiones de las manos*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HandCollisionRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HandCollisionLeft;

	/*String que almacena el nombre de la seccion del combo*/
	UPROPERTY(VisibleAnywhere, Category = Generic)
		FString ActualCombo;

	/*Index para determinar la posicion del combo en la que nos encontramos*/
	UPROPERTY(VisibleAnywhere, Category = Generic)
		int ActualIndexCombo;

	/*Timer para determinar cuando el combo se rompe y empezar de nuevo*/
	UPROPERTY(VisibleAnywhere, Category = Generic)
		float TimerResetCombo;

	/*Variabl de daño del personaje*/
	UPROPERTY(VisibleAnywhere, Category = Generic)
		int Damage;

	/*Montaje de animacion de atacar con puños*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* PunchsAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* WooshSoundCue;


public:
	AProyectoCCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/*Componente donde se almacenara el audio de los ataques*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		UAudioComponent* WooshAudioComponent;

	/*Estado de Enum para el estado del jugador*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EPlayerState> MyState;

	//Funcion que desencadena el ataque cuando se presiona el input del jugador
	void PunchInput();

	//Cambia las colisiones de las manos al tipo Punch
	void PunchStart();

	//Cambia las colisiones de las manos a NoCollision 
	void PunchEnd();

	/*Funcion para cuando inicia la colision con algun actor*/
	UFUNCTION()
	void OnPunchOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*Funcion para cuando termina la colision con algun actor*/
	UFUNCTION()
	void OnPunchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void BeginPlay();

	/*Cambia el ataque del combo*/
	void NextCombo(UAnimMontage* Montage, FString str);

	/*Reinicia el combo*/
	void TimerComboReset(float DeltaTime);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/*Obtener el daño*/
	FORCEINLINE int GetDamage() const { return Damage; }

	/*Setear el estado del jugador*/
	FORCEINLINE void SetPlayerState(TEnumAsByte<EPlayerState> state) { MyState = state;}
};

