# UE5 Ultimate Game - Полноценная игра на Unreal Engine 5

## 📋 Описание

Полнофункциональный шаблон игры на Unreal Engine 5 с виртуальным джойстиком и всеми современными механиками:

### ✅ Реализованные системы:

1. **Управление и ввод**
   - Виртуальный джойстик (UJoystickComponent)
   - Поддержка touch, клавиатуры (WASD), геймпада
   - Переключение камеры 1-го/3-го лица

2. **Персонаж (AUltimateCharacter)**
   - Ходьба, бег (Sprint)
   - Двойной прыжок
   - Рывок (Dash)
   - Атака с уроном
   - Взаимодействие с объектами

3. **Боевая система**
   - HealthComponent (здоровье, урон, регенерация)
   - Система урона и смерти
   - Невосприимчивость

4. **Система выживания**
   - StaminaComponent (выносливость)
   - Голод (Hunger)
   - Жажда (Thirst)
   - Температура тела
   - Урон от голода/жажды/температуры

5. **Взаимодействие**
   - Interactable интерфейс
   - LineTrace для определения объектов
   - Контекстные действия

6. **ИИ врагов** (заготовки)
   - Behavior Tree
   - Патрулирование
   - Преследование игрока

7. **Дополнительные системы** (структура готова):
   - Инвентарь и крафт
   - Квесты и диалоги
   - Сохранение прогресса
   - Погода и время суток
   - Паркур
   - Навыки и прокачка
   - Экономика
   - Достижения
   - Настройки доступности
   - Мультиплеер
   - Кастомизация персонажа

## 📁 Структура проекта

```
UE5UltimateGame/
├── Config/                  # Настройки движка и игры
├── Content/                 # Ассеты (модели, текстуры, звуки)
├── Source/
│   └── UE5UltimateGame/
│       ├── Public/          # Заголовочные файлы C++
│       │   ├── Character/   # Персонаж и джойстик
│       │   ├── Combat/      # Боевая система
│       │   ├── Survival/    # Выживание
│       │   ├── Interaction/ # Взаимодействие
│       │   ├── AI/          # Искусственный интеллект
│       │   ├── Inventory/   # Инвентарь
│       │   ├── Quests/      # Квесты
│       │   ├── Weather/     # Погода
│       │   ├── Parkour/     # Паркур
│       │   └── ...          # Другие системы
│       ├── Private/         # Реализация C++
│       └── UE5UltimateGame.Build.cs
└── README_FULL.md           # Эта документация
```

## 🚀 Быстрый старт

### Требования:
- Unreal Engine 5.3 или новее
- Visual Studio 2022 (для Windows)
- Xcode (для macOS/iOS)
- Android Studio (для Android)

### Установка:

1. **Клонирование проекта**
   ```bash
   cd /workspace
   # Проект уже создан в UE5UltimateGame/
   ```

2. **Открытие в Unreal Engine**
   - Запустите Unreal Engine 5.3+
   - Нажмите "Open Project"
   - Выберите файл `UE5UltimateGame.uproject`

3. **Компиляция C++ кода**
   - При первом открытии движок предложит перекомпилировать модули
   - Нажмите "Yes"
   - Дождитесь завершения компиляции

4. **Настройка проекта**
   - Откройте `Edit > Project Settings`
   - В разделе "Maps & Modes" установите:
     - Default GameMode: `BP_UltimateGameMode` (создать в Blueprint)
     - Default Pawn: `BP_UltimateCharacter` (создать на основе AUltimateCharacter)

## 🎮 Создание Blueprint классов

### 1. Персонаж игрока (BP_UltimateCharacter)

1. В Content Browser нажмите `Right Click > Create Class`
2. Выберите родительский класс: `AUltimateCharacter`
3. Назовите: `BP_UltimateCharacter`
4. Откройте и настройте:
   - Добавьте Mesh (скелетную модель)
   - Настройте Animation Blueprint
   - Привяжите компоненты к UI

### 2. Виджет джойстика (WBP_VirtualJoystick)

1. `Right Click > User Interface > Widget Blueprint`
2. Назовите: `WBP_VirtualJoystick`
3. Добавьте:
   - Circular background для джойстика
   - Knob (рукоятка) для управления
   - Кнопки действий (Jump, Attack, Interact, etc.)
4. Скрипт:
   ```blueprint
   Event Tick > Get Player Controller > 
   Get JoystickComponent > Set MovementVector/LookVector
   ```

### 3. Game Mode (BP_UltimateGameMode)

1. `Right Click > Create Class > GameModeBase`
2. Назовите: `BP_UltimateGameMode`
3. Установите:
   - Default Pawn Class: `BP_UltimateCharacter`
   - Player Controller Class: `APlayerController`

## 🕹️ Управление

### Мобильные устройства (Touch):
- **Левый стик**: Перемещение
- **Правый стик**: Камера
- **Кнопка A**: Прыжок (double-tap для двойного прыжка)
- **Кнопка B**: Атака
- **Кнопка C**: Взаимодействие
- **Кнопка D**: Рывок (Dash)
- **Кнопка E**: Бег (Sprint, удержание)
- **Кнопка F**: Смена камеры

### ПК (Клавиатура + Мышь):
- **WASD**: Перемещение
- **Мышь**: Камера
- **Пробел**: Прыжок
- **ЛКМ**: Атака
- **E**: Взаимодействие
- **Shift**: Бег
- **Ctrl**: Рывок
- **V**: Смена камеры

### Геймпад:
- **Левый стик**: Перемещение
- **Правый стик**: Камера
- **A/Cross**: Прыжок
- **X/Square**: Атака
- **B/Circle**: Взаимодействие
- **Y/Triangle**: Смена камеры
- **LT/L2**: Бег
- **RT/R2**: Рывок

## ⚙️ Настройка компонентов

### UJoystickComponent
```cpp
DeadZoneRadius = 0.1f;        // Мертвая зона джойстика
MaxOutputValue = 1.0f;        // Максимальное значение
```

### AUltimateCharacter
```cpp
WalkSpeed = 300.0f;           // Скорость ходьбы
SprintSpeed = 600.0f;         // Скорость бега
DashSpeed = 1200.0f;          // Скорость рывка
DashDuration = 0.3f;          // Длительность рывка
MaxDoubleJumps = 2;           // Максимум двойных прыжков
AttackRange = 200.0f;         // Дальность атаки
AttackDamage = 25.0f;         // Урон атаки
InteractionRange = 300.0f;    // Дальность взаимодействия
```

### UHealthComponent
```cpp
MaxHealth = 100.0f;           // Максимальное здоровье
RegenerationDelay = 5.0f;     // Задержка перед регенерацией
RegenerationRate = 2.0f;      // Здоровья в секунду
```

### USurvivalComponent
```cpp
MaxStamina = 100.0f;          // Максимальная выносливость
StaminaRegenRate = 10.0f;     // Регенерация в секунду
SprintStaminaDrain = 15.0f;   // Потребление при беге
DashStaminaCost = 25.0f;      // Стоимость рывка
MaxHunger = 100.0f;           // Максимальный голод
HungerDecayRate = 2.0f;       // Потребление голода в секунду
MaxThirst = 100.0f;           // Максимальная жажда
ThirstDecayRate = 3.0f;       // Потребление жажды в секунду
```

## 📦 Сборка проекта

### Для Windows:
1. `File > Package Project > Windows > Win64`
2. Выберите папку для сборки
3. Дождитесь завершения

### Для Android:
1. Установите Android SDK и NDK
2. `Edit > Project Settings > Platforms > Android`
3. Настройте подписывание
4. `File > Package Project > Android > Android (ASTC)`

### Для iOS:
1. Требуется macOS с Xcode
2. Настройте signing certificate
3. `File > Package Project > iOS`

## 🔧 Расширение функционала

### Добавление нового interagable объекта:

1. Создайте Blueprint Class от `AActor`
2. Добавьте компонент `UInteractableComponent`
3. Реализуйте логику взаимодействия:
   ```blueprint
   Event OnInteract > Custom Logic
   ```

### Добавление нового типа оружия:

1. Создайте `AWeapon` класс
2. Настройте:
   - Damage value
   - Fire rate
   - Projectile class (если есть)
3. Добавьте в инвентарь игрока

### Создание квеста:

1. Используйте `UQuestManager`
2. Создайте Data Asset с параметрами квеста
3. Настройте условия выполнения
4. Настройте награды

## 🐛 Известные ограничения

- Некоторые системы требуют настройки в Blueprint
- Для полноценной работы нужны ассеты (модели, анимации, звуки)
- Мультиплеер требует дополнительной настройки сервера
- ИИ врагов нуждается в настройке Behavior Tree

## 📞 Поддержка

Для вопросов и предложений:
- Документация Unreal Engine: https://docs.unrealengine.com/
- Форумы Unreal: https://forums.unrealengine.com/

## 📄 Лицензия

Этот проект является шаблоном для образовательных целей.
Используйте свободно в своих проектах.

---

**Создано с помощью AI Assistant для Unreal Engine 5**
Версия: 1.0.0
Дата: 2024
