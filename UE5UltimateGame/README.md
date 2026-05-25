# UE5 Ultimate Game - Полноценная игра на Unreal Engine 5

## 🎮 Описание

Полноценный проект игры на Unreal Engine 5 с полным набором современных игровых механик:

### ✨ Основные возможности

- **🕹️ Виртуальный джойстик** - Поддержка touch, клавиатуры и геймпада
- **⚔️ Боевая система** - Атаки, умения, прицеливание
- **❤️ Система здоровья** - Характеристики, урон, смерть
- **🤖 ИИ врагов** - Патрулирование, преследование, атака, разные типы поведения
- **🌐 Онлайн мультиплеер** - Создание/поиск сессий через Steam
- **🎒 Инвентарь** - Предметы, экипировка, крафт, система веса
- **🌳 Дерево навыков** - Прокачка, пассивные и активные навыки
- **📜 Квестовая система** - Сюжетные, побочные, ежедневные квесты
- **🌤️ Динамическая погода** - Дождь, снег, шторм, цикл дня/ночи
- **💥 Разрушаемое окружение** - Физика Chaos, осколки, эффекты
- **🎨 Кастомизация персонажа** - Внешность, цвета, пресеты
- **🏗️ Строительство** - Система размещения объектов

## 📁 Структура проекта

```
UE5UltimateGame/
├── Config/
│   ├── DefaultGame.ini      # Настройки проекта
│   └── DefaultEngine.ini    # Настройки движка и сети
├── Content/                  # Ассеты (карты, меши, текстуры)
└── Source/
    └── UE5UltimateGame/
        ├── Core/
        │   ├── JoystickInputComponent.h/cpp  # Виртуальный джойстик
        ├── Gameplay/
        │   ├── UltimateCharacter.h/cpp       # Персонаж игрока
        │   ├── InventorySystem.h             # Инвентарь
        │   ├── SkillTreeSystem.h             # Дерево навыков
        │   ├── QuestSystem.h                 # Квесты
        │   └── CustomizationSystem.h         # Кастомизация
        ├── Interaction/
        │   ├── InteractableInterface.h       # Интерфейс взаимодействия
        │   └── InteractionComponent.h        # Компонент взаимодействия
        ├── AI/
        │   └── EnemyCharacter.h              # ИИ врагов
        ├── Network/
        │   └── NetworkSessionManager.h       # Мультиплеер (Steam)
        ├── Environment/
        │   ├── WeatherSystem.h               # Погода и время суток
        │   └── DestructibleObject.h          # Разрушаемые объекты
        └── Movement/                         # Продвинутая система движения
```

## 🚀 Установка

### Требования
- Unreal Engine 5.3 или выше
- Visual Studio 2022 (для Windows)
- Xcode (для macOS/iOS)
- Android Studio (для Android)

### Шаги установки

1. **Клонирование проекта**
   ```bash
   cd /workspace
   # Проект уже создан в папке UE5UltimateGame
   ```

2. **Открытие в Unreal Engine**
   - Запустите Unreal Engine 5.3+
   - Нажмите "Open Project"
   - Выберите файл `UE5UltimateGame.uproject`

3. **Компиляция кода**
   - При первом открытии UE предложит скомпилировать модули
   - Нажмите "Yes" для компиляции
   - Или откройте `.sln` файл в Visual Studio и соберите решение

