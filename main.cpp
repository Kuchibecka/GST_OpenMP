#include <iostream>
#include <clocale>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <omp.h>

using namespace std;

// предельные параметры запуска: 192000 строк, 1000 столбцов, 1000 длина вектора
int main() {
    setlocale(LC_ALL, "RUSSIAN");
    int n, m, v;   // размерности матрицы и длина вектора
    FILE *in=fopen("in.txt", "rt"); // файл чтения
    FILE *out=fopen("out.txt", "wt"); // файл записи

    cout << endl << "____________________________________________________________" << endl;
    cout << " Заполните файл in.txt в соответствии со следующим форматом:" << endl;
    cout << " A1  A2 ...  Ai  Av"  << endl;

    cout << " B11 B12 ... B1j B1n" << endl;
    cout << " B21 B22 ... B2j B2n" << endl;
    cout << " ... ... ... ... ..." << endl;
    cout << " Bi1 Bi2 ... Bij Bin" << endl;
    cout << " Bm1 Bm2 ... Bmj Bmn" << endl;

    cout << "                    , где Bij - [iter][jiter]-ый элемент матрицы B размерности m x n," << endl;
    cout << "                      а Ai - iter-ый элемент вектора A длины v." << endl;
    cout << "                      Учтите, что для умножения матрицы на вектор должно выполнятся равенство: m=v (кол-во столбцов матрицы = длине вектора)" << endl;
    cout << "____________________________________________________________" << endl << endl;

    cout << " Затем введите количество строк матрицы: " << endl;
    cin >> n;
    cout << " Введите количество столбцов матрицы: " << endl;
    cin >> m;
    cout << " Введите длину вектора: " << endl;
    cin >> v;

    if(m != v) {
        cout << " Ошибка: длина вектора должна совпадать с кол-вом столбцов!" << endl;
        return -1;
    }

    // Выделение памяти под матрицу и вектор
    int **a = new int *[n];
    for (int i = 0; i < n; i++) {
        a[i] = new int[m];
    }
    int *vect = new int[v];

    // Считывание вектора из файла
    cout << " Считывание вектора из файла..." << endl;
    for (int i = 0; i < v; i++) {
        fscanf(in, "%d", &vect[i]);
    }

    // Вывод вектора на экран
    /*cout << " Ваш вектор: " << endl;
    for (int iter = 0; iter < v; iter++) {
        cout << "(" << vect[iter] << ")" << endl;
    }
    cout << endl << endl;*/

    // Считывание матрицы из файла
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            fscanf(in,"%d",&a[i][j]);
        }
    }

    // Вывод матрицы на экран
    /*cout << " Считанная матрица:" << endl;
    for (int iter = 0; iter < n; iter++) {
        cout << "(";
        for (int jiter = 0; jiter < m; jiter++) {
            cout << a[iter][jiter];
            if (jiter < m-1) {
                cout  << "\t";
            }
        }
        cout << ")" << endl;
    }
    cout << endl;*/

    // Перемножение вектора на матрицу
    int *result = new int[n];
    int threadsNum;
    int iter = 0;
    int jiter = 0;
    double wall_timer;
    cout << "Максимальное кол-во потоков: " << omp_get_max_threads() << endl;
    for (threadsNum = 1; threadsNum <= omp_get_max_threads(); ++threadsNum) {
        clock_t start_time =  clock(); // начальное время
        wall_timer = omp_get_wtime();
#pragma omp parallel for private(iter, jiter) default(none) shared(n, m, result, vect, a) num_threads(threadsNum)
        for (iter = 0; iter < n; iter++) {
            result[iter] = 0;
            for (jiter = 0; jiter < m; jiter++) {
                result[iter] += vect[jiter] * a[iter][jiter];
            }
        }
        double calculation_time = (double) (clock() - start_time) / CLOCKS_PER_SEC; // время перемножения
        cout << "________________________________________" << endl << "Время вычисления произведения: " << calculation_time << endl;
        std::ofstream log_out;
        log_out.open("log.txt", std::ios::app);
        log_out << "threads:\t" << threadsNum << ", m:\t" << m << ", n:\t" << n << ", v:\t" << v << ", time on wall:\t" << omp_get_wtime() - wall_timer << ", \telapsed time: \t" << calculation_time << endl;
        if (threadsNum == omp_get_max_threads())
            log_out << endl;
        log_out.close();
        cout << "Файл log.txt дополнен информацией о запуске" << endl;
    }

    // запись произведения в выходной файл
    for (int i = 0; i < n; i++) {
        fprintf(out,"%d ", result[i]);
    }
    cout << "Результат умножения записан в файл out.txt " << endl << "________________________________________" << endl;

    // Закрыть фд
    fclose(in);
    fclose(out);

    // Очистить память
    for (int i = 0; i < n; i++)
        delete[] a[i];
    delete[] a;
    delete[] vect;
    delete[] result;

    return 0;
}