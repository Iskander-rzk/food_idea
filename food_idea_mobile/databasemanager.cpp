#include "databasemanager.h"

#include <QRandomGenerator>
#include <QSettings>
#include <algorithm>

namespace {
QString normalized(const QString &value)
{
    return value.trimmed().toLower();
}

bool containsProduct(const QString &available, const QString &ingredient)
{
    const QString have = normalized(available);
    const QString need = normalized(ingredient);
    return !have.isEmpty() && !need.isEmpty() && (have.contains(need) || need.contains(have));
}
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
    initializeData();
}

void DatabaseManager::initializeData()
{
    m_dishes.clear();
    m_nextId = 1;

    m_dishes << Dish(m_nextId++, "Овсянка с яблоком", Dish::BREAKFAST,
                     "1. Залейте 50 г овсянки 200 мл воды\n2. Варите 5–7 мин\n3. Добавьте нарезанное яблоко",
                     320, 8.5, 4.0, 58.0, {"овсяные хлопья", "яблоко", "вода"});

    m_dishes << Dish(m_nextId++, "Омлет с помидорами", Dish::BREAKFAST,
                     "1. Взбейте 2 яйца\n2. Нарежьте помидор\n3. Жарьте 5–7 мин",
                     280, 18.0, 19.0, 5.0, {"яйцо", "помидор", "соль"});

    m_dishes << Dish(m_nextId++, "Творог с ягодами", Dish::BREAKFAST,
                     "1. 200 г творога смешайте с 100 г ягод",
                     250, 25.0, 5.0, 15.0, {"творог 5%", "клубника"});

    m_dishes << Dish(m_nextId++, "Курица с гречкой", Dish::LUNCH,
                     "1. Отварите 80 г гречки\n2. Обжарьте 150 г куриной грудки\n3. Смешайте",
                     480, 45.0, 10.0, 50.0, {"куриная грудка", "гречка", "лук"});

    m_dishes << Dish(m_nextId++, "Рыба с овощами", Dish::LUNCH,
                     "1. Запеките 200 г рыбы с брокколи 20 мин при 180°C",
                     380, 35.0, 12.0, 18.0, {"филе трески", "брокколи", "лимон"});

    m_dishes << Dish(m_nextId++, "Творог с огурцом и зеленью", Dish::DINNER,
                     "1. 200 г творога + натёртый огурец + укроп",
                     220, 28.0, 8.0, 9.0, {"творог 5%", "огурец", "укроп"});

    m_dishes << Dish(m_nextId++, "Салат с тунцом", Dish::DINNER,
                     "1. Огурец, помидор, тунец из банки",
                     260, 25.0, 12.0, 12.0, {"тунец консервированный", "помидор", "огурец"});

    m_dishes << Dish(m_nextId++, "Греческий йогурт + орехи", Dish::SNACK,
                     "1. 150 г йогурта + 15 г орехов",
                     220, 12.0, 14.0, 14.0, {"греческий йогурт", "грецкие орехи"});

    m_dishes << Dish(m_nextId++, "Яблоко с арахисовой пастой", Dish::SNACK,
                     "1. Нарежьте яблоко, намажьте 15 г пасты",
                     250, 6.0, 16.0, 25.0, {"яблоко", "арахисовая паста"});

    m_dishes << Dish(m_nextId++, "Творог с мёдом", Dish::SNACK,
                     "1. 150 г творога + 1 ч.л. мёда",
                     200, 22.0, 5.0, 18.0, {"творог 5%", "мёд"});

    loadCustomDishes();
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

    for (const Dish &dish : m_dishes) {
        if (dish.getType() != type || forbidden.contains(dish.getId()))
            continue;

        int missing = 0;
        for (const QString &ingredient : dish.getIngredients()) {
            bool found = false;
            for (const QString &have : available) {
                if (containsProduct(have, ingredient)) {
                    found = true;
                    break;
                }
            }
            if (!found)
                ++missing;
        }

        if (missing <= maxMissing)
            candidates.append({dish, missing});
    }

    if (candidates.isEmpty()) {
        for (const Dish &dish : m_dishes) {
            if (dish.getType() == type && !forbidden.contains(dish.getId()))
                candidates.append({dish, 999});
        }
    }

    if (candidates.isEmpty())
        return Dish();

    std::sort(candidates.begin(), candidates.end(),
              [](const Candidate &a, const Candidate &b) {
                  return a.missingCount < b.missingCount;
              });

    const int bestMissing = candidates.first().missingCount;
    QList<Dish> bestGroup;
    for (const Candidate &candidate : candidates) {
        if (candidate.missingCount == bestMissing)
            bestGroup.append(candidate.dish);
    }

    return bestGroup.at(QRandomGenerator::global()->bounded(bestGroup.size()));
}

bool DatabaseManager::addDish(const Dish &dish)
{
    Dish newDish(m_nextId++, dish.getName(), dish.getType(), dish.getRecipe(),
                 dish.getCalories(), dish.getProtein(), dish.getFat(), dish.getCarbs(),
                 dish.getIngredients(), dish.getImagePath());
    m_dishes.append(newDish);
    saveCustomDishes();
    return true;
}

void DatabaseManager::loadCustomDishes()
{
    QSettings settings("MealApp", "Planner");
    const int size = settings.beginReadArray("CustomDishes");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        const int id = settings.value("id").toInt();
        const int type = settings.value("type").toInt();
        if (id <= 0 || type < 0 || type >= Dish::MEAL_TYPE_COUNT)
            continue;

        Dish dish(id,
                  settings.value("name").toString(),
                  static_cast<Dish::MealType>(type),
                  settings.value("recipe").toString(),
                  settings.value("calories").toDouble(),
                  settings.value("protein").toDouble(),
                  settings.value("fat").toDouble(),
                  settings.value("carbs").toDouble(),
                  settings.value("ingredients").toStringList(),
                  settings.value("imagePath").toString());

        m_dishes.append(dish);
        m_nextId = std::max(m_nextId, id + 1);
    }

    settings.endArray();
}

void DatabaseManager::saveCustomDishes() const
{
    QSettings settings("MealApp", "Planner");
    settings.beginWriteArray("CustomDishes");

    int index = 0;
    for (const Dish &dish : m_dishes) {
        if (dish.getId() < 11)
            continue;

        settings.setArrayIndex(index++);
        settings.setValue("id", dish.getId());
        settings.setValue("type", static_cast<int>(dish.getType()));
        settings.setValue("name", dish.getName());
        settings.setValue("recipe", dish.getRecipe());
        settings.setValue("calories", dish.getCalories());
        settings.setValue("protein", dish.getProtein());
        settings.setValue("fat", dish.getFat());
        settings.setValue("carbs", dish.getCarbs());
        settings.setValue("ingredients", dish.getIngredients());
        settings.setValue("imagePath", dish.getImagePath());
    }

    settings.endArray();
}

void DatabaseManager::savePreviousDishes(const QMap<Dish::MealType, int> &previousIds)
{
    QSettings settings("MealApp", "Planner");
    settings.beginGroup("Previous");
    settings.remove("");
    for (auto it = previousIds.cbegin(); it != previousIds.cend(); ++it)
        settings.setValue(QString::number(static_cast<int>(it.key())), it.value());
    settings.endGroup();
}

QMap<Dish::MealType, int> DatabaseManager::loadPreviousDishes() const
{
    QMap<Dish::MealType, int> result;
    QSettings settings("MealApp", "Planner");
    settings.beginGroup("Previous");

    for (const QString &key : settings.childKeys()) {
        bool ok = false;
        const int type = key.toInt(&ok);
        if (ok && type >= 0 && type < Dish::MEAL_TYPE_COUNT)
            result[static_cast<Dish::MealType>(type)] = settings.value(key).toInt();
    }

    settings.endGroup();
    return result;
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
