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


void task3_fill_array(int SIZE) {
    
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



// Последовательное вычисление суммы
int sumSequential(const std::vector<int>& vec) {
    auto start = std::chrono::high_resolution_clock::now();

    int sum = 0;
    for (int num : vec) {
        sum += num;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = end - start;
    std::cout << "[Вычисление суммы элементов вектора последовательно]: " << durationParallel.count() << " секунд" << std::endl;
    return sum;
}

// Параллельное вычисление суммы с редукцией
int sumParallelReduction(const std::vector<int>& vec) {
    auto start = std::chrono::high_resolution_clock::now();

    int sum = 0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < vec.size(); ++i) {
        sum += vec[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = end - start;
    std::cout << "[Вычисление суммы элементов вектора параллельно с редукцией]: " << durationParallel.count() << " секунд" << std::endl;
    return sum;
}

// Параллельное вычисление суммы с критической секцией
int sumParallelCritical(const std::vector<int>& vec) {
    auto start = std::chrono::high_resolution_clock::now();
    int sum = 0;
#pragma omp parallel for
    for (int i = 0; i < vec.size(); ++i) {
#pragma omp critical
        {
            sum += vec[i];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = end - start;
    std::cout << "[Вычисление суммы элементов вектора параллельно с секцией]: " << durationParallel.count() << " секунд" << std::endl;
    return sum;
}


void task3_summ_els(const std::vector<int>& vec) {
    sumSequential(vec);
    sumParallelReduction(vec);
    sumParallelCritical(vec);
}


// Последовательное сложение
void add_vectors_sequential(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = end - start;
    std::cout << "[Сложение последовательное]: " << durationParallel.count() << " секунд" << std::endl;

}

// Параллельное сложение (FOR)
void add_vectors_parallel_for(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result) {
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = end - start;
    std::cout << "[Сложение параллельное FOR]: " << durationParallel.count() << " секунд" << std::endl;
}

// Параллельное сложение (Sections)
void add_vectors_parallel_sections(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result) {
    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int i = 0; i < a.size() / 2; ++i) {
                result[i] = a[i] + b[i];
            }
        }

#pragma omp section
        {
            for (int i = a.size() / 2; i < a.size(); ++i) {
                result[i] = a[i] + b[i];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationParallel = end - start;
    std::cout << "[Сложение параллельное SECTION]: " << durationParallel.count() << " секунд" << std::endl;
}

void lab_3_add_vectors(int VECTOR_SIZE) {
    
    std::vector<int> a(VECTOR_SIZE);
    std::vector<int> b(VECTOR_SIZE);
    std::vector<int> result(VECTOR_SIZE);

    add_vectors_sequential(a, b, result);
    add_vectors_parallel_for(a, b, result);
    add_vectors_parallel_sections(a, b, result);
    task3_summ_els(result);



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
    //task3_fill_array(10000000);
    lab_3_add_vectors(1000000);
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
