#include <iostream>
#include <clocale>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <omp.h>

using namespace std;

// ���������� ��������� �������: 192000 �����, 1000 ��������, 1000 ����� �������
int main() {
    setlocale(LC_ALL, "RUSSIAN");
    int n, m, v;   // ����������� ������� � ����� �������
    FILE *in=fopen("in.txt", "rt"); // ���� ������
    FILE *out=fopen("out.txt", "wt"); // ���� ������

    cout << endl << "____________________________________________________________" << endl;
    cout << " ��������� ���� in.txt � ������������ �� ��������� ��������:" << endl;
    cout << " A1  A2 ...  Ai  Av"  << endl;

    cout << " B11 B12 ... B1j B1n" << endl;
    cout << " B21 B22 ... B2j B2n" << endl;
    cout << " ... ... ... ... ..." << endl;
    cout << " Bi1 Bi2 ... Bij Bin" << endl;
    cout << " Bm1 Bm2 ... Bmj Bmn" << endl;

    cout << "                    , ��� Bij - [iter][jiter]-�� ������� ������� B ����������� m x n," << endl;
    cout << "                      � Ai - iter-�� ������� ������� A ����� v." << endl;
    cout << "                      ������, ��� ��� ��������� ������� �� ������ ������ ���������� ���������: m=v (���-�� �������� ������� = ����� �������)" << endl;
    cout << "____________________________________________________________" << endl << endl;

    cout << " ����� ������� ���������� ����� �������: " << endl;
    cin >> n;
    cout << " ������� ���������� �������� �������: " << endl;
    cin >> m;
    cout << " ������� ����� �������: " << endl;
    cin >> v;

    if(m != v) {
        cout << " ������: ����� ������� ������ ��������� � ���-��� ��������!" << endl;
        return -1;
    }

    // ��������� ������ ��� ������� � ������
    int **a = new int *[n];
    for (int i = 0; i < n; i++) {
        a[i] = new int[m];
    }
    int *vect = new int[v];

    // ���������� ������� �� �����
    cout << " ���������� ������� �� �����..." << endl;
    for (int i = 0; i < v; i++) {
        fscanf(in, "%d", &vect[i]);
    }

    // ����� ������� �� �����
    /*cout << " ��� ������: " << endl;
    for (int iter = 0; iter < v; iter++) {
        cout << "(" << vect[iter] << ")" << endl;
    }
    cout << endl << endl;*/

    // ���������� ������� �� �����
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            fscanf(in,"%d",&a[i][j]);
        }
    }

    // ����� ������� �� �����
    /*cout << " ��������� �������:" << endl;
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

    // ������������ ������� �� �������
    int *result = new int[n];
    int threadsNum;
    int iter = 0;
    int jiter = 0;
    double wall_timer;
    cout << "������������ ���-�� �������: " << omp_get_max_threads() << endl;
    for (threadsNum = 1; threadsNum <= omp_get_max_threads(); ++threadsNum) {
        clock_t start_time =  clock(); // ��������� �����
        wall_timer = omp_get_wtime();
#pragma omp parallel for private(iter, jiter) default(none) shared(n, m, result, vect, a) num_threads(threadsNum)
        for (iter = 0; iter < n; iter++) {
            result[iter] = 0;
            for (jiter = 0; jiter < m; jiter++) {
                result[iter] += vect[jiter] * a[iter][jiter];
            }
        }
        double calculation_time = (double) (clock() - start_time) / CLOCKS_PER_SEC; // ����� ������������
        cout << "________________________________________" << endl << "����� ���������� ������������: " << calculation_time << endl;
        std::ofstream log_out;
        log_out.open("log.txt", std::ios::app);
        log_out << "threads:\t" << threadsNum << ", m:\t" << m << ", n:\t" << n << ", v:\t" << v << ", time on wall:\t" << omp_get_wtime() - wall_timer << ", \telapsed time: \t" << calculation_time << endl;
        if (threadsNum == omp_get_max_threads())
            log_out << endl;
        log_out.close();
        cout << "���� log.txt �������� ����������� � �������" << endl;
    }

    // ������ ������������ � �������� ����
    for (int i = 0; i < n; i++) {
        fprintf(out,"%d ", result[i]);
    }
    cout << "��������� ��������� ������� � ���� out.txt " << endl << "________________________________________" << endl;

    // ������� ��
    fclose(in);
    fclose(out);

    // �������� ������
    for (int i = 0; i < n; i++)
        delete[] a[i];
    delete[] a;
    delete[] vect;
    delete[] result;

    return 0;
}