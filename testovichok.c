#define _CRT_SECURE_NO_WARNINGS                                                                                                                 


#include <stdio.h>                                                                                             
#include <stdlib.h>                                                                                                                                                                                                                                    
#include <string.h>                                                                                        
#include <locale.h>


#define MAX_SIZE          9      
#define TARGET_SUM       100      
#define MATRIX_FILENAME  "temp.txt"

#define MAX_LIFES_NUM    3

#define TASK_USER_FROM_FILE     1
#define TASK_USER_FROM_KEYBOARD 2

#define TASK_AI_FROM_FILE       1
#define TASK_AI_FROM_KEYBOARD   2

#define MENU_ANSWER_RULES     1
#define MENU_ANSWER_TASK_USER 2
#define MENU_ANSWER_TASK_AI   3
#define MENU_ANSWER_EXIT      4

struct matrix
{
    int** data;
    int rows;
    int cols;
} typedef matrix_t;


static int matrixMalloc(matrix_t* m);
static int matrixFree(matrix_t* m);
static int matrixReadFromFile(matrix_t* m, const char* filename);
static int matrixReadFromKeyboard(matrix_t* m);
static int matrixCheckSum(matrix_t* m, int* lives, int* win);
static int matrixCheckColsSum(matrix_t* m);
static int matrixCheckRowSum(matrix_t* m);
static int matrixWriteToFile(matrix_t* m, const char* filename);
static int matrixPrint(matrix_t* m);


static int taskForUser(int answer);
static int taskForUserFromFile();
static int taskForUserFromKeyboard();
static int taskForAI(int answer);


static int findSolution(int** matrix, int rows, int cols, int x, int y, int* rowSums, int* colSums);


static int possibleValues[10][19] =
{
    {0, 10, 20, 30, 40, 50, 60, 70, 80, 90},
    {1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 31, 41, 51, 61, 71, 81, 91},
    {2, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 12, 32, 42, 52, 62, 72, 82, 92},
    {3, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 13, 23, 43, 53, 63, 73, 83, 93},
    {4, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 14, 24, 34, 54, 64, 74, 84, 94},
    {5, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 15, 25, 35, 45, 65, 75, 85, 95},
    {6, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 16, 26, 36, 46, 56, 76, 86, 96},
    {7, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 17, 27, 37, 47, 57, 67, 87, 97},
    {8, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 18, 28, 38, 48, 58, 68, 78, 98},
    {9, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 19, 29, 39, 49, 59, 69, 79, 89}
};


int main()
{
    setlocale(LC_ALL, "RUS");
    system("color F0");

    printf("Вас приветствует игра головоломка Сто («Hundred»)!\nДля выбора необходимого пункта в главном меню игры, введите число от 1-го до 4-х.\n");

    int answer = 0;
    while (answer != MENU_ANSWER_EXIT)
    {
        int file_answer = 0;

        printf("Главное меню игры:\n1. Показать правила игры.\n2. Новая игра.\n3. Головоломка для ИИ.\n4. Выход из игры.\nВаш ответ: ");

        scanf("%d", &answer);
        switch (answer)
        {
        case MENU_ANSWER_RULES:
        {
            printf("Сто («Hundred») состоит из квадратной сетки в виде кроссворда/головоломки, в каждой клетке которой проставлены цифры.\n");
            printf("Требуется поставить дополнительные цифры таким образом(слева, или справа цыфры в чейке), чтобы сумма всех чисел в каждом ряду и в каждом столбце была равна 100.\n");
            break;
        }
        case MENU_ANSWER_TASK_USER:
        {
            printf("Хотите восстановить предыдущее игровое поле?\n1. Да.\n2. Нет.\n");
            scanf("%d", &file_answer);
            taskForUser(file_answer);
            break;
        }
        case MENU_ANSWER_TASK_AI:
        {
            printf("Хотите восстановить предыдущее игровое поле?\n1. Да.\n2. Нет.\n");
            scanf("%d", &file_answer);
            taskForAI(file_answer);
            break;
        }
        case MENU_ANSWER_EXIT:
        {
            printf("Выход из игры...\n");
            break;
        }
        default:
        {
            printf("Для выбора пункта меню, вам нужно ввести число от 1-го до 4-x.\nПожалуйста, попробуйте ещё раз.\n");
        }
        }
    }

    return EXIT_SUCCESS;
}

int matrixMalloc(matrix_t* m)
{
    if (m->rows < 0 || m->cols < 0)
        return -1;

    m->data = (int**)malloc(sizeof(int*) * m->rows);
    for (int i = 0; i < m->rows; i++)
        m->data[i] = (int*)malloc(sizeof(int) * m->cols);

    return 1;
}

int matrixFree(matrix_t* m)
{
    for (int i = 0; i < m->rows; i++)
        free(m->data[i]);
    free(m->data);

    return 1;
}

int matrixReadFromFile(matrix_t* m, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла \"%s\".\n", filename);
        return -1;
    }

    m->rows = fgetc(file) - '0';
    m->cols = fgetc(file) - '0';
    fgetc(file);
    fgetc(file);

    matrixMalloc(m);

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            m->data[i][j] = fgetc(file) - '0';
            fgetc(file);
        }
        fgetc(file);
    }

    fclose(file);

    return 1;
}

int matrixReadFromKeyboard(matrix_t* m)
{
    printf("Ваше решение: \n");
    printf("Введите элементы головоломки построчно через пробел:\n");

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            scanf("%d", &m->data[i][j]);
        }
    }

    return 0;
}

int matrixCheckSum(matrix_t* m, int* lives, int* win)
{
    if (m->rows == matrixCheckColsSum(m) && m->cols == matrixCheckRowSum(m))
    {
        const int score = m->rows * 13 + m->cols * 13 + (*lives) * 100 + 1000;
        printf("Браво! Поздравляем с победой! :)\n");
        printf("Количество заработанных очков за решение головоломки: %d\n", score);

        *win = 1;
    }
    else
    {
        *lives -= 1;
        printf("Неверно! Попробуйте еще раз.\nОставшееся количество жизней: %d\n", *lives);
        if (*lives == 0)
        {
            printf("Игра окончена! Повезет в следующий раз. :(\n");
        }
    }
    return 1;
}

int matrixCheckColsSum(matrix_t* m)
{
    int sumCounter = 0;
    for (int j = 0; j < m->cols; j++)
    {
        int sum = 0;
        for (int i = 0; i < m->rows; i++)
        {
            sum += m->data[i][j];
        }
        if (sum == 100)
        {
            sumCounter++;
        }
        else
        {
            printf("Ошибка: сумма элементов столбца %d не равна 100.\n", j + 1);
            return -1;
        }
    }

    return sumCounter;
}

int matrixCheckRowSum(matrix_t* m)
{
    int sumCounter = 0;
    for (int i = 0; i < m->rows; i++)
    {
        int sum = 0;

        for (int j = 0; j < m->cols; j++)
        {
            sum += m->data[i][j];
        }

        if (sum == 100)
        {
            sumCounter++;
        }
        else
        {
            printf("Ошибка: сумма элементов строки %d не равна 100.\n", i + 1);
            return 0;
        }
    }

    return sumCounter;
}

int matrixWriteToFile(matrix_t* m, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла \"%s\".\n", filename);
        return -1;
    }

    fprintf(file, "%d", m->rows);
    fprintf(file, "%d", m->cols);
    fprintf(file, "\n\n");

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            fprintf(file, "%d ", m->data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 1;
}

int matrixPrint(matrix_t* m)
{
    for (int i = 0; i < m->cols * 4 - 1; i++)
    {
        printf("_");
    }
    printf("\n");

    for (int i = 0; i < m->rows; i++)
    {
        printf("|");
        for (int j = 0; j < m->cols; j++)
        {
            printf("%2d|", m->data[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < m->cols * 4 - 1; i++)
    {
        printf("-");
    }
    printf("\n");

    return 1;
}

int taskForUser(int answer)
{
    switch (answer)
    {
    case TASK_USER_FROM_FILE: taskForUserFromFile(); break;
    case TASK_USER_FROM_KEYBOARD: taskForUserFromKeyboard(); break;
    };

    return 1;
}

int taskForUserFromFile()
{
    matrix_t  user_matrix;
    matrix_t  start_matrix;
    int     lives;
    int     win;

    matrixReadFromFile(&start_matrix, MATRIX_FILENAME);

    printf("Начальное игровое поле:\n");
    matrixPrint(&start_matrix);

    user_matrix.rows = start_matrix.rows;
    user_matrix.cols = start_matrix.cols;
    matrixMalloc(&user_matrix);

    lives = MAX_LIFES_NUM;
    win = 0;

    while (lives && !win)
    {
        matrixReadFromKeyboard(&user_matrix);
        matrixCheckSum(&user_matrix, &lives, &win);
    }

    matrixFree(&user_matrix);
    matrixFree(&start_matrix);

    return 0;
}

int taskForUserFromKeyboard()
{
    int     rowSums[MAX_SIZE] = { 0 };
    int     colSums[MAX_SIZE] = { 0 };
    matrix_t  start_matrix;
    matrix_t  user_matrix;
    int     cols;
    int     rows;
    int     lives;
    int     win;

    printf("Введите количество строк (от 1 до %d): ", MAX_SIZE);
    scanf("%d", &rows);
    if (rows < 1 || rows > MAX_SIZE)
        printf("Неверное количество строк.\n");

    printf("Введите количество столбцов (от 1 до %d): ", MAX_SIZE);
    scanf("%d", &cols);
    if (cols < 1 || cols > MAX_SIZE)
        printf("Неверное количество столбцов.\n");

    start_matrix.rows = user_matrix.rows = rows;
    start_matrix.cols = user_matrix.cols = cols;

    matrixMalloc(&start_matrix);
    matrixMalloc(&user_matrix);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("Введите элемент [%d][%d]: ", i + 1, j + 1);
            scanf("%d", &start_matrix.data[i][j]);
            rowSums[i] += start_matrix.data[i][j];
            colSums[j] += start_matrix.data[i][j];
        }
    }

    printf("Начальное игровое поле:\n");
    matrixPrint(&start_matrix);
    matrixWriteToFile(&start_matrix, MATRIX_FILENAME);

    lives = MAX_LIFES_NUM;
    win = 0;

    while (lives && !win)
    {
        matrixReadFromKeyboard(&user_matrix);
        matrixCheckSum(&user_matrix, &lives, &win);
    }

    return 0;
}

int taskForAI(int answer)
{
    int rowSums[MAX_SIZE] = { 0 };
    int colSums[MAX_SIZE] = { 0 };

    switch (answer)
    {
    case 1:
    {
        matrix_t m;
        matrixReadFromFile(&m, MATRIX_FILENAME);
        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                rowSums[i] += m.data[i][j];
                colSums[j] += m.data[i][j];
            }
        }
        if (findSolution(m.data, m.rows, m.cols, 0, 0, rowSums, colSums))
        {
            printf("Решение найдено!\n");
            matrixPrint(&m);
        }
        else
        {
            printf("Решение не найдено!\n");
        }

        matrixFree(&m);
        break;
    }
    case 2:
    {
        matrix_t m;
        printf("Введите количество строк (от 1 до %d): ", MAX_SIZE);
        scanf("%d", &m.rows);
        if (m.rows < 1 || m.rows > MAX_SIZE)
        {
            printf("Неверное количество строк.\n");
        }

        printf("Введите количество столбцов (от 1 до %d): ", MAX_SIZE);
        scanf("%d", &m.cols);
        if (m.cols < 1 || m.cols > MAX_SIZE)
        {
            printf("Неверное количество столбцов.\n");
        }

        matrixMalloc(&m);

        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                printf("Введите элемент [%d][%d]: ", i + 1, j + 1);
                scanf("%d", &m.data[i][j]);
                rowSums[i] += m.data[i][j];
                colSums[j] += m.data[i][j];
            }
        }

        printf("Начальное игровое поле:\n");

        matrixPrint(&m);
        matrixWriteToFile(&m, MATRIX_FILENAME);
        if (findSolution(m.data, m.rows, m.cols, 0, 0, rowSums, colSums))
        {
            printf("Решение найдено!\n");
            matrixPrint(&m);
        }
        else
        {
            printf("Решение не найдено!\n");
        }


        matrixFree(&m);
        break;
    }
    default:
    {
        printf("Для выбора пункта меню, вам нужно ввести число от 1-го до 2-x.\nПожалуйста, попробуйте ещё раз.\n");
    }
    }

    return 1;
}

int findSolution(int** matrix, int rows, int cols, int x, int y, int* rowSums, int* colSums)
{
    if (x == rows)
    {
        for (int i = 0; i < rows; i++)
        {
            if (rowSums[i] != TARGET_SUM) return 0;
        }
        for (int j = 0; j < cols; j++)
        {
            if (colSums[j] != TARGET_SUM) return 0;
        }
        return 1;
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
                return 1;
            }


            matrix[x][y] = originalValue;
            rowSums[x] -= add;
            colSums[y] -= add;
        }
    }
    return 0;
}
