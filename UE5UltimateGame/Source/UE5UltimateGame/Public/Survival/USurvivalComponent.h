// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USurvivalComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSurvivalStatChanged, float, CurrentValue, float, MaxValue, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API USurvivalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USurvivalComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	// Stamina Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate = 10.0f; // Per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float SprintStaminaDrain = 15.0f; // Per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float DashStaminaCost = 25.0f;

	// Hunger Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
	float MaxHunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
	float CurrentHunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
	float HungerDecayRate = 2.0f; // Per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
	float StarvationDamage = 5.0f; // Damage per second when starving

	// Thirst Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirst")
	float MaxThirst = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirst")
	float CurrentThirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirst")
	float ThirstDecayRate = 3.0f; // Per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirst")
	float DehydrationDamage = 5.0f; // Damage per second when dehydrated

	// Temperature Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float NormalBodyTemp = 36.6f; // Celsius

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float CurrentBodyTemp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float MinSafeTemp = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float MaxSafeTemp = 38.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float TempAdjustmentRate = 0.5f; // Per second

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSurvivalStatChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSurvivalStatChanged OnHungerChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSurvivalStatChanged OnThirstChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSurvivalStatChanged OnTemperatureChanged;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Survival")
	void InitializeComponent();

	UFUNCTION(BlueprintCallable, Category = "Survival")
	bool HasEnoughStaminaForSprint() const;

	UFUNCTION(BlueprintCallable, Category = "Survival")
	bool HasEnoughStaminaForDash() const;

	UFUNCTION(BlueprintCallable, Category = "Survival")
	bool ConsumeStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Survival")
	void AddStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Survival")
	void EatFood(float FoodValue);

	UFUNCTION(BlueprintCallable, Category = "Survival")
	void DrinkWater(float WaterValue);

	UFUNCTION(BlueprintCallable, Category = "Survival")
	void AdjustTemperature(float ExternalTemp, float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Survival")
	bool IsStarving() const;

	UFUNCTION(BlueprintPure, Category = "Survival")
	bool IsDehydrated() const;

	UFUNCTION(BlueprintPure, Category = "Survival")
	bool IsOverheating() const;

	UFUNCTION(BlueprintPure, Category = "Survival")
	bool IsFreezing() const;

private:
	void HandleStaminaRegen(float DeltaTime);
	void HandleHungerDecay(float DeltaTime);
	void HandleThirstDecay(float DeltaTime);
	void HandleTemperatureEffects(float DeltaTime);
};
