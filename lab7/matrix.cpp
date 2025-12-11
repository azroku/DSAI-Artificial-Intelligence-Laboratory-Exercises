#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

class Matrix {
private:
    std::vector<std::vector<double>> mat;

public:
    Matrix(int height, int width) {
        mat.resize(height, std::vector<double>(width, 0.0));
    }

    double& getPixel(int i, int j) {
        return mat[i][j];
    }

    int getWidth() { return mat[0].size(); }
    int getHeight() { return mat.size(); }

    void print() {
        for (int i = 0; i < getHeight(); i++) {
            for (int j = 0; j < getWidth(); j++) {
                double val = mat[i][j];
                if (val < 0.2) std::cout << " ";
                else if (val < 0.4) std::cout << "-";
                else if (val < 0.6) std::cout << "x";
                else if (val < 0.8) std::cout << "O";
                else std::cout << "@";
            }
            std::cout << std::endl;
        }
    }

    double Convolution(Matrix &kernel, int i, int j) {
        double sum = 0.0;
        int kh = kernel.getHeight();
        int kw = kernel.getWidth();
        for (int m = 0; m < kh; m++) {
            for (int n = 0; n < kw; n++) {
                int x = i + m;
                int y = j + n;
                if (x >= 0 && x < getHeight() && y >= 0 && y < getWidth()) {
                    sum += mat[x][y] * kernel.getPixel(kh - 1 - m, kw - 1 - n);
                }
            }
        }
        return sum;
    }

    double Correlation(Matrix &kernel, int i, int j) {
        double sum = 0.0;
        int kh = kernel.getHeight();
        int kw = kernel.getWidth();
        for (int m = 0; m < kh; m++) {
            for (int n = 0; n < kw; n++) {
                int x = i + m;
                int y = j + n;
                if (x >= 0 && x < getHeight() && y >= 0 && y < getWidth()) {
                    sum += mat[x][y] * kernel.getPixel(m, n);
                }
            }
        }
        return sum;
    }
};

Matrix createBoxFilter(int k) {
    Matrix kernel(k, k);
    double val = 1.0 / (k * k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            kernel.getPixel(i, j) = val;
    return kernel;
}

Matrix applyConvolution(Matrix &input, Matrix &kernel) {
    int h = input.getHeight();
    int w = input.getWidth();
    int kh = kernel.getHeight();
    int kw = kernel.getWidth();
    Matrix result(h, w);

    for (int i = 0; i <= h - kh; i++) {
        for (int j = 0; j <= w - kw; j++) {
            result.getPixel(i + kh/2, j + kw/2) = input.Convolution(kernel, i, j);
        }
    }
    return result;
}

Matrix applyCorrelation(Matrix &input, Matrix &kernel) {
    int h = input.getHeight();
    int w = input.getWidth();
    int kh = kernel.getHeight();
    int kw = kernel.getWidth();
    Matrix result(h, w);

    for (int i = 0; i <= h - kh; i++) {
        for (int j = 0; j <= w - kw; j++) {
            result.getPixel(i + kh/2, j + kw/2) = input.Correlation(kernel, i, j);
        }
    }
    return result;
}

int main() {
    Matrix m(21, 21);
    m.getPixel(10, 10) = 1.0; 

    std::cout << "Original Matrix:\n";
    m.print();

    int kernelSizes[] = {3, 5, 7};
    for (int ks : kernelSizes) {
        Matrix kernel = createBoxFilter(ks);
        Matrix filtered = applyConvolution(m, kernel);
        std::cout << "\nFiltered Matrix with " << ks << "x" << ks << " Box Filter:\n";
        filtered.print();
    }

    Matrix img(21, 21);
    img.getPixel(5, 5) = 1.0; img.getPixel(5, 6) = 1.0; img.getPixel(6, 5) = 1.0;
    img.getPixel(15, 15) = 1.0; img.getPixel(15, 16) = 1.0; img.getPixel(16, 15) = 1.0;

    Matrix kernel(3, 3);
    kernel.getPixel(0, 0) = 0.5; kernel.getPixel(0, 1) = 0.5; kernel.getPixel(0, 2) = 0.5;
    kernel.getPixel(1, 0) = 0.5; kernel.getPixel(1, 1) = 0.0; kernel.getPixel(1, 2) = 0.5;
    kernel.getPixel(2, 0) = 0.5; kernel.getPixel(2, 1) = 0.5; kernel.getPixel(2, 2) = 0.5;

    Matrix result = applyCorrelation(img, kernel);

    std::cout << "\nPattern Detection Result:\n";
    result.print();

    img.getPixel(5, 5) = 0.9;
    result = applyCorrelation(img, kernel);

    std::cout << "\nPattern Detection After Slight Change:\n";
    result.print();

    return 0;
}