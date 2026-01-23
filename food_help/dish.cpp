#include "dish.h"

Dish::Dish() {}

Dish::Dish(int id, const QString &name, MealType type, const QString &recipe,
           double calories, double protein, double fat, double carbs,
           const QStringList &ingredients, const QString &imagePath)
    : m_id(id), m_name(name), m_type(type), m_recipe(recipe),
    m_calories(calories), m_protein(protein), m_fat(fat), m_carbs(carbs),
    m_ingredients(ingredients), m_imagePath(imagePath)
{
}

int Dish::getId() const { return m_id; }
QString Dish::getName() const { return m_name; }
Dish::MealType Dish::getType() const { return m_type; }
QString Dish::getRecipe() const { return m_recipe; }
double Dish::getCalories() const { return m_calories; }
double Dish::getProtein() const { return m_protein; }
double Dish::getFat() const { return m_fat; }
double Dish::getCarbs() const { return m_carbs; }
QStringList Dish::getIngredients() const { return m_ingredients; }
QString Dish::getImagePath() const { return m_imagePath; }

QString Dish::getTypeString() const {
    switch (m_type) {
    case BREAKFAST: return "Завтрак";
    case LUNCH:     return "Обед";
    case DINNER:    return "Ужин";
    case SNACK:     return "Перекус";
    default:        return "Неизвестно";
    }
}