4. **Настройка Steam** (для мультиплеера)
   - Зарегистрируйтесь на [Steamworks](https://partner.steamgames.com/)
   - Получите App ID
   - Замените `SteamDevAppId=480` в `DefaultEngine.ini` на ваш App ID

## 🎯 Управление

### Мобильные устройства
- **Левый стик** - Перемещение
- **Правый стик** - Камера/прицеливание
- **Кнопки действий** - Атака, прыжок, взаимодействие

### ПК
- **WASD** - Перемещение
- **Мышь** - Камера/прицеливание
- **ЛКМ** - Атака
- **ПКМ** - Прицеливание
- **Пробел** - Прыжок
- **Shift** - Бег
- **Ctrl** - Приседание
- **E** - Взаимодействие
- **I** - Инвентарь
- **K** - Дерево навыков
- **Q** - Квесты

### Геймпад
- **Левый стик** - Перемещение
- **Правый стик** - Камера
- **A/Cross** - Прыжок
- **B/Circle** - Отмена/приседание
- **X/Square** - Взаимодействие
- **Y/Triangle** - Атака
- **RB/R1** - Прицеливание
- **LB/L1** - Блок/умение

## 🔧 Настройка

### Добавление контента

1. **Создание карты**
   - File → New Level → выберите шаблон
   - Добавьте освещение (Directional Light, Sky Light, Exponential Height Fog)
   - Добавьте NavMeshBoundsVolume для ИИ

2. **Настройка персонажа**
   - Создайте Blueprint класса `AUltimateCharacter`
   - Настройте характеристики в деталях
   - Добавьте меши и компоненты

3. **Создание врагов**
   - Создайте Blueprint класса `AEnemyCharacter`
   - Настройте точки патрулирования
   - Настройте поведение в Blackboard

### Настройка мультиплеера

1. **Steam конфигурация**
   ```ini
   [OnlineSubsystemSteam]
   bEnabled=true
   SteamDevAppId=480  ; Замените на ваш App ID
   ```

2. **Создание сессии** (Blueprint)
   - Get Game Instance → Cast to YourGameInstance
   - Call `CreateSession` с параметрами
   - Обработайте событие `OnSessionCreated`

3. **Поиск сессий**
   - Call `FindSessions`
   - Обработайте `OnSessionFound`
   - Call `JoinSessionByIndex`

## 📚 API Reference

### JoystickInputComponent
```cpp
// Получить ввод левого джойстика
FJoystickData LeftJoystick;
float Magnitude = LeftJoystick.Magnitude; // 0-1
FVector2D Direction = LeftJoystick.InputVector; // -1 до 1

// Получить ввод правого джойстика
FJoystickData RightJoystick;
```

### UltimateCharacter
```cpp
// Характеристики
FCharacterStats Stats;
Stats.Health = 100.f;
Stats.Damage = 25.f;

// События
OnHealthChanged(OldHealth, NewHealth);
OnLevelUp(NewLevel);
OnDeath();
```

### InventorySystem
```cpp
// Добавить предмет
FItemData Item;
Item.ItemID = "Sword_001";
Item.ItemName = "Iron Sword";
Inventory->AddItem(Item, Quantity);

// Экипировать
Inventory->EquipItem(SlotIndex);
```

### NetworkSessionManager
```cpp
// Создать сессию
SessionManager->CreateSession(MaxPlayers, bIsPrivate, ServerName);

// Найти сессии
SessionManager->FindSessions(MaxResults, bUseLAN);

// Присоединиться
SessionManager->JoinSessionByIndex(SessionIndex);
```

## 🛠️ Расширение

### Добавление новых навыков
1. Откройте `SkillTreeSystem.h`
2. Добавьте новый `FSkillData` в массив навыков
3. Настройте эффекты в Blueprint

### Добавление новых типов врагов
1. Создайте класс наследник `AEnemyCharacter`
2. Переопределите `BehaviorType`
3. Настройте уникальные способности

### Создание новых квестов
1. Используйте `UQuestSystem`
2. Создайте `FQuestData` с целями
3. Привяжите к NPC через Blueprint

## 📱 Сборка под платформы

### Windows
```bash
# В редакторе: File → Package Project → Windows
```

### Android
```bash
# Требуется: Android SDK, NDK, JDK
# File → Package Project → Android
```

### iOS
```bash
# Требуется: macOS, Xcode
# File → Package Project → iOS
```

## ⚠️ Известные ограничения

- Для компиляции требуется установленный Unreal Engine 5.3+
- Мультиплеер требует настройки Steamworks
- Некоторые функции (AR, облачные сохранения) требуют дополнительной настройки
- Мобильная оптимизация требует настройки LOD и текстур

## 📄 Лицензия

Copyright 2024 YourCompany. Все права защищены.

## 🤝 Поддержка

- Документация: `/workspace/UE5UltimateGame/README.md`
- Исходный код: `/workspace/UE5UltimateGame/Source/`
- Конфигурация: `/workspace/UE5UltimateGame/Config/`

---

**Создано с помощью Unreal Engine 5**
*Версия проекта: 1.0.0*
