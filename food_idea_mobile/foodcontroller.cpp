#include "foodcontroller.h"

#include <QSet>
#include <QSettings>
#include <QRandomGenerator>
#include <QtMath>
#include <limits>

namespace {
QString normalize(const QString &value)
{
    return value.trimmed().toLower();
}

bool productMatches(const QString &have, const QString &need)
{
    const QString h = normalize(have);
    const QString n = normalize(need);
    return !h.isEmpty() && !n.isEmpty() && (h.contains(n) || n.contains(h));
}
}

FoodController::FoodController(QObject *parent)
    : QObject(parent)
    , m_db(DatabaseManager::instance())
    , m_products(m_db.loadUserProducts())
    , m_previousIds(m_db.loadPreviousDishes())
{
    QSettings settings("MealApp", "Planner");
    m_excludedNames = settings.value("Preferences/ExcludedNames").toStringList();
    const QVariantList flags = settings.value("Preferences/ExcludedAllergy").toList();
    for (int i = 0; i < m_excludedNames.size(); ++i)
        m_excludedAllergy.append(i < flags.size() ? flags.at(i).toBool() : false);
    for (const QVariant &item : settings.value("Preferences/Favorites").toList())
        m_favorites.insert(item.toInt());
    m_shopping = settings.value("Preferences/Shopping").toStringList();
    m_shoppingDone = QSet<QString>::fromList(settings.value("Preferences/ShoppingDone").toStringList());
    m_goalCalories = settings.value("Goals/Calories", 2000).toDouble();
    m_goalProtein = settings.value("Goals/Protein", 100).toDouble();
    m_goalFat = settings.value("Goals/Fat", 70).toDouble();
    m_goalCarbs = settings.value("Goals/Carbs", 250).toDouble();
    for (int type = 0; type < Dish::MEAL_TYPE_COUNT; ++type) {
        const QString key = QString("History/Recent_%1").arg(type);
        const QVariantList stored = settings.value(key).toList();
        QList<int> recent;
        for (const QVariant &value : stored) {
            const int id = value.toInt();
            if (id >= 0 && !recent.contains(id))
                recent.append(id);
        }
        const Dish::MealType mealType = static_cast<Dish::MealType>(type);
        if (recent.isEmpty() && m_previousIds.contains(mealType))
            recent.append(m_previousIds.value(mealType));
        m_recentChoices.insert(mealType, recent);
    }
    generateDay();
}

QVariantList FoodController::today() const
{
    QVariantList result;
    const Dish::MealType order[] = {
        Dish::BREAKFAST, Dish::LUNCH, Dish::DINNER, Dish::SNACK
    };

    for (Dish::MealType type : order) {
        const Dish dish = m_today.value(type);
        if (dish.getId() == -1) {
            QVariantMap placeholder;
            placeholder["id"] = -1;
            placeholder["mealType"] = static_cast<int>(type);
            placeholder["typeName"] = Dish(-1, "", type, "", 0, 0, 0, 0).getTypeString();
            placeholder["name"] = "Блюдо не найдено";
            result.append(placeholder);
        } else {
            result.append(dishToMap(dish));
        }
    }
    return result;
}

QVariantList FoodController::allRecipes() const
{
    QVariantList result;
    for (const Dish &dish : m_db.getAllDishes())
        result.append(dishToMap(dish));
    return result;
}

QVariantList FoodController::myRecipes() const
{
    QVariantList result;
    if (m_products.isEmpty())
        return result;

    for (const Dish &dish : m_db.getAllDishes()) {
        if (!dishExcluded(dish) && missingIngredients(dish).size() <= 2)
            result.append(dishToMap(dish));
    }
    return result;
}

QStringList FoodController::products() const
{
    return m_products;
}

int FoodController::recipeCount() const
{
    return m_db.getAllDishes().size();
}

