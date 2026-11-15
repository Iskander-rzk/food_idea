#include "foodcontroller.h"

#include <QSet>

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
        if (missingIngredients(dish).size() <= 2)
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

void FoodController::generateDay()
{
    QSet<int> forbidden;
    for (int id : m_previousIds.values())
        forbidden.insert(id);

    m_today[Dish::BREAKFAST] = m_db.getBestDishForType(Dish::BREAKFAST, m_products, forbidden);
    m_today[Dish::LUNCH]     = m_db.getBestDishForType(Dish::LUNCH, m_products, forbidden);
    m_today[Dish::DINNER]    = m_db.getBestDishForType(Dish::DINNER, m_products, forbidden);
    m_today[Dish::SNACK]     = m_db.getBestDishForType(Dish::SNACK, m_products, forbidden);

    m_previousIds.clear();
    for (auto it = m_today.cbegin(); it != m_today.cend(); ++it) {
        if (it.value().getId() != -1)
            m_previousIds[it.key()] = it.value().getId();
    }

    m_db.savePreviousDishes(m_previousIds);
    emit dataChanged();
}

void FoodController::replaceMeal(int mealType)
{
    if (mealType < 0 || mealType >= Dish::MEAL_TYPE_COUNT)
        return;

    const auto type = static_cast<Dish::MealType>(mealType);
    QSet<int> forbidden;
    if (m_today.contains(type) && m_today[type].getId() != -1)
        forbidden.insert(m_today[type].getId());

    const Dish newDish = m_db.getBestDishForType(type, m_products, forbidden, 3);
    if (newDish.getId() == -1) {
        emit messageChanged("Не удалось найти другое подходящее блюдо");
        return;
    }

    m_today[type] = newDish;
    m_previousIds[type] = newDish.getId();
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

    m_today[dish.getType()] = dish;
    m_previousIds[dish.getType()] = dish.getId();
    m_db.savePreviousDishes(m_previousIds);
    emit dataChanged();
    emit messageChanged(QString("«%1» добавлено в меню на день").arg(dish.getName()));
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
