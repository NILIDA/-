#include <iostream>
using namespace std;

/**
 * Функция для выделения памяти под двумерный массив и заполнения его значениями
 * в порядке возрастания от 1 до N*N.
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void fillArray(int** array, int size)
{
    int value = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array[i][j] = value;
            value++;
        }
    }
}

/**
 * Функция для вывода двумерного массива на экран.
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void printArray(int** array, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << array[i][j] << "\t";
        }
        cout << endl;
    }
}

/**
 * Функция для восстановления прямого порядка элементов в массиве.
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void restoreOrder(int** array, int size)
{
    fillArray(array, size);
}

/**
 * Функция для изменения порядка элементов в массиве на обратный.
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void reverseOrder(int** array, int size)
{
    int value = size * size;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array[i][j] = value;
            value--;
        }
    }
}

/**
 * Функция для выкладывания элементов вдоль главной диагонали.
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void glavDiagonal(int** array, int size)
{
    int value = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            array[i - j][j] = value;
            value++;
        }
    }
    for (int i = 1; i < size; i++)
    {
        for (int j = i; j < size; j++)
        {
            array[size - j + i - 1][j] = value;
            value++;
        }
    }
}

/**
 * Функция для выкладывания элементов вдоль побочной диагонали.
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void pobochDiagonal(int** array, int size)
{
    int value = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = size - 1, k = 0; j >= 0 && k <= i; j--, k++)
        {
            array[i - k][j] = value;
            value++;
        }
    }
    for (int i = size - 1; i >= 0; i--)
    {
        for (int j = size - 1, k = 1; j >= 0 && i - k >= 0; j--, k++)
        {
            array[j][i - k] = value;
            value++;
        }
    }


}


/**
 * Функция для выкладывания элементов по часовой стрелке спиралью от элемента [0,0].
 * @param array Указатель на двумерный массив.
 * @param size Размер массива.
 */
void spiralOrder(int** array, int size)
{
    int value = 1;
    int left = 0, right = size - 1;
    int top = 0, bottom = size - 1;

    while (left <= right && top <= bottom)
    {
        // Верхняя сторона
        for (int col = left; col <= right; col++)
        {
            array[top][col] = value;
            value++;
        }
        top++;

        // Правая сторона
        for (int row = top; row <= bottom; row++)
        {
            array[row][right] = value;
            value++;
        }
        right--;

        // Нижняя сторона
        if (top <= bottom)
        {
            for (int col = right; col >= left; col--)
            {
                array[bottom][col] = value;
                value++;
            }
            bottom--;
        }

        // Левая сторона
        if (left <= right)
        {
            for (int row = bottom; row >= top; row--)
            {
                array[row][left] = value;
                value++;
            }
            left++;
        }
    }
}

int main()
{
    int N;
    cout << "Введите значение N: ";
    cin >> N;

    // Создание двумерного массива и выделение памяти
    int** array = new int* [N];
    for (int i = 0; i < N; i++)
    {
        array[i] = new int[N];
    }

    fillArray(array, N);  // Заполнение массива значениями

    // Вывод исходного массива
    cout << "Исходный массив:" << endl;
    printArray(array, N);

    cout << "Выберите операцию: " << std::endl;
    cout << "1. Восстановить прямой порядок элементов." << endl;
    cout << "2. Сделать обратный порядок элементов." << endl;
    cout << "3. Выложить элементы вдоль главной диагонали." << endl;
    cout << "4. Выложить элементы вдоль побочной диагонали." << endl;
    cout << "5. Закрутить элементы по часовой стрелке спиралью." << endl;
    cout << "6. Закончить" << endl;

    // Возможность манипуляций с порядком элементов
    int choice = 0;
    while (choice != 6) {
        cout << "Выберете операцию: ";
        cin >> choice;
        using namespace std;
        switch (choice)
        {
        case 1:
            restoreOrder(array, N);
            cout << "Прямой порядок элементов восстановлен." << endl;
            break;
        case 2:
            reverseOrder(array, N);
            cout << "Обратный порядок элементов создан." << endl;
            break;
        case 3:
            glavDiagonal(array, N);
            cout << "Элементы выложены вдоль главной диагонали." << endl;
            break;
        case 4:
            pobochDiagonal(array, N);
            cout << "Элементы выложены вдоль побочной диагонали." << endl;
            break;
        case 5:
            spiralOrder(array, N);
            cout << "Элементы выложены по часовой стрелке спиралью." << endl;
            break;
        case 6:
            cout << "Конец" << endl;
            break;
        default:
            cout << "Некорректный выбор операции." << endl;
            break;

        }
        cout << "Измененный массив:" << endl;
        printArray(array, N);
    }


    // Освобождение памяти, выделенной для массива
    for (int i = 0; i < N; i++)
    {
        delete[] array[i];
    }
    delete[] array;

    return 0;
}