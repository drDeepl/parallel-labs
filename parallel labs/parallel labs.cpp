// parallel labs.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>

using namespace std;

void greetWithCout(int num_threads) {
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; i++) {
        if (i % 2 == 0) {
            std::cout << "Hello World от потока # " << i << std::endl;
        }
    }
}

void greetWithPrint(int num_threads) {
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; i++) {
        std::cout << "Hello World от потока # " << i << std::endl;
    }
}

void task2() {
    
    int N;
    cout << "Введите число N" << endl;
    cin >> N;


    int total_sum = 0;
#pragma omp parallel reduction(+:total_sum)
    {
        
        int partial_sum = 0;
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Вычисляем часть чисел, которую следует обрабатывать каждому потоку
        int chunk_size = N / num_threads;
        int start = thread_id * chunk_size + 1;
        int end = (thread_id == num_threads - 1) ? N : start + chunk_size - 1;

        // Каждый поток вычисляет свою частичную сумму
        for (int i = start; i <= end; ++i)
        {
            partial_sum += i;
        }

        // Выводим частичную сумму, вычисленную в текущем потоке
        std::cout << "[" << thread_id << "]: Sum= " << partial_sum << std::endl;

        // Обновляем общую сумму
        total_sum += partial_sum;
    }

// Выводим общую сумму, которая является результатом сложения всех частичных сумм
std::cout << "Sum= " << total_sum << std::endl;
}


void tastk3(int SIZE) {
    
    auto startParallel = std::chrono::high_resolution_clock::now();
    
    auto startSequential = std::chrono::high_resolution_clock::now();

    int* parallelArray = new int[SIZE];



    int* sequentialArray = new int[SIZE];

    for (int i = 0; i < SIZE; ++i) {
        sequentialArray[i] = i;
    }



    auto endSequential = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationSequential = endSequential - startSequential;
    std::cout << "[Последовательно]: " << durationSequential.count() << " секунд" << std::endl;
    delete[] sequentialArray;


#pragma omp parallel for

    for (int i = 0; i < SIZE; ++i) {
        parallelArray[i] = i;
    }

    auto endParallel = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = endParallel - startParallel;
    std::cout << "[Параллельно]: " << durationParallel.count() << " секунд" << std::endl;

    // Проверка результатов
    bool isCorrect = true;
    for (int i = 0; i < SIZE; ++i) {
        if (sequentialArray[i] != parallelArray[i]) {
            isCorrect = false;
            std::cout << "Массивы различны на индексе " << i << std::endl;
            break;
        }
    }

    if (isCorrect) {
        std::cout << "Массивы заполнены корректно и идентично" << std::endl;
    }
    else {
        std::cout << "Массивы заполнены некорректно." << std::endl;
    }
    delete[] sequentialArray;
    delete[] parallelArray;

}

int main()
{
    setlocale(LC_ALL, "");
    

    //int num_threads;
    //std::cout << "Введит число потоков для запуска" << std::endl;
    //std::cin >> num_threads;

    //greetWithCout(num_threads);
    //greetWithPrint(num_threads);
    //task2();
    tastk3(10000000);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
