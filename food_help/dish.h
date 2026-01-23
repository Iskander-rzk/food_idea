#ifndef DISH_H
#define DISH_H

#include <QString>
#include <QStringList>

class Dish
{
public:
    enum MealType { BREAKFAST, LUNCH, DINNER, SNACK, MEAL_TYPE_COUNT };

    Dish();
    Dish(int id, const QString &name, MealType type, const QString &recipe,
         double calories, double protein, double fat, double carbs,
         const QStringList &ingredients = {}, const QString &imagePath = "");

    int getId() const;
    QString getName() const;
    MealType getType() const;
    QString getRecipe() const;
    double getCalories() const;
    double getProtein() const;
    double getFat() const;
    double getCarbs() const;
    QStringList getIngredients() const;
    QString getImagePath() const;

    QString getTypeString() const;

private:
    int m_id = -1;
    QString m_name;
    MealType m_type;
    QString m_recipe;
    double m_calories = 0;
    double m_protein = 0;
    double m_fat = 0;
    double m_carbs = 0;
    QStringList m_ingredients;
    QString m_imagePath;
};

#endif // DISH_H
