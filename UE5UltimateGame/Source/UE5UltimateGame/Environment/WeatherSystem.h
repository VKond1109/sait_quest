#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UWeatherSystem.generated.h"

// Типы погоды
UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Clear,
    Cloudy,
    Rain,
    Storm,
    Snow,
    Fog,
    Sandstorm
};

// Время суток
UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
    Dawn,
    Morning,
    Noon,
    Afternoon,
    Sunset,
    Evening,
    Night,
    Midnight
};

UCLASS()
class UE5ULTIMATEGAME_API AWeatherSystem : public AActor
{
    GENERATED_BODY()

public:
    AWeatherSystem();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // === ПОГОДА ===
    void UpdateWeather(float DeltaTime);
    void ChangeWeather(EWeatherType NewWeather);
    void TransitionWeather(EWeatherType From, EWeatherType To, float Duration);
    
    // === ВРЕМЯ СУТОК ===
    void UpdateTimeOfDay(float DeltaTime);
    void SetTimeOfDay(float Hours);
    ETimeOfDay CalculateTimeOfDay(float Hours) const;
    
    // === ЭФФЕКТЫ ===
    void UpdateSkySphere();
    void UpdateExposure();
    void SpawnWeatherVFX();
    void UpdateWind();

    // === РАЗРУШАЕМОСТЬ ===
    void CheckDestructibleObjects();
    void ApplyWindForce();

public:
    // Настройки времени
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float TimeScale; // Скорость течения времени (1.0 = реальное время)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float CurrentHour; // 0-24

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    bool bAutoAdvanceTime;

    // Настройки погоды
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    EWeatherType CurrentWeather;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    EWeatherType TargetWeather;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    float WeatherTransitionDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    float WeatherChangeInterval; // Как часто менять погоду автоматически

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    bool bAutoChangeWeather;

    // Параметры ветра
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
    FVector WindDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
    float WindSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
    float WindGustStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
    float WindGustInterval;

    // Ссылки на компоненты окружения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    class ASkySphere* SkySphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    class AExponentialHeightFog* ExponentialFog;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    class ADirectionalLight* SunLight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    class UNiagaraComponent* WeatherVFXComponent;

    // Текущие параметры
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float CurrentTemperature;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float CurrentHumidity;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float VisibilityDistance;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float RainIntensity;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float ThunderChance;

    // События
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnWeatherChanged(EWeatherType OldWeather, EWeatherType NewWeather);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnTimeOfDayChanged(ETimeOfDay OldTime, ETimeOfDay NewTime);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnThunderStrike(FVector Location);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weather")
    ETimeOfDay GetCurrentTimeOfDay() const { return CalculateTimeOfDay(CurrentHour); }

    UFUNCTION(BlueprintCallable, Category = "Weather")
    void ForceWeatherChange(EWeatherType NewWeather);

    UFUNCTION(BlueprintCallable, Category = "Time")
    void SetTimeScale(float NewScale);
};
