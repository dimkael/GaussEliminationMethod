#include <iostream>
#include <fstream>
#include <iomanip>

bool readFile(std::ifstream& file, double** matrix, double* b, int N) {
	while (!file.eof()) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				file >> matrix[i][j];
			}
			file >> b[i];
		}
	}

	return true;
}

void printMatrix(double** matrix, double* b, int N) {
	std::cout.setf(std::ios::fixed);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << std::setprecision(3) << matrix[i][j] << '\t';
		}
		std::cout << std::setprecision(3) << b[i] << '\n';
	}
	std::cout << '\n';
}

void printArray(double* arr, int N) {
	std::cout.setf(std::ios::fixed);
	for (int i = 0; i < N; i++) {
		std::cout << "x" << i + 1 << " = " << std::setprecision(3) << arr[i] << '\n';
	}
}

double* gaussEliminationMethod(double** matrix, double* b, int N) {
	double* x = new double[N];
	double max, t;
	int idx;

	// Цикл по строкам матрицы
	for (int k = 0; k < N; k++) {
		max = fabs(matrix[k][k]);
		idx = k;

		// Нахождение максимума в текущем столбце
		for (int j = k + 1; j < N; j++) {
			if (fabs(matrix[j][k]) > max) {
				max = fabs(matrix[j][k]);
				idx = j;
			}
		}
		
		// Перестановка строк таким образом, чтобы максимум в столбце стоял на главной диагонали
		for (int j = 0; j < N; j++) {
			std::swap(matrix[k][j], matrix[idx][j]);
		}
		std::swap(b[k], b[idx]);

		// Прямой ход
		for (int i = k; i < N; i++) {
			t = matrix[i][k];
			for (int j = k; j < N; j++) {
				matrix[i][j] /= t;
				if (i != k) matrix[i][j] -= matrix[k][j];
			}
			b[i] /= t;
			if (i != k) b[i] -= b[k];
		}
	}

	// Обратный ход
	x[N - 1] = b[N - 1];
	for (int i = N - 2; i >= 0; i--) {
		x[i] = b[i];
		for (int j = N - 1; j >= i + 1; j--) {
			x[i] -= matrix[i][j] * x[j];
		}
	}

	return x;
}

int main() {
	std::ifstream file("input.txt");
	if (!file) {
		std::cout << "File input.txt not found";
		return -1;
	}

	int N;
	file >> N;
	if (N < 1) {
		std::cout << "Error while reading input.txt. Invalid matrix dimension.";
		return -2;
	}

	double** matrix = new double* [N];
	for (int i = 0; i < N; i++) {
		matrix[i] = new double[N];
	}
	double* b = new double[N];
	double* x;

	readFile(file, matrix, b, N);

	std::cout << "Init matrix:\n";
	printMatrix(matrix, b, N);

	x = gaussEliminationMethod(matrix, b, N);

	std::cout << "Solve:\n";
	printArray(x, N);

	for (int i = 0; i < N; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	delete[] b;
	delete[] x;

	return 0;
}