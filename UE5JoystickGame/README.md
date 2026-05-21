# UE5 Joystick Game - Полноценная игра с мультиплеером

Полнопроектный шаблон игры на Unreal Engine 5 с виртуальным джойстиком, системой взаимодействия, боевой механикой, ИИ врагов и онлайн-мультиплеером.

## 🎮 Возможности

### Основные системы
- **Виртуальный джойстик** - поддержка touch, клавиатуры (WASD) и геймпада
- **Система взаимодействия** - подбор предметов, открытие дверей, активация объектов
- **Боевая система** - прицеливание, атака, кулдауны, урон
- **Здоровье** - компонент здоровья с событиями смерти и восстановления
- **ИИ врагов** - патрулирование, преследование, атака
- **Онлайн мультиплеер** - создание/поиск сессий через Steam

### Управление

#### ПК
- **WASD** - перемещение
- **Shift** - бег
- **Пробел** - прыжок
- **ЛКМ** - атака
- **ПКМ** - прицеливание
- **E** - взаимодействие

#### Мобильные
- **Левый стик** - перемещение
- **Правый стик** - камера
- **Кнопка атаки** - атака
- **Кнопка взаимодействия** - взаимодействие

#### Геймпад
- **Левый стик** - перемещение
- **Правый стик** - камера
- **A/Cross** - прыжок
- **B/Circle** - бег
- **R2/RT** - атака
- **L2/LT** - прицеливание
- **X/Square** - взаимодействие

## 📁 Структура проекта

```
UE5JoystickGame/
├── Source/
│   └── UE5JoystickGame/
│       ├── VirtualJoystickComponent.h/cpp    # Виртуальный джойстик
│       ├── JoystickPlayerCharacter.h/cpp     # Персонаж игрока
│       ├── InteractionComponent.h/cpp        # Система взаимодействия
│       ├── InteractableObject.h/cpp          # Базовый класс взаимодействуемых объектов
│       ├── CombatComponent.h/cpp             # Боевая система
│       ├── HealthComponent.h/cpp             # Здоровье
│       ├── BaseEnemyAI.h/cpp                 # ИИ врагов
│       ├── MultiplayerGameMode.h/cpp         # Мультиплеер сессии
│       └── UE5JoystickGame.Build.cs          # Настройки сборки
├── Config/
│   └── DefaultEngine.ini                     # Конфигурация движка
└── README.md
```

## 🚀 Установка

### Требования
- Unreal Engine 5.3 или выше
- Visual Studio 2022 (для Windows)
- Steam (для мультиплеера)

### Шаги установки

1. **Скопируйте проект** в папку проектов Unreal Engine:
   ```
   C:\Users\[User]\Documents\Unreal Projects\UE5JoystickGame
   ```

2. **Откройте проект** через Epic Games Launcher или дважды кликнув на `.uproject` файл

3. **Дождитесь компиляции** кода и загрузки ассетов

4. **Настройте Steam** (для мультиплеера):
   - Убедитесь, что Steam запущен
   - Для тестирования используется Steam Dev App ID 480

## 🔧 Настройка мультиплеера

### Для Steam
1. Откройте `Config/DefaultEngine.ini`
2. Замените `SteamDevAppId=480` на ваш App ID
3. Включите плагин "Online Subsystem Steam" в редакторе

### Создание сервера (Blueprint)
```
Create Server Session (Session Name: "MyRoom", Max Players: 4)
→ On Session Created Success → Load Map / Open Level
```

### Поиск сессий (Blueprint)
```
Find Sessions
→ On Sessions Found → Показать список в UI
→ Join Session (Index)
→ On Join Session Success → Connect to server
```

## 🎯 Использование компонентов

### Добавление взаимодействия персонажу
1. Добавьте компонент `Interaction Component` к персонажу
2. Создайте Blueprint от `InteractableObject`
3. Реализуйте события `OnInteractBegin` и `OnInteractEnd`

### Добавление боевой системы
1. Добавьте компонент `Combat Component` к персонажу
2. Настройте параметры урона и дальности
3. Реализуйте событие `OnDamageDealt` для визуальных эффектов

### Создание врага
1. Создайте Blueprint от `BaseEnemyAI`
2. Добавьте меш и компоненты
3. Настройте точки патрулирования
4. Реализуйте `OnEnemyDied` для логики смерти

### Настройка здоровья
1. Добавьте компонент `Health Component` к любому актёру
2. Привяжите события `OnHealthChanged` к UI
3. Используйте `TakeDamage()` и `Heal()` из Blueprint

## 📝 Примеры Blueprint

### Взаимодействие
```
Event Tick → Can Interact? → Yes → Show Interaction Widget
Event E Pressed → Start Interaction
```

### Атака
```
Event LMB → Start Aiming (hold)
Event LMB Released → Perform Attack
```

### Мультиплеер меню
```
Button Create → Create Server Session("Room", 4)
Button Find → Find Sessions()
List Item Clicked → Join Session(Index)
```

## ⚙️ Сборка проекта

### Windows
```bash
# Через Unreal Editor
File → Package Project → Windows

# Или через командную строку
Engine\Build\BatchFiles\Build.bat UE5JoystickGame Win64 Development
```

### Android
1. Включите плагин "Android Platform"
2. Настройте SDK/NDK в настройках проекта
3. File → Package Project → Android

### iOS
1. Включите плагин "iOS Platform"
2. Настройте подписку в Xcode
3. File → Package Project → iOS

## 🐛 Решение проблем

### Мультиплеер не работает
- Проверьте, что Steam запущен
- Убедитесь, что firewall не блокирует соединение
- Проверьте настройки в DefaultEngine.ini

### Ошибки компиляции
- Убедитесь, что установлен Visual Studio 2022
- Перегенерируйте файлы проекта (правый клик на .uproject → Generate Visual Studio project files)

### Проблемы с джойстиком на мобильных
- Проверьте настройки Input System в проекте
- Убедитесь, что Enabled Enhanced Input System включен

## 📄 Лицензия

Этот проект предоставляется как есть для образовательных целей.

## 🤝 Вклад

Не стесняйтесь создавать issues и pull requests для улучшения проекта!
