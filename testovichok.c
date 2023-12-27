#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#define MAX_SIZE 9
#define TARGET_SUM 100

int possibleValues[10][19] =
{
    {0, 10, 20, 30, 40, 50, 60, 70, 80, 90},  // Для 0
    {1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 31, 41, 51, 61, 71, 81, 91},  // Для 1
    {2, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 12, 32, 42, 52, 62, 72, 82, 92},  // Для 2
    {3, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 13, 23, 43, 53, 63, 73, 83, 93},  // Для 3
    {4, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 14, 24, 34, 54, 64, 74, 84, 94},  // Для 4
    {5, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 15, 25, 35, 45, 65, 75, 85, 95},  // Для 5
    {6, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 16, 26, 36, 46, 56, 76, 86, 96},  // Для 6
    {7, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 17, 27, 37, 47, 57, 67, 87, 97},  // Для 7
    {8, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 18, 28, 38, 48, 58, 68, 78, 98},  // Для 8
    {9, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 19, 29, 39, 49, 59, 69, 79, 89}   // Для 9
};
// Функция для вывода матрицы
void printMatrix(int** matrix, int rows, int cols)
{
    for (int i = 0; i < cols * 4 - 1; i++)
    {
        printf("_");
    }
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        printf("|");
        for (int j = 0; j < cols; j++)
        {
            printf("%2d|", matrix[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < cols * 4 - 1; i++)
    {
        printf("-");
    }
     
    printf("\n");
}

// Рекурсивная функция для нахождения решения
bool findSolution(int** matrix, int rows, int cols, int x, int y, int* rowSums, int* colSums)
{
    if (x == rows) 
    {
        for (int i = 0; i < rows; i++)
        {
            if (rowSums[i] != TARGET_SUM) return false;
        }
        for (int j = 0; j < cols; j++)
        {
            if (colSums[j] != TARGET_SUM) return false;
        }
        return true;
    }
    int nextX = (y + 1 < cols) ? x : x + 1;
    int nextY = (y + 1 < cols) ? y + 1 : 0;

    int originalValue = matrix[x][y];
    int* values = possibleValues[originalValue];

    for (int i = 0; i < 19; i++)
    {
        int newValue = values[i];
        int add = newValue - originalValue;

        if (rowSums[x] + add <= TARGET_SUM && colSums[y] + add <= TARGET_SUM)
        {
            matrix[x][y] = newValue;
            rowSums[x] += add;
            colSums[y] += add;

            if (findSolution(matrix, rows, cols, nextX, nextY, rowSums, colSums))
            {
                return true;
            }

            // Откат изменений (backtracking)
            matrix[x][y] = originalValue;
            rowSums[x] -= add;
            colSums[y] -= add;
        }
    }
    return false;
}
int main()
{
    setlocale(LC_ALL, "RUS");
    system("color F0");
    printf("Вас приветствует игра головоломка Сто («Hundred»)!\nДля выбора необходимого пункта в главном меню игры, введите число от 1-го до 4-х.\n");
    int rows, cols, answer, file_answer, f_rows, f_cols, score, row_sum_counter, col_sum_counter, lives, c;
    int** matrix;
    int rowSums[MAX_SIZE] = { 0 };
    int colSums[MAX_SIZE] = { 0 };
    int user_matrix[MAX_SIZE][MAX_SIZE];
    bool win = false;
    FILE* file;
    while (1)
    {
        printf("Главное меню игры:\n1. Показать правила игры.\n2. Новая игра.\n3. Головоломка для ИИ.\n4. Выход из игры.\nВаш ответ: ");
        scanf("%d", &answer);
        if (answer == 1)
        {
            printf("Сто («Hundred») состоит из квадратной сетки в виде кроссворда/головоломки, в каждой клетке которой проставлены цифры. Требуется поставить дополнительные цифры таким образом (слева, или справа цыфры в чейке), чтобы сумма всех чисел в каждом ряду и в каждом столбце была равна 100.\n");
        }
        else if (answer == 2 || answer == 3)
        {
            row_sum_counter = 0;
            col_sum_counter = 0;
            lives = 3;
            win = false;
            printf("Хотите восстановить предыдущее игровое поле?\n1. Да.\n2. Нет.\n");
            scanf("%d", &file_answer);
            file = fopen("temp.txt", "r");
            int rows = 0, cols = 0, ch;
            if (answer == 2)
            {
                if (file_answer == 1)
                {
                   
                    int matrix[10][10];
                    for (int i = 0; i < 10; i++)
                    {
                        for (int k = 0; k < 10; k++)
                        {
                            matrix[i][k] = 0;
                        }
                    }
                    if (file == NULL)
                    {
                        printf("Не удалось открыть файл.\n");
                        return 1;
                    }

                    char sym[5] = "";
                    char s = '\0';

                    int r = 0;
                    int l = 0;


                    while (s != EOF)
                    {
                        s = fgetc(file);

                        if (s == ' ' || s == '\n' || s == EOF)
                        {
                            int len = strlen(sym);
                            matrix[r][l] = atoi(sym);

                            for (int x = 0; x < len + 1; x++)
                                sym[x] = '\0';

                            if (s == '\n')
                            {
                                l = 0;
                                r++;
                            }
                            else if (s == EOF)
                                break;
                            else
                                l++;
                        }
                        else
                        {
                            sym[strlen(sym)] = s;
                        }

                    }

                    int len_r = 0; //Макс длина строки
                    int len_c = 0; //Макс дли

                    for (int i = 0; i < 10; i++)
                    {
                        int mt = matrix[i][0];
                        int mc = matrix[0][i];

                        if (mt != 0)
                            len_r++;

                        if (mc != 0)
                            len_c++;

                    }
                    printf("Начальное игровое поле:\n");
                    for (int i = 0; i < len_r; i++)
                    {
                        for (int k = 0; k < len_c; k++)
                        {
                            printf("%d ", matrix[i][k]);
                        }
                        printf("\n");
                    }
                    fclose(file);
                    row_sum_counter = 0;
                    col_sum_counter = 0;
                    lives = 3;
                    win = false;

                    while (lives != 0 || win == false)
                    {
                        printf("Ваше решение: \n");
                        printf("Введите элементы головоломки построчно через пробел:\n");
                        for (int i = 0; i < rows; i++)
                        {
                            for (int j = 0; j < cols; j++)
                            {
                               
                                scanf("%d", &user_matrix[i][j]);
                                
                            }
                        }
                        for (int i = 0; i < rows; i++)
                        {
                            int sum = 0;
                            for (int j = 0; j < cols; j++)
                            {
                                sum += user_matrix[i][j];
                            }
                            if (sum == 100)
                            {
                                row_sum_counter++;
                            }
                            else
                            {
                                printf("Ошибка: сумма элементов строки %d не равна 100.\n", i + 1);
                                return 1;
                            }
                        }
                        // Проверка сумм столбцов
                        for (int j = 0; j < cols; j++)
                        {
                            int sum = 0;
                            for (int i = 0; i < rows; i++)
                            {
                                sum += user_matrix[i][j];
                            }
                            if (sum == 100)
                            {
                                col_sum_counter++;
                            }
                            else
                            {
                                printf("Ошибка: сумма элементов столбца %d не равна 100.\n", j + 1);
                                return 1;
                            }
                        }
                        if (rows == row_sum_counter && cols == col_sum_counter)
                        {
                            printf("Браво! Поздравляем с победой! :)\n");
                            score = rows * 13 + cols * 13 + lives * 100 + 1000;
                            printf("Количество заработанных очков за решение головоломки: %d\n", score);
                            win = true;
                            break;
                        }
                        else
                        {
                            lives--;
                            printf("Неверно! Попробуйте еще раз.\nОставшееся количество жизней: %d\n", lives);
                            if (lives == 0)
                            {
                                printf("Игра окончена! Повезет в следующий раз. :(");
                                break;
                            }
                        }
                    }
                }
                else if (file_answer == 2)
                {
                    printf("Введите количество строк (от 1 до %d): ", MAX_SIZE);
                    scanf("%d", &rows);
                    if (rows < 1 || rows > MAX_SIZE)
                    {
                        printf("Неверное количество строк.\n");
                        continue;

                    }

                    printf("Введите количество столбцов (от 1 до %d): ", MAX_SIZE);
                    scanf("%d", &cols);
                    if (cols < 1 || cols > MAX_SIZE)
                    {
                        printf("Неверное количество столбцов.\n");
                        continue;
                    }

                    matrix = (int**)malloc(rows * sizeof(int*));
                    for (int i = 0; i < rows; i++)
                    {
                        matrix[i] = (int*)malloc(cols * sizeof(int));
                        for (int j = 0; j < cols; j++)
                        {
                            printf("Введите элемент [%d][%d]: ", i + 1, j + 1);
                            scanf("%d", &matrix[i][j]);
                            rowSums[i] += matrix[i][j];
                            colSums[j] += matrix[i][j];
                        }
                    }
                    printf("Начальное игровое поле:\n");
                    // Выделение памяти для матрицы
                    // ... (печать матрицы и поиск решения)
                    printMatrix(matrix, rows, cols);
                    while (lives != 0 || win == false)
                    {
                        printf("Ваше решение: \n");
                        printf("Введите элементы головоломки построчно через пробел:\n");
                        for (int i = 0; i < rows; i++)
                        {
                            for (int j = 0; j < cols; j++)
                            {
                                scanf("%d", &user_matrix[i][j]);
                            }
                        }
                        for (int i = 0; i < rows; i++)
                        {
                            int sum = 0;
                            for (int j = 0; j < cols; j++)
                            {
                                sum += user_matrix[i][j];
                            }
                            if (sum == 100)
                            {
                                row_sum_counter++;
                            }
                            else
                            {
                                printf("Ошибка: сумма элементов строки %d не равна 100.\n", i + 1);
                            }
                        }
                        // Проверка сумм столбцов
                        for (int j = 0; j < cols; j++)
                        {
                            int sum = 0;
                            for (int i = 0; i < rows; i++)
                            {
                                sum += user_matrix[i][j];
                            }
                            if (sum == 100)
                            {
                                col_sum_counter++;
                            }
                            else
                            {
                                printf("Ошибка: сумма элементов столбца %d не равна 100.\n", j + 1);
                            }
                        }
                        if (rows == row_sum_counter && cols == col_sum_counter)
                        {
                            printf("Браво! Поздравляем с победой! :)\n");
                            score = rows * 13 + cols * 13 + lives * 100 + 1000;
                            printf("Количество заработанных очков за решение головоломки: %d\n", score);
                            win = true;
                            break;
                        }
                        else
                        {
                            lives -= 1;
                            printf("Неверно! Попробуйте еще раз.\nОставшееся количество жизней: %d\n", lives);
                            if (lives == 0)
                            {
                                printf("Игра окончена! Повезет в следующий раз. :(\n");
                                break;
                            }
                        }
                    }
                    file = fopen("temp.txt", "w");
                    if (file == NULL)
                    {
                        printf("Ошибка при открытии файла.\n");
                    }
                    else
                    {
                        for (int i = 0; i < rows; i++)
                        {
                            for (int j = 0; j < cols; j++)
                            {
                                fprintf(file, "%d ", matrix[i][j]);
                            }
                            fprintf(file, "\n");
                        }

                        // ... (запись в файл)
                        fclose(file);
                    }
                    for (int i = 0; i < rows; i++)
                    {
                        free(matrix[i]);
                    }
                    free(matrix);
                }
                else
                {
                   printf("Для выбора пункта меню, вам нужно ввести число от 1-го до 2-x.\nПожалуйста, попробуйте ещё раз.\n");
                }
            }
            
            else if (answer == 3)
            {
                if (file_answer == 1)
                {
                    int len_r = 0; // Максимальная длина строки
                    int len_c = 0;
                    char s = '\0';
                    char sym[5] = "";
                    int r = 0;
                    int l = 0;
                    while (s != EOF)
                    {
                        s = fgetc(file);

                        if (s == ' ' || s == '\n' || s == EOF)
                        {
                            int len = strlen(sym);
                            if (len > 0)
                            {
                                if (l == 0)
                                    len_r++;
                                if (r == 0)
                                    len_c++;
                            }

                            for (int x = 0; x < len + 1; x++)
                                sym[x] = '\0';

                            if (s == '\n')
                            {
                                l = 0;
                                r++;
                            }
                            else if (s == EOF)
                                break;
                            else
                                l++;
                        }
                        else
                        {
                            sym[strlen(sym)] = s;
                        }
                    }

                    // Создаем массив с нужными размерами
                    int** matrix = (int**)malloc(len_r * sizeof(int*));
                    for (int i = 0; i < len_r; i++)
                    {
                        matrix[i] = (int*)malloc(len_c * sizeof(int));
                        for (int k = 0; k < len_c; k++)
                        {
                            matrix[i][k] = 0;
                        }
                    }

                    // Считываем значения из файла в массив
                    rewind(file);
                    s = '\0';
                    r = 0;
                    l = 0;
                    while (s != EOF)
                    {
                        s = fgetc(file);

                        if (s == ' ' || s == '\n' || s == EOF)
                        {
                            int len = strlen(sym);
                            if (len > 0)
                            {
                                matrix[r][l] = atoi(sym);
                                if (s == '\n')
                                {
                                    l = 0;
                                    r++;
                                }
                                else if (s == EOF)
                                    break;
                                else
                                    l++;
                            }

                            for (int x = 0; x < len + 1; x++)
                                sym[x] = '\0';
                        }
                        else
                        {
                            sym[strlen(sym)] = s;
                        }
                    }

                    // Проверяем, что файл не пустой
                    if (len_r == 0 || len_c == 0)
                    {
                        printf("Файл пустой.\n");
                        return 1;
                    }

                    // Остальной код без изменений

                    printf("Начальное игровое поле:\n");
                    for (int i = 0; i < len_c * 4 - 1; i++)
                    {
                        printf("_");
                    }
                    printf("\n");
                    for (int i = 0; i < len_r; i++)
                    {
                        printf("|");
                        for (int k = 0; k < len_c; k++)
                        {
                            printf("%2d|", matrix[i][k]);
                            rowSums[i] += matrix[i][k];
                            colSums[k] += matrix[i][k];
                        }
                        printf("\n");
                    }
                    for (int i = 0; i < len_c * 4 - 1; i++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    fclose(file);
                    if (findSolution(matrix, len_r, len_c, 0, 0, rowSums, colSums))
                    {
                        printf("Решение найдено!\n");
                        printMatrix(matrix, len_r, len_c);
                    }
                    else
                    {
                        printf("Решение не найдено!\n");
                    }

                    // Освобождаем память
                    for (int i = 0; i < len_r; i++)
                    {
                        free(matrix[i]);
                    }
                    free(matrix);
                }
                else if (file_answer == 2)
                {
                    printf("Введите количество строк (от 1 до %d): ", MAX_SIZE);
                    scanf("%d", &rows);
                    if (rows < 1 || rows > MAX_SIZE)
                    {
                        printf("Неверное количество строк.\n");
                        continue;

                    }
                    printf("Введите количество столбцов (от 1 до %d): ", MAX_SIZE);
                    scanf("%d", &cols);
                    if (cols < 1 || cols > MAX_SIZE)
                    {
                        printf("Неверное количество столбцов.\n");
                        continue;
                    }

                    matrix = (int**)malloc(rows * sizeof(int*));
                    for (int i = 0; i < rows; i++)
                    {
                        matrix[i] = (int*)malloc(cols * sizeof(int));
                        for (int j = 0; j < cols; j++)
                        {
                            printf("Введите элемент [%d][%d]: ", i + 1, j + 1);
                            scanf("%d", &matrix[i][j]);
                            rowSums[i] += matrix[i][j];
                            colSums[j] += matrix[i][j];
                        }
                    }
                    printf("Начальное игровое поле:\n");
                    // Выделение памяти для матрицы
                    // ... (печать матрицы и поиск решения)
                    printMatrix(matrix, rows, cols);

                    if (findSolution(matrix, rows, cols, 0, 0, rowSums, colSums))
                    {
                        printf("Решение найдено!\n");
                        printMatrix(matrix, rows, cols);
                    }
                    else
                    {
                        lives--;
                        printf("Решение не найдено!\nУ вас осталось жизней: %d\nПопробуйте снова.\n", lives);
                    }

                    file = fopen("temp.txt", "w");
                    if (file == NULL)
                    {
                        printf("Ошибка при открытии файла.\n");
                    }
                    else
                    {
                        for (int i = 0; i < rows; i++)
                        {
                            for (int j = 0; j < cols; j++)
                            {
                                fprintf(file, "%d ", matrix[i][j]);
                            }
                            fprintf(file, "\n");
                        }

                        // ... (запись в файл)
                        fclose(file);
                    }
                    for (int i = 0; i < rows; i++)
                    {
                        free(matrix[i]);
                    }
                    free(matrix);
                }
            }
            else
            {
                printf("Для выбора пункта меню, вам нужно ввести число от 1-го до 2-x.\nПожалуйста, попробуйте ещё раз.\n");
            }
        }
        else if (answer == 4)
        {
            printf("Выход из игры...\n");
            break;
        }
        else
        {
            printf("Для выбора пункта меню, вам нужно ввести число от 1-го до 4-x.\nПожалуйста, попробуйте ещё раз.\n");
        }
    }
    printf("Спасибо, что играли с нами.\nДо следующей игры!\n");
    return 0;
}