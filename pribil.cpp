#include "pribil.h"
#include <algorithm>
#include <iostream>

ProfitCalculator::ProfitCalculator(double budget, double maxWeight)
    : budget(budget), maxWeight(maxWeight) {
    if (budget <= 0 || maxWeight <= 0) {
        throw std::invalid_argument("Бюджет и грузоподъёмность должны быть положительными числами.");
    }
}

void ProfitCalculator::addItem(const Item& item) {
    item.validate();  // Проверка корректности данных товара
    items.push_back(item);
}

std::vector<Item> ProfitCalculator::maximizeProfit() {
    validateInputs();  // Проверка корректности входных данных

    // Таблица для динамического программирования
    int n = items.size();
    std::vector<std::vector<double>> dp(n + 1, std::vector<double>(static_cast<int>(maxWeight) + 1, 0));
    std::vector<std::vector<int>> itemCount(n + 1, std::vector<int>(static_cast<int>(maxWeight) + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= maxWeight; ++w) {
            const Item& item = items[i - 1];
            for (int qty = 1; qty <= item.availableQty; ++qty) {
                double cost = qty * item.buyPrice;
                if (w >= item.weight * qty && cost <= budget) {
                    double profit = qty * (item.sellPrice - item.buyPrice);
                    if (profit + dp[i - 1][w - static_cast<int>(item.weight * qty)] > dp[i - 1][w]) {
                        dp[i][w] = profit + dp[i - 1][w - static_cast<int>(item.weight * qty)];
                        itemCount[i][w] = qty;  // Запоминаем количество выбранных товаров
                    } else {
                        dp[i][w] = dp[i - 1][w];
                    }
                }
            }
        }
    }

    // Восстанавливаем выбранные товары
    std::vector<Item> selectedItems;
    double remainingWeight = maxWeight;
    double remainingBudget = budget;

    for (int i = n; i > 0; --i) {
        int qty = itemCount[i][static_cast<int>(remainingWeight)];
        if (qty > 0) {
            const Item& item = items[i - 1];
            selectedItems.push_back(item);
            remainingWeight -= item.weight * qty;
            remainingBudget -= item.buyPrice * qty;
        }
    }

    return selectedItems;
}

void ProfitCalculator::validateInputs() const {
    if (items.empty()) {
        throw std::runtime_error("Список товаров пуст. Добавьте хотя бы один товар.");
    }
    if (budget <= 0) {
        throw std::runtime_error("Бюджет должен быть положительным числом.");
    }
    if (maxWeight <= 0) {
        throw std::runtime_error("Грузоподъёмность должна быть положительным числом.");
    }
}