void FoodController::rememberChoice(Dish::MealType type, int id)
{
    if (id < 0)
        return;
    QList<int> &recent = m_recentChoices[type];
    recent.removeAll(id);
    recent.prepend(id);
    while (recent.size() > 3)
        recent.removeLast();
    QSettings settings("MealApp", "Planner");
    QVariantList values;
    for (int value : recent)
        values.append(value);
    settings.setValue(QString("History/Recent_%1").arg(static_cast<int>(type)), values);
}

void FoodController::generateDay()
{
    // Prefer recipes not shown recently. If the catalogue is small,
    // allow older recipes, but never repeat the current one when an alternative exists.
    const Dish::MealType order[] = { Dish::BREAKFAST, Dish::LUNCH, Dish::DINNER, Dish::SNACK };
    QSet<int> selected;
    for (Dish::MealType type : order) {
        QSet<int> current;
        const Dish oldDish = m_today.value(type);
        if (oldDish.getId() >= 0)
            current.insert(oldDish.getId());
        else if (m_previousIds.contains(type))
            current.insert(m_previousIds.value(type));

        QSet<int> avoid = selected;
        avoid.unite(current);
        for (int id : m_recentChoices.value(type))
            avoid.insert(id);

        Dish dish = selectDish(type, avoid, 2);
        if (dish.getId() == -1) {
            QSet<int> blocked = selected;
            blocked.unite(current);
            dish = selectDish(type, blocked, 2);
        }
        if (dish.getId() == -1)
            dish = selectDish(type, selected, 2); // Only one eligible recipe may exist.
        m_today[type] = dish;
        if (dish.getId() >= 0) {
            selected.insert(dish.getId());
            rememberChoice(type, dish.getId());
            m_previousIds[type] = dish.getId();
        }
    }
    m_db.savePreviousDishes(m_previousIds);
    emit dataChanged();
}

void FoodController::replaceMeal(int mealType)
{
    if (mealType < 0 || mealType >= Dish::MEAL_TYPE_COUNT)
        return;
    const auto type = static_cast<Dish::MealType>(mealType);
    QSet<int> current;
    for (auto it = m_today.cbegin(); it != m_today.cend(); ++it)
        if (it.value().getId() >= 0)
            current.insert(it.value().getId());
    QSet<int> avoid = current;
    for (int id : m_recentChoices.value(type))
        avoid.insert(id);
    Dish dish = selectDish(type, avoid, 2);
    if (dish.getId() == -1)
        dish = selectDish(type, current, 2);
    if (dish.getId() == -1) {
        emit messageChanged("Нет других блюд с учётом исключений");
        return;
    }
    m_today[type] = dish;
    m_previousIds[type] = dish.getId();
    rememberChoice(type, dish.getId());
    m_db.savePreviousDishes(m_previousIds);
    emit dataChanged();
}

void FoodController::addToDayMenu(int dishId)
{
    const Dish dish = dishById(dishId);
    if (dish.getId() == -1) {
        emit messageChanged("Блюдо не найдено");
        return;
    }
    if (dishExcluded(dish)) {
        emit messageChanged("Нельзя добавить блюдо: есть исключённый ингредиент");
        return;
    }
    m_today[dish.getType()] = dish;
    m_previousIds[dish.getType()] = dish.getId();
    rememberChoice(dish.getType(), dish.getId());
    m_db.savePreviousDishes(m_previousIds);
    emit dataChanged();
    emit messageChanged(QString("«%1» добавлено в меню").arg(dish.getName()));
}

void FoodController::addProduct(const QString &product)
{
    const QString value = product.trimmed();
    if (value.isEmpty())
        return;

    for (const QString &existing : m_products) {
        if (normalize(existing) == normalize(value)) {
            emit messageChanged("Такой продукт уже есть в списке");
            return;
        }
    }

    for (const QString &excluded : m_excludedNames) {
        if (productMatches(value, excluded)) {
            emit messageChanged("Этот продукт находится в исключениях");
            return;
        }
    }
    m_products.append(value);
    m_db.saveUserProducts(m_products);
    emit dataChanged();
    generateDay();
}

void FoodController::removeProduct(int index)
{
    if (index < 0 || index >= m_products.size())
        return;

    m_products.removeAt(index);
    m_db.saveUserProducts(m_products);
    emit dataChanged();
    generateDay();
}

