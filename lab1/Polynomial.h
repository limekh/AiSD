#include <iostream>
#pragma once

template <typename T>
class Polynomial {
	size_t degree;
	T* coefficient;
public:
	//Константа для сравнения чисел
	static constexpr double EPS = 1e-9;

	//Конструктор с параметром: степень многочлена
	Polynomial<T>(size_t deg) : degree(deg) {
		coefficient = new T[degree + 1]();
		for (size_t i = 0; i < degree + 1; ++i) {
			coefficient[i] = 0;
		}
	}

	//Конструктор с параметром: массив коэффициентов
	//при соответствующих степенях
	Polynomial(size_t deg, const T* coef) : degree(deg) {
		coefficient = new T[degree + 1]();
		for (size_t i = 0; i <= degree; ++i) {
			coefficient[i] = coef[i];
		}
	}

	//Конструктор копирования
	Polynomial(const Polynomial& other) : degree(other.degree) {
		coefficient = new T[degree + 1]();
		for (size_t i = 0; i <= degree; ++i) {
			coefficient[i] = other.coefficient[i];
		}
	}

	//Деструктор
	~Polynomial() {
		delete[] coefficient;
	}

	T operator[](size_t idx) {
		if (idx > degree) return 0;
		return coefficient[idx];
	}

	//Оператор присваивания
	Polynomial operator=(const Polynomial& other) {
		if (this == other) {
			return *this;
		}
		
		delete[] coefficient;
		degree = (other.degree);
		coefficient = new T[other.degree + 1]();
		for (size_t i = 0; i <= degree; ++i) {
			coefficient[i] = other.coefficient[i];
		}
		
		return *this;
	}

	//Метод для установки коэффициента
	void set(size_t deg, T value) {
		if (deg > degree) {
			throw std::out_of_range("Index out of range");
		}
		coefficient[deg] = value;
	}

	//set для получения степени многочлена
	size_t get_degree() const {
		return degree;
	}

	//set для получения i-го коэффициента
	T get_coef(size_t idx) const {
		return coefficient[idx];
	}

	//Оператор сложения многочленов
	Polynomial operator+(const Polynomial& other) const {
		if (degree != other.degree) {
			throw std::invalid_argument("Polynomials must be of the same degree");
		}
		Polynomial result(degree);
		for (size_t i = 0; i <= degree; ++i) {
			result.coefficient[i] = coefficient[i] + other.coefficient[i];
		}
		return result;
	}

	//Оператор вычитания многочленов
	Polynomial operator-(const Polynomial& other) const {
		if (degree != other.degree) {
			throw std::invalid_argument("Polynomials must be of the same degree");
		}
		Polynomial result(degree);
		for (size_t i = 0; i <= degree; ++i) {
			result.coefficient[i] = coefficient[i] - other.coefficient[i];
		}
		return result;
	}

	//Оператор умножения многочлена на скаляр
	Polynomial operator*(const T& number) const {
		Polynomial result(degree);
		for (size_t i = 0; i <= degree; ++i) {
			result.coefficient[i] = coefficient[i] * number;
		}
		return result;
	}

	//Вычисление значения многочлена при указанном х
	T polynom_value(const T& x_value) {
		T result = 0;
		for (size_t i = 0; i <= degree; ++i) {
			result += coefficient[i] * std::pow(x_value, i);
		}
		return result;
	}

	//Уменьшение размера многочлена до минимально возможного
	//(при наличии ведущих нулей)
	void shrink_to_fit() {
		size_t new_degree = degree;
		while (new_degree > 0 && std::abs(coefficient[new_degree]) < EPS) {
			new_degree--;
		}
		if (new_degree < degree) {
			T* new_coefficient = new T[new_degree + 1]();
			for (size_t i = 0; i <= new_degree; ++i) {
				new_coefficient[i] = coefficient[i];
			}
			delete[] coefficient;
			coefficient = new_coefficient;
			degree = new_degree;
		}
	}

	//Резервирование памяти под более высокие степени с коэф 0
	void expand(size_t new_degree) {
		if (new_degree > degree) {
			T* new_coefficient = new T[new_degree + 1]();
			for (size_t i = 0; i <= degree; ++i) {
				new_coefficient[i] = coefficient[i];
			}
			delete[] coefficient;
			coefficient = new_coefficient;
			degree = new_degree;
		}
	}

	bool operator==(const Polynomial& other) {
		if (degree != other.degree) return false;
		for (size_t i = 0; i <= degree; ++i) {
			if (std::abs(coefficient[i] - other.coefficient[i]) >= EPS) return false;
		}
		return true;
	}

	bool operator!=(const Polynomial& other) {
		return !(*this == other);
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& poly) {
	for (size_t i = poly.get_degree(); i > 0; i--) {
		os << std::abs(poly.get_coef(i)) << "x^" << i << (poly.get_coef(i) >= 0 ? " + " : " - ");
	}
	os << std::abs(poly.get_coef(0)) << ".";

	return os;
}
