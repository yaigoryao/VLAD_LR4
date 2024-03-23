#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <iomanip>
#include <chrono>

template <class T>
class Matrix
{
    T** matrix = nullptr;
    int dim = 0;
    void clear()
    {
        if (this->matrix)
        {
            for (int i = 0; i < dim; i++)
            {
                delete[] this->matrix[i];
            }
            delete[] this->matrix;
            this->matrix = nullptr;
            this->dim = 0;
        }
    }

    void init()
    {
        if (!this->matrix)
        {
            this->matrix = new T* [this->dim];
            for (int i = 0; i < this->dim; i++)
            {
                this->matrix[i] = new T[this->dim];
            }
        }
    }

private:
public:
    Matrix() : dim{ 0 }, matrix{ nullptr } {};
    ~Matrix()
    {
        clear();
    }
    void insertion_sort()
    {
        T* temp = new T[this->dim];
        for (int i = 0; i < this->dim; i++)
        {
            temp[i] = this->matrix[i][i];
        }
        for (int i = 1; i < this->dim; i++)
        {
            int j = i;
            while (temp[j] < temp[j-1])
            {
                if (j == 0) break;
                std::swap(temp[j], temp[j-1]);
                j--;
            }
        }
        for (int i = 0; i < this->dim; i++)
        {
            this->matrix[i][i] = temp[i];
        }
        delete[] temp;
    }

    void print()
    {
        std::cout << this->dim << std::endl;
        for (int i = 0; i < this->dim; i++)
        {
            for (int j = 0; j < this->dim; j++)
            {
                std::cout << std::setiosflags(std::ios_base::left) << std::setw(4) << matrix[i][j];
            }
            std::cout << std::endl;
        }
    }

    void readMatrixFromFile(std::string path)
    {
        std::ifstream fin = std::ifstream(path);
        if (fin.is_open())
        {
            try
            {
                int rows = 0, cols = 0;
                fin >> rows >> cols;
                if (rows != cols)
                {
                    std::cout << "Матрица не квадратная!" << std::endl;
                    throw new std::runtime_error("Matrix not square");
                }
                this->dim = rows;
                init();
                for (int i = 0; i < this->dim; i++)
                {
                    for (int j = 0; j < this->dim; j++)
                    {
                        fin >> this->matrix[i][j];
                    }
                }
                std::cout << "Матрица успешно считана из файла" << std::endl;
                fin.close();
            }
            catch (...)
            {
                std::cout << "Ошибка парсинга!" << std::endl;
                fin.close();
            }
        }
    }

};


int main()
{
    setlocale(0, "");
    Matrix<int> m = Matrix<int>();
    m.readMatrixFromFile("matrix.txt");
    m.print();
    std::cout << "========================" << std::endl;

    auto begin = std::chrono::high_resolution_clock::now();
    m.insertion_sort();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = ((std::chrono::nanoseconds)(end - begin)).count();
    std::cout << "Время работы сортировки вставками (мс): " << elapsed_ms / std::pow(10, 6) << std::endl;
    m.print();
}
