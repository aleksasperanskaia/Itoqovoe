#include <iostream>
#include <limits>
#include "pribil.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cout << "Ошибка: введите положительное число.\n";
            clearInput();
        } else {
            return value;
        }
    }
}

int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cout << "Ошибка: введите положительное целое число.\n";
            clearInput();
        } else {
            return value;
        }
    }
}

int main() {
    try {
        double budget = getValidDouble("Введите доступный бюджет: ");
        double maxWeight = getValidDouble("Введите максимальную грузоподъёмность фуры: ");
        int numItems = getValidInt("Введите количество доступных товаров: ");

        ProfitCalculator calculator(budget, maxWeight);

        for (int i = 0; i < numItems; ++i) {
            Item item;
            std::cout << "\nТовар #" << i + 1 << "\n";
            std::cout << "Введите имя товара: ";
            std::cin >> item.name;

            item.weight = getValidDouble("Введите вес товара: ");
            item.buyPrice = getValidDouble("Введите оптовую цену товара: ");
            item.sellPrice = getValidDouble("Введите цену перепродажи товара: ");
            item.availableQty = getValidInt("Введите доступное количество товара: ");

            calculator.addItem(item);
        }

        auto selectedItems = calculator.maximizeProfit();

        std::cout << "\nВыбранные товары:\n";
        for (const auto& item : selectedItems) {
            std::cout << "Товар: " << item.name
                      << ", Количество: " << item.availableQty
                      << ", Прибыль: " << (item.sellPrice - item.buyPrice) * item.availableQty << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}

