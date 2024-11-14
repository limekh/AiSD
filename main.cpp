#include "Polynomial.h"
#include "derivative.cpp"
#include <iostream>

int main() {
	try {
		int coefficient1[] = { 1, 2, 3 };  // 3x^2 + 2x + 1
		int coefficient2[] = { 3, 2, 1 };  // x^2 + 2x + 3
		int coefficient3[] = { 4, 3, 2, 1 }; // x^3 + 2x^2 + 3x^1 + 4

		Polynomial<int> p1(2, coefficient1);
		Polynomial<int> p2(2, coefficient2);
		Polynomial<int> p3(3, coefficient3);

		std::cout << "p1: " << p1 << "\n";
		std::cout << "p2: " << p2 << "\n";
		std::cout << "p3: " << p3 << "\n";

		auto p_sum = p1 + p2; // сложение
		std::cout << "p_sum = p1 + p2: " << p_sum << "\n";

		auto p_dif = p1 - p2; // вычитание
		std::cout << "p_dif = p1 - p2: " << p_dif << "\n";

		auto p_mult = p1 * 5; // умножение на скаляр
		std::cout << "p_mult = p1 * 5: " << p_mult << "\n";

		auto p_eq = p2;
		std::cout << "p_eq = p2: " << p_eq << "\n";

		int value = p3.polynom_value(5); // вычисление значения при х = 5
		std::cout << "p3(5) = " << value << "\n";

		p1.expand(5); // расширение
		std::cout << "p1 after expand: " << p1 << "\n";

		p1.shrink_to_fit();  // уменьшение размера
		std::cout << "p1 after shrink_to_fit: " << p1 << "\n";

		std::cout << "derivative of p3: " << derivative<int>(p3) << "\n"; // производная многочлена р3
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
	}

	return 0;
}