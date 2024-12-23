#ifndef PRIBIL_H
#define PRIBIL_H

#include <vector>
#include <string>
#include <stdexcept>

struct Item {
    std::string name;
    double weight; // Вес единицы товара
    double buyPrice; // Оптовая цена
    double sellPrice; // Цена перепродажи
    int availableQty; // Доступное количество

    // Валидация данных товара
    void validate() const {
        if (weight <= 0 || buyPrice <= 0 || sellPrice <= 0 || availableQty <= 0) {
            throw std::invalid_argument("Данные товара неверны: вес, цены и количество должны быть положительными.");
        }
        if (buyPrice > sellPrice) {
            throw std::logic_error("Оптовая цена не может быть выше цены перепродажи.");
        }
    }
};

class ProfitCalculator {
public:
    ProfitCalculator(double budget, double maxWeight);
    void addItem(const Item& item);
    std::vector<Item> maximizeProfit();

private:
    double budget; // Доступный бюджет
    double maxWeight; // Максимальная грузоподъёмность фуры
    std::vector<Item> items; // Список всех доступных товаров

    // Вспомогательная функция для проверки бюджета и веса
    void validateInputs() const;
};

#endif // PRIBIL_H