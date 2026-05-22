// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	// Health Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MinHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bCanRegenerate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float RegenerationDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float RegenerationRate = 2.0f; // Health per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bIsInvulnerable = false;

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeComponent();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageAmount, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Kill();

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAtMaxHealth() const;

private:
	float LastDamageTime;
	bool bIsRegenerating;
	float RegenerationTimer;

	void StartRegeneration();
	void HandleRegeneration(float DeltaTime);
};