bool FoodController::addDish(const QString &name,
                             int mealType,
                             const QString &recipe,
                             double calories,
                             double protein,
                             double fat,
                             double carbs,
                             const QString &ingredients,
                             const QString &imagePath)
{
    const QString cleanName = name.trimmed();
    if (cleanName.isEmpty() || mealType < 0 || mealType >= Dish::MEAL_TYPE_COUNT) {
        emit messageChanged("Заполните название и тип блюда");
        return false;
    }

    QStringList ingredientList;
    const QStringList raw = ingredients.split(',', Qt::SkipEmptyParts);
    for (QString value : raw) {
        value = value.trimmed();
        if (!value.isEmpty())
            ingredientList.append(value);
    }

    Dish dish(-1,
              cleanName,
              static_cast<Dish::MealType>(mealType),
              recipe.trimmed(),
              qMax(0.0, calories),
              qMax(0.0, protein),
              qMax(0.0, fat),
              qMax(0.0, carbs),
              ingredientList,
              imagePath);

    if (!m_db.addDish(dish)) {
        emit messageChanged("Не удалось сохранить рецепт");
        return false;
    }

    emit dataChanged();
    emit messageChanged(QString("Рецепт «%1» добавлен").arg(cleanName));
    generateDay();
    return true;
}

QVariantMap FoodController::recipeById(int id) const
{
    const Dish dish = dishById(id);
    return dish.getId() == -1 ? QVariantMap{} : dishToMap(dish);
}

QVariantMap FoodController::dishToMap(const Dish &dish, bool includeMissing) const
{
    QVariantMap map;
    map["id"] = dish.getId();
    map["name"] = dish.getName();
    map["mealType"] = static_cast<int>(dish.getType());
    map["typeName"] = dish.getTypeString();
    map["recipe"] = dish.getRecipe();
    map["calories"] = dish.getCalories();
    map["protein"] = dish.getProtein();
    map["fat"] = dish.getFat();
    map["carbs"] = dish.getCarbs();
    map["ingredients"] = dish.getIngredients();
    map["imagePath"] = dish.getImagePath();
    map["favorite"] = m_favorites.contains(dish.getId());
    map["excluded"] = dishExcluded(dish);
    if (includeMissing) {
        const QStringList missing = missingIngredients(dish);
        QVariantList missingList;
        for (const QString &value : missing)
            missingList.append(value);
        map["missing"] = missingList;
        map["missingString"] = missingString(dish);
    }
    return map;
}

QStringList FoodController::missingIngredients(const Dish &dish) const
{
    QStringList missing;
    for (const QString &ingredient : dish.getIngredients()) {
        bool found = false;
        for (const QString &product : m_products) {
            if (productMatches(product, ingredient)) {
                found = true;
                break;
            }
        }
        if (!found)
            missing.append(ingredient);
    }
    return missing;
}

QString FoodController::missingString(const Dish &dish) const
{
    const QStringList missing = missingIngredients(dish);
    return missing.isEmpty() ? "всё есть" : missing.join(", ");
}

Dish FoodController::dishById(int id) const
{
    for (const Dish &dish : m_db.getAllDishes()) {
        if (dish.getId() == id)
            return dish;
    }
    return Dish();
}

// User preferences and derived daily nutrition data.
void FoodController::saveExtras() const
{
    QSettings settings("MealApp", "Planner");
    settings.setValue("Preferences/ExcludedNames", m_excludedNames);
    QVariantList flags;
    for (bool item : m_excludedAllergy) flags.append(item);
    settings.setValue("Preferences/ExcludedAllergy", flags);
    QVariantList favorites;
    for (int id : m_favorites) favorites.append(id);
    settings.setValue("Preferences/Favorites", favorites);
    settings.setValue("Preferences/Shopping", m_shopping);
    settings.setValue("Preferences/ShoppingDone", QStringList(m_shoppingDone.values()));
    settings.setValue("Goals/Calories", m_goalCalories);
    settings.setValue("Goals/Protein", m_goalProtein);
    settings.setValue("Goals/Fat", m_goalFat);
    settings.setValue("Goals/Carbs", m_goalCarbs);
}

bool FoodController::dishExcluded(const Dish &dish) const
{
    for (const QString &ingredient : dish.getIngredients())
        for (const QString &excluded : m_excludedNames)
            if (productMatches(excluded, ingredient)) return true;
    return false;
}

bool FoodController::isExcluded(int dishId) const
{
    const Dish dish = dishById(dishId);
    return dish.getId() >= 0 && dishExcluded(dish);
}

QVariantList FoodController::excludedProducts() const
{
    QVariantList result;
    for (int i = 0; i < m_excludedNames.size(); ++i) {
        QVariantMap item;
        item["name"] = m_excludedNames.at(i);
        item["allergy"] = m_excludedAllergy.value(i);
        result.append(item);
    }
    return result;
}

void FoodController::addExcludedProduct(const QString &name, bool allergy)
{
    const QString value = name.trimmed();
    if (value.isEmpty()) return;
    for (const QString &existing : m_excludedNames)
        if (normalize(existing) == normalize(value)) {
            emit messageChanged("Продукт уже исключён");
            return;
        }
    m_excludedNames.append(value);
    m_excludedAllergy.append(allergy);
    for (int i = m_products.size()-1; i >= 0; --i)
        if (productMatches(value, m_products.at(i))) m_products.removeAt(i);
    m_db.saveUserProducts(m_products);
    saveExtras();
    generateDay();
    emit messageChanged("Исключение сохранено — меню обновлено");
}

void FoodController::removeExcludedProduct(int index)
{
    if (index < 0 || index >= m_excludedNames.size()) return;
    m_excludedNames.removeAt(index);
    m_excludedAllergy.removeAt(index);
    saveExtras();
    generateDay();
}

void FoodController::toggleFavorite(int dishId)
{
    if (dishById(dishId).getId() < 0) return;
    if (m_favorites.contains(dishId)) m_favorites.remove(dishId);
    else m_favorites.insert(dishId);
    saveExtras();
    emit dataChanged();
}

bool FoodController::isFavorite(int dishId) const { return m_favorites.contains(dishId); }

QVariantList FoodController::favoriteRecipes() const
{
    QVariantList result;
    for (const Dish &dish : m_db.getAllDishes())
        if (m_favorites.contains(dish.getId())) result.append(dishToMap(dish));
    return result;
}

QVariantList FoodController::shoppingList() const
{
    QVariantList result;
    for (const QString &name : m_shopping) {
        QVariantMap row;
        row["name"] = name;
        row["done"] = m_shoppingDone.contains(normalize(name));
        result.append(row);
    }
    return result;
}

void FoodController::addShoppingItem(const QString &item)
{
    const QString value = item.trimmed();
    if (value.isEmpty()) return;
    for (const QString &existing : m_shopping)
        if (normalize(existing) == normalize(value)) return;
    m_shopping.append(value);
    saveExtras();
    emit dataChanged();
}

void FoodController::removeShoppingItem(int index)
{
    if (index < 0 || index >= m_shopping.size()) return;
    m_shoppingDone.remove(normalize(m_shopping.at(index)));
    m_shopping.removeAt(index);
    saveExtras();
    emit dataChanged();
}

void FoodController::toggleShoppingItem(int index)
{
    if (index < 0 || index >= m_shopping.size()) return;
    const QString key = normalize(m_shopping.at(index));
    if (m_shoppingDone.contains(key)) m_shoppingDone.remove(key);
    else m_shoppingDone.insert(key);
    saveExtras();
    emit dataChanged();
}

QStringList FoodController::missingForToday() const
{
    QStringList result;
    QSet<QString> seen;
    for (auto it = m_today.cbegin(); it != m_today.cend(); ++it) {
        if (it.value().getId() < 0) continue;
        for (const QString &ingredient : missingIngredients(it.value())) {
            const QString key = normalize(ingredient);
            if (!seen.contains(key)) {
                seen.insert(key);
                result.append(ingredient);
            }
        }
    }
    return result;
}

void FoodController::addMissingToShopping()
{
    for (const QString &item : missingForToday()) {
        bool exists = false;
        for (const QString &current : m_shopping)
            if (normalize(current) == normalize(item)) { exists = true; break; }
        if (!exists) m_shopping.append(item);
    }
    saveExtras();
    emit dataChanged();
    emit messageChanged("Недостающие продукты добавлены в покупки");
}

QVariantMap FoodController::nutritionGoals() const
{
    QVariantMap result;
    result["calories"] = m_goalCalories;
    result["protein"] = m_goalProtein;
    result["fat"] = m_goalFat;
    result["carbs"] = m_goalCarbs;
    return result;
}

QVariantMap FoodController::dailyTotals() const
{
    double calories=0, protein=0, fat=0, carbs=0;
    for (const Dish &dish : m_today) {
        if (dish.getId() < 0) continue;
        calories += dish.getCalories();
        protein += dish.getProtein();
        fat += dish.getFat();
        carbs += dish.getCarbs();
    }
    QVariantMap result;
    result["calories"] = calories;
    result["protein"] = protein;
    result["fat"] = fat;
    result["carbs"] = carbs;
    return result;
}

void FoodController::setNutritionGoals(double calories, double protein, double fat, double carbs)
{
    if (!qIsFinite(calories) || !qIsFinite(protein) || !qIsFinite(fat) || !qIsFinite(carbs) ||
        calories < 500 || calories > 10000 || protein < 0 || fat < 0 || carbs < 0 ||
        protein > 1000 || fat > 1000 || carbs > 1500) {
        emit messageChanged("Проверьте значения целей по КБЖУ");
        return;
    }
    m_goalCalories = calories;
    m_goalProtein = protein;
    m_goalFat = fat;
    m_goalCarbs = carbs;
    saveExtras();
    generateDay();
    emit messageChanged("Цели сохранены — меню обновлено");
}

Dish FoodController::selectDish(Dish::MealType type, const QSet<int> &avoid, int maxMissing) const
{
    QList<Dish> eligible;
    QList<Dish> preferred;
    for (const Dish &dish : m_db.getAllDishes()) {
        if (dish.getType() != type || avoid.contains(dish.getId()) || dishExcluded(dish))
            continue;
        eligible.append(dish);
        if (missingIngredients(dish).size() <= maxMissing)
            preferred.append(dish);
    }
    const QList<Dish> &candidates = preferred.isEmpty() ? eligible : preferred;
    if (candidates.isEmpty()) return Dish();

    // Soft preference for nutritional targets and available ingredients.
    // Every eligible dish keeps a nonzero probability of being chosen.
    const double shares[] = {0.25, 0.35, 0.30, 0.10};
    const double share = shares[static_cast<int>(type)];
    QList<double> weights;
    double totalWeight = 0.0;
    for (const Dish &dish : candidates) {
        const int missing = missingIngredients(dish).size();
        const double kcalDiff = qAbs(dish.getCalories() - m_goalCalories * share) / qMax(1.0, m_goalCalories * share);
        const double proteinDiff = qAbs(dish.getProtein() - m_goalProtein * share) / qMax(1.0, m_goalProtein * share);
        const double fatDiff = qAbs(dish.getFat() - m_goalFat * share) / qMax(1.0, m_goalFat * share);
        const double carbsDiff = qAbs(dish.getCarbs() - m_goalCarbs * share) / qMax(1.0, m_goalCarbs * share);
        const double score = missing * 0.30 + kcalDiff * 1.2 + proteinDiff * 0.35 + fatDiff * 0.20 + carbsDiff * 0.25;
        const double weight = 1.0 / (1.0 + score * score);
        weights.append(weight);
        totalWeight += weight;
    }

    double ticket = QRandomGenerator::global()->generateDouble() * totalWeight;
    for (int i = 0; i < candidates.size(); ++i) {
        ticket -= weights.at(i);
        if (ticket < 0.0)
            return candidates.at(i);
    }
    return candidates.last();
}
