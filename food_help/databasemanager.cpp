#include "databasemanager.h"
#include <QRandomGenerator>
#include <QSettings>
#include <algorithm>
#include <QFile>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::DatabaseManager()
{
    initializeData();
}

void DatabaseManager::initializeData()
{
    m_dishes.clear();
    m_nextId = 1;

    // Завтраки
    m_dishes << Dish(m_nextId++, "Овсянка с яблоком", Dish::BREAKFAST,
                     "1. Залейте 50 г овсянки 200 мл воды\n2. Варите 5–7 мин\n3. Добавьте нарезанное яблоко",
                     320, 8.5, 4.0, 58.0, {"овсяные хлопья", "яблоко", "вода"});

    m_dishes << Dish(m_nextId++, "Омлет с помидорами", Dish::BREAKFAST,
                     "1. Взбейте 2 яйца\n2. Нарежьте помидор\n3. Жарьте 5–7 мин",
                     280, 18.0, 19.0, 5.0, {"яйцо", "помидор", "соль"});

    m_dishes << Dish(m_nextId++, "Творог с ягодами", Dish::BREAKFAST,
                     "1. 200 г творога смешайте с 100 г ягод",
                     250, 25.0, 5.0, 15.0, {"творог 5%", "клубника"});

    // Обеды
    m_dishes << Dish(m_nextId++, "Курица с гречкой", Dish::LUNCH,
                     "1. Отварите 80 г гречки\n2. Обжарьте 150 г куриной грудки\n3. Смешайте",
                     480, 45.0, 10.0, 50.0, {"куриная грудка", "гречка", "лук"});

    m_dishes << Dish(m_nextId++, "Рыба с овощами", Dish::LUNCH,
                     "1. Запеките 200 г рыбы с брокколи 20 мин при 180°C",
                     380, 35.0, 12.0, 18.0, {"филе трески", "брокколи", "лимон"});

    // Ужины
    m_dishes << Dish(m_nextId++, "Творог с огурцом и зеленью", Dish::DINNER,
                     "1. 200 г творога + натёртый огурец + укроп",
                     220, 28.0, 8.0, 9.0, {"творог 5%", "огурец", "укроп"});

    m_dishes << Dish(m_nextId++, "Салат с тунцом", Dish::DINNER,
                     "1. Огурец, помидор, тунец из банки",
                     260, 25.0, 12.0, 12.0, {"тунец консервированный", "помидор", "огурец"});

    // Перекусы
    m_dishes << Dish(m_nextId++, "Греческий йогурт + орехи", Dish::SNACK,
                     "1. 150 г йогурта + 15 г орехов",
                     220, 12.0, 14.0, 14.0, {"греческий йогурт", "грецкие орехи"});

    m_dishes << Dish(m_nextId++, "Яблоко с арахисовой пастой", Dish::SNACK,
                     "1. Нарежьте яблоко, намажьте 15 г пасты",
                     250, 6.0, 16.0, 25.0, {"яблоко", "арахисовая паста"});

    m_dishes << Dish(m_nextId++, "Творог с мёдом", Dish::SNACK,
                     "1. 150 г творога + 1 ч.л. мёда",
                     200, 22.0, 5.0, 18.0, {"творог 5%", "мёд"});
}

QList<Dish> DatabaseManager::getAllDishes() const
{
    return m_dishes;
}

Dish DatabaseManager::getBestDishForType(Dish::MealType type,
                                         const QStringList &available,
                                         const QSet<int> &forbidden,
                                         int maxMissing) const
{
    struct Candidate {
        Dish dish;
        int missingCount;
    };

    QList<Candidate> candidates;

    for (const auto &d : m_dishes) {
        if (d.getType() != type || forbidden.contains(d.getId())) continue;

        int missing = 0;
        for (const auto &ing : d.getIngredients()) {
            QString norm = ing.trimmed().toLower();
            bool found = false;
            for (const auto &have : available) {
                if (have.trimmed().toLower().contains(norm) || norm.contains(have.trimmed().toLower())) {
                    found = true;
                    break;
                }
            }
            if (!found) ++missing;
        }

        if (missing <= maxMissing) {
            candidates << Candidate{d, missing};
        }
    }

    if (candidates.isEmpty()) {
        for (const auto &d : m_dishes) {
            if (d.getType() == type && !forbidden.contains(d.getId()))
                candidates << Candidate{d, 999};
        }
    }

    if (candidates.isEmpty()) return Dish();

    std::sort(candidates.begin(), candidates.end(),
              [](const Candidate &a, const Candidate &b){ return a.missingCount < b.missingCount; });

    int bestMissing = candidates.first().missingCount;
    QList<Dish> bestGroup;
    for (const auto &c : candidates) {
        if (c.missingCount == bestMissing) bestGroup << c.dish;
    }

    return bestGroup.at(QRandomGenerator::global()->bounded(bestGroup.size()));
}

bool DatabaseManager::addDish(const Dish &dish)
{
    Dish newDish(m_nextId++, dish.getName(), dish.getType(), dish.getRecipe(),
                 dish.getCalories(), dish.getProtein(), dish.getFat(), dish.getCarbs(),
                 dish.getIngredients(), dish.getImagePath());
    m_dishes.append(newDish);
    return true;
}

void DatabaseManager::savePreviousDishes(const QMap<Dish::MealType, int> &prevIds)
{
    QSettings settings("MealApp", "Planner");
    settings.beginGroup("Previous");
    for (auto it = prevIds.cbegin(); it != prevIds.cend(); ++it) {
        settings.setValue(QString::number(it.key()), it.value());
    }
    settings.endGroup();
}

QMap<Dish::MealType, int> DatabaseManager::loadPreviousDishes() const
{
    QMap<Dish::MealType, int> map;
    QSettings settings("MealApp", "Planner");
    settings.beginGroup("Previous");
    QStringList keys = settings.childKeys();
    for (const QString &k : keys) {
        bool ok;
        int t = k.toInt(&ok);
        if (ok && t >= 0 && t < Dish::MEAL_TYPE_COUNT) {
            map[static_cast<Dish::MealType>(t)] = settings.value(k).toInt();
        }
    }
    settings.endGroup();
    return map;
}

void DatabaseManager::saveUserProducts(const QStringList &products)
{
    QSettings settings("MealApp", "Planner");
    settings.setValue("User/Products", products);
}

QStringList DatabaseManager::loadUserProducts() const
{
    QSettings settings("MealApp", "Planner");
    return settings.value("User/Products").toStringList();
}
