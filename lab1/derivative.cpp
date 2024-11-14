#include "Polynomial.h"

template <typename T>
Polynomial<T> derivative(const Polynomial<T>& poly) {
	Polynomial<T> result(poly.get_degree() - 1);
	for (size_t i = 0; i < poly.get_degree(); ++i) {
		result.set(i, poly.get_coef(i + 1) * (i + 1));
	}
	return result;
}