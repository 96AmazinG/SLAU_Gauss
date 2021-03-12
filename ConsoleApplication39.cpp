#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

using namespace std;

int      DisplayMenu(void); //печать меню и запрос пункта
double** InputMatrix(double** m, int size_m, int size_n); //ввод массива с клавиатуры
double** RandMatrix(double** m, int size_m, int size_n); //задание массива случайными числами
void     DisplayArray(double** m, int size_m, int size_n); //вывод на экран двумерного массива
void     DisplayArray(double* m, int size_m); //вывод на экран одномерного массива
int      CopyMatrix(double** source, double** target, int size_m, int size_n); //копирование массива
int      swapMatrixRow(double** m, int size_m, int size_n, int row_src, int row_trg); //перестановка строк
int      swapMatrixColumn(double** m, int size_m, int size_n, int col_src, int col_trg); //перестановка колонок
double** Matrix_Pivot(double** m, int size_m, int size_n, int index_m=-1, int index_n=-1, bool debug = false); //определения ведущего элемента и перестановка строк и столбцов
int      SLAU_Gauss(double**m, int size_m, int size_n, double* b, bool debug=false); // решение СЛАУ методом Гаусса (предполагается, что ведущий элемент стоит на первом месте)
int      Det_Gauss(double** m, int size_m, int size_n, double& det, bool debug = false); // вычисление определителя методом Гаусса
int      InvMatrix_Gauss(double** m, int size_m, int size_n, double** im, bool debug = false); // вычисление обратной матрицы методом Гаусса
int      DeleteM(double** m, double **im, double *b, int size_m); //очистка памяти от всех массивов
int      Test(void); //функция тестирования функций

int main()
{
    setlocale(LC_ALL, "Russian"); //задание русской кодировки для консоли
    srand((unsigned)time(0)); // задание (включение) генератора случайных чисел

    //задаются "пустые" массивы (ссылка на NULL) и переменная под определитель
    double** m=NULL, *b = NULL, **im = NULL, det;
    // переменные размера массива (size_m - строки, size_n - столбцы, ind_m - выбор ведущей строки, ind_m - выбор ведущего столбца)
    int size_m = 0, size_n = 0, ind_m = -1, ind_n = -1;
    int sel = 0; //sel - выбранный пункт меню

    while (sel != 10) //бесконечный цикл пока не выбирут 10 пункт меню - "Выход"
    {
        system("cls");
        sel = DisplayMenu(); //вывод меню и запрос выбора

        switch (sel) //оператор выбора действи в зависимости от введенного пункта меню
        {
        case 1: //Задать СЛАУ случайными числами
            //если ввели этот пункт, то сначало нужно удалить память под старые массивы
            DeleteM(m, im, b, size_m); //удаление массивов из памяти
            m = NULL; im = NULL; b = NULL; size_m = 0; size_n = 0;//задание "пустых" ссылок массивам и обнуление размера
            
            //запрос размера массива
            printf_s("Задать СЛАУ случайными числами\n");
            printf_s("Введите число строк: ");
            scanf_s("%d", &size_m);
            size_n = size_m + 1; // считается, что столбцов на один больше, т.е. объеденены матрица A и матрица B в один массив
            printf_s("Число столбцов: %d\n", size_n);
            
            if (size_m < 1 || size_m > 15 || size_n < 1 || size_n > 15) // если размер матрицы не от 1 до 15, то ничего не делаем
                break;
            
            //выделение памяти под все эти массивы
            m = new double* [size_m]; //выделение памяти под массив ссылок на строки матрицы
            im = new double* [size_m]; // выделение памяти под массив ссылок на троки обратной матрицы
            for (int i = 0; i < size_m; i++) //цикл по строкам массива
            {
                m[i] = new double[size_n]; //выделение памяти под каждую строку массива - столбцы
                im[i] = new double[size_n]; //выделение памяти под каждую строку массива - столбцы
            }
            b = new double[size_m]; //выделение памяти под массив ответа
            
            m = RandMatrix(m, size_m, size_n); //вызов функции заполнение матрицы случайными числами
            DisplayArray(m, size_m, size_n); // вывод массива (матрица m - объединение матриц A и B)) на экран
            break;

        case 2: //Задать СЛАУ с клавиатуры
            //если ввели этот пункт, то сначало нужно удалить память под старые массивы
            DeleteM(m, im, b, size_m); //удаление массивов из памяти
            m = NULL; im = NULL; b = NULL; size_m = 0; size_n = 0;//задание "пустых" ссылок массивам и обнуление размера
            
            //запрос размера массива
            printf_s("Задать СЛАУ с клавиатуры\n");
            printf_s("Введите число строк: ");
            scanf_s("%d", &size_m);
            size_n = size_m + 1; // считается, что столбцов на один больше, т.е. объеденены матрица A и матрица B в один массив
            printf_s("Число столбцов: %d\n", size_n);

            //выделение памяти под все эти массивы
            m = new double* [size_m]; //выделение памяти под массив ссылок на строки матрицы
            im = new double* [size_m]; // выделение памяти под массив ссылок на троки обратной матрицы
            for (int i = 0; i < size_m; i++) //цикл по строкам массива
            {
                m[i] = new double[size_n]; //выделение памяти под каждую строку массива - столбцы
                im[i] = new double[size_n]; //выделение памяти под каждую строку массива - столбцы
            }
            b = new double[size_m]; //выделение памяти под массив ответа

            m = InputMatrix(m, size_m, size_n); //вызов функции заполнение матрицы числами с клавиатуры
            DisplayArray(m, size_m, size_n); // вывод массива (матрица m - объединение матриц A и B)) на экран
            break;

        case 3: //Решить СЛАУ с выбором ведущего элемента: по всей матрице
            // вызов функции определения ведущего элемента и перестановка его на первое место
            m = Matrix_Pivot(m, size_m, size_n);

            // вывод получившейсы матрицы
            printf_s("Исходная матрица\n");
            DisplayArray(m, size_m, size_n);

            //вызов функции решения СЛАУ (true - означает выводить на экран промежуточные результаты, ответ в массиве b)
            if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
            {
                //вывод матрицы ответа - b
                printf_s("Матрица ответа\n");
                DisplayArray(b, size_m);
            }
            else //если ответа нет
                printf_s("Не могу найти ответ\n");

            break;

        case 4: //Решить СЛАУ с выбором ведущего элемента: по строке
            //запрос строки ведущего элемента
            printf_s("Введите ведущую строку: ");
            scanf_s("%d", &ind_m);
            // вызов функции определения ведущего элемента и перестановка его на первое место
            m = Matrix_Pivot(m, size_m, size_n, ind_m-1);

            printf_s("Матрица:\n");
            DisplayArray(m, size_m, size_n);

            //вызов функции решения СЛАУ (true - означает выводить на экран промежуточные результаты, ответ в массиве b)
            if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
            {
                //вывод матрицы ответа - b
                printf_s("Матрица ответа\n");
                DisplayArray(b, size_m);
            }
            else //если ответа нет
                printf_s("Не могу найти ответ\n");

            break;

        case 5: //Решить СЛАУ с выбором ведущего элемента: по столбцу
            //запрос столбца ведущего элемента
            printf_s("Введите ведущий столбец: ");
            scanf_s("%d", &ind_n);
            // вызов функции определения ведущего элемента и перестановка его на первое место
            m = Matrix_Pivot(m, size_m, size_n, -1, ind_n-1);

            printf_s("Матрица:\n");
            DisplayArray(m, size_m, size_n);

            //вызов функции решения СЛАУ (true - означает выводить на экран промежуточные результаты, ответ в массиве b)
            if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
            {
                //вывод матрицы ответа - b
                printf_s("Матрица ответа\n");
                DisplayArray(b, size_m);
            }
            else //если ответа нет
                printf_s("Не могу найти ответ\n");

            break;

        case 6://Вычислить определитель матрицы системы, используя метод Гаусса
            //вызов вычисления определителя матрицы (true - означает выводить на экран промежуточные результаты)
            Det_Gauss(m, size_m, size_n, det, true);
            printf_s("\n%lf\n", det);
            break;

        case 7://Найти обратную матрицу для матрицы системы, используя метод Гаусса
            //вычисление обратной матрицы
            InvMatrix_Gauss(m, size_m, size_n, im, true);(true - означает выводить на экран промежуточные результаты)
            DisplayArray(im, size_m, size_n - 1);
            break;

        case 8://Тестирование
            Test();
            break;

        case 9: //Вывод массива на экран
            DisplayArray(m, size_m, size_n);
            break;

        default:
            break;
        }
        printf_s("Для продолжения нажмите любую клавишу...\n");
        _getch();
    }


    //Удаляем массивы
    DeleteM(m, im, b, size_m);

    return 0;
}

//печать меню и запрос пункта
int DisplayMenu(void)
{
    int s = 1;
    //вывод списка меню
    printf_s("Решение системы линейных алгебраических уравнений (СЛАУ) методом Гаусса:\n");
    printf_s("------------------------------------------------------------------------\n");
    printf_s("%d. Задать СЛАУ случайными числами\n", s++);
    printf_s("%d. Задать СЛАУ с клавиатуры\n", s++);
    printf_s("%d. Решить СЛАУ с выбором ведущего элемента: по всей матрице\n", s++);
    printf_s("%d. Решить СЛАУ с выбором ведущего элемента: по строке\n", s++);
    printf_s("%d. Решить СЛАУ с выбором ведущего элемента: по столбцу\n", s++);
    printf_s("%d. Вычислить определитель матрицы системы, используя метод Гаусса\n", s++);
    printf_s("%d. Найти обратную матрицу для матрицы системы, используя метод Гаусса\n", s++);
    printf_s("%d. Тестирование\n", s++);
    printf_s("%d. Вывод массив\n", s++);
    printf_s("%d. Выход\n", s++);
    printf_s("------------------------------------------------------------------------\n");
    printf_s("Выберете пункт меню: ");
    scanf_s("%d", &s);
    return s;
}

//ввод массива с клавиатуры
double** InputMatrix(double** m, int size_m, int size_n)
{
    int i; //Переменная, номер строки.
    int j; //Переменная, номер столбца.

    if (m == NULL)//если под массив не выделена память, то выделяем
    {
        m = new double* [size_m];
        for (i = 0; i < size_m; i++)
            m[i] = new double[size_n];
    }

    //Инициализируем:
    for (i = 0; i < size_m; i++)//массив по строкам
    {
        for (j = 0; j < size_n; j++)//массив по столбцам
        {
            if (j == size_n-1)//Каждый последний элемент в строке будет являться свободным членом уравнения соответствующего этой строке
                printf_s("Введите свободный член в строке %d: ", i+1);
            else
                printf_s("Введите элемент матрицы (%d, %d): ", i + 1, j + 1);
            scanf_s("%lf", &m[i][j]);
        }
    }
    return m;
}

//задание массива случайными числами
double** RandMatrix(double** m, int size_m, int size_n)
{
    int i;//Переменная, номер строки.
    int j;//Переменная, номер столбца.

    if (m == NULL)//если под массив не выделена память, то выделяем
    {
        m = new double* [size_m];
        for (i = 0; i < size_m; i++)
            m[i] = new double[size_n];
    }

    //Инициализируем:
    for (i = 0; i < size_m; i++)
        for (j = 0; j < size_n; j++)
            m[i][j] = rand() % 21 - 10;

    return m;
}

//вывод на экран массива
void DisplayArray(double** m, int size_m, int size_n)
{
    if (m == NULL) return;
    int i;//Переменная, номер строки.
    int j;//Переменная, номер столбца.

    //Выводим массив
    for (i = 0; i < size_m; i++) //строки
    {
        for (j = 0; j < size_n; j++) //столбцы
            printf_s("%10.3lf",m[i][j]);
        cout << endl; //вывод окончания строки
    }
    cout << endl;//вывод окончания строки
    return;
}

//вывод на экран массива
void DisplayArray(double* m, int size_m)
{
    if (m == NULL) return;//если не выделена под массив память, то выходим из процедуры
    int i;//Переменная, номер строки.

    //Выводим массив:
    for (i = 0; i < size_m; i++)
        printf_s("%10.3lf", m[i]);
    cout << endl;
    return;
}

//Копирование матрицы
int CopyMatrix(double** source, double** target, int size_m, int size_n)
{
    if (source == NULL || target == NULL) return 1;//если не выделена память под один или другой массив, то и выходим из функции
    for (int i = 0; i < size_m; i++)//строки
        for (int j = 0; j < size_n; j++)//столбцы
            target[i][j] = source[i][j];
    return 0;
}

//перестановка строк
int swapMatrixRow(double** m, int size_m, int size_n, int row_src, int row_trg)
{
    double tmp;
    for (int j = 0; j < size_n; j++)//цикл по столбцу
    {
        //стандартный обмен элементов через третий
        tmp = m[row_src][j];
        m[row_src][j] = m[row_trg][j];
        m[row_trg][j] = tmp;
    }
    return 0;
}

//перестановка строк
int swapMatrixColumn(double** m, int size_m, int size_n, int col_src, int col_trg)
{
    double tmp;
    for (int i = 0; i < size_m; i++)//цикл по строкам
    {
        //стандартный обмен элементов через третий
        tmp = m[i][col_src];
        m[i][col_src] = m[i][col_trg];
        m[i][col_trg] = tmp;
    }
    return 0;
}

// определения ведущего элемента и перестановка строк и столбцов
// если не задана строка и столбец (index_m = -1 и index_n = -1), то ведущий по матрице
// если задана строка (index_n = -1), то ищется в строке
// если задан столбец (index_m = -1), то ищется в столбцу
double** Matrix_Pivot(double** m, int size_m, int size_n, int index_m , int index_n, bool debug)
{
    if (m == NULL) return NULL;
    int i;//Переменная, номер строки.
    int j;//Переменная, номер столбца.

    int iabsmax = 0, jabsmax = 0; // индексы максимального элемента
    int step_m = 1; // шаг перечисления для строк
    int beg_m = 0; // начальная строка
    int step_n = 1; // шаг перечисления для столбцов
    int beg_n = 0; // начальный столбец

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
    {
        printf("Процесс выбора ведущего элемента\n");
        DisplayArray(m, size_m, size_n);
    }

    if (index_m != -1)//если выбрана ведущая строка, то
    {
        beg_m = index_m;//задаем в какой строке ищем ведущий элемент
        step_m = size_m;//шаг перечисления по строкам (идея в том, что шаг больше длины массива в данном направлении и цикл автоматически перейдет к следующему элементу в строке)
    }
    if (index_n != -1)
    {
        beg_n = index_n;//задаем в каком столбце ищем ведущий элемент
        step_n = size_n;//шаг перечисления по столбцу (идея в том, что шаг больше длины массива в данном направлении и цикл автоматически перейдет к следующему элементу в столбце)
    }
    //предположим, что первый элемент (в матрице/столбце/строке) максимальный
    iabsmax = beg_m;
    jabsmax = beg_n;

    //Поиск ведущего элемента:
    for (i = beg_m; i < size_m; i+=step_m) // цикл по строкам
    {
        for (j = beg_n; j < size_n-1; j += step_n) //цикл по стролбцам
        {
            if (fabs(m[i][j]) > fabs(m[iabsmax][jabsmax])) // текущий элемент больше уже найденого, то макс становится ведущим
            {
                iabsmax = i;
                jabsmax = j;
            }
        }
    }

    //Перестановка строк:
    if (iabsmax != 0)//если ведущий элемент не в первой строке, то нужно сделать, чтобы стал
    {
        swapMatrixRow(m, size_m, size_n, iabsmax, 0);//смена строк iabsmax <=> 0
        if (debug)//если включен режим отладки (debug = true), то выводим матрицу
            DisplayArray(m, size_m, size_n);
    }

    if (jabsmax != 0)//если ведущий элемент не в первой столбце, то нужно сделать, чтобы стал
    {
        swapMatrixColumn(m, size_m, size_n, jabsmax, 0);//смена столбцов jabsmax <=> 0
        if (debug)//если включен режим отладки (debug = true), то выводим матрицу
            DisplayArray(m, size_m, size_n);
    }
    return m; // возвращаем полученную марицу
}

// решение СЛАУ методом Гаусса (предполагается, что ведущий элемент стоит на первом месте)
int SLAU_Gauss(double** m1, int size_m, int size_n, double* b, bool debug)
{
    if (m1 == NULL) return 0;
    int i; //Переменная, номер строки
    int j; //Переменная, номер столбца
    int k;
    double tmp;

    //копируем матрицу, чтобы не испротить исходную
    double** m = new double* [size_m];
    for (i = 0; i < size_m; i++)
        m[i] = new double[size_n];
    CopyMatrix(m1, m, size_m, size_n);

    if(b==NULL)//если не выделена память под ответ, то выделяем
        b = new double[size_m];

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
    {
        printf("Процесс решение СЛАУ методом Гаусса\n");
        DisplayArray(m, size_m, size_n);
    }

    //Приведение к верхнетреугольному виду
    for(k = 0; k < size_m; k++) //цикл для перебора по диагонали
    {
        tmp = m[k][k]; //Первый элемент в текущей матрице

        if (tmp != 0) // если текущий диагональный элемент не равен 0 (можно на него делить)
        {
            for (j = k; j < size_n; j++) // цикл по строке: нормализация строки (деление на диагональный элемент)
                m[k][j] = m[k][j] / tmp;
            for (i = k + 1; i < size_m; i++)//цикл по остальным строчкам
            {
                tmp = m[i][k]; // главный элемент в текущей строке
                // Из следующей строки вычитаем первую, умноженную на главный элемент
                for (j = k; j < size_m + 1; j++) // цикл по столбцам (для вычисления всех элементов в текущей строке)
                    m[i][j] = m[i][j] - tmp * m[k][j]; 
            }
        }
    }

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
        DisplayArray(m, size_m, size_n);

    // Продолжаем формировать матрицу. теперь оставшуюся часть (правый верхний угол матрицы) приводим к 0
    for (k = size_m - 1; k >= 0; k--) // цикл по диагональному элементу, начиная с конца
    {
        for (i = 0; i < k; i++) // цикл по строке
        {
            tmp = m[i][k]; // Множитель
            for (j = k; j < size_m + 1; j++) // Из текущей строки вычитаем k-ую, умноженную на главный элемент
                m[i][j] = m[i][j] - tmp * m[k][j];
        }
    }

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
        DisplayArray(m, size_m, size_n);

    bool nenulinline = false; // Переменная, которая обозначает, что в строке есть ненулевой элемент
    bool Result=0; // Переменная, которая обозначает количество решений: нет, одно или бесконечное множество

    for (k = 0; k < size_m; k++)
    {
        if (m[k][k] == 0) //Если на диагонали 0, значит проверяем строку
        {
            nenulinline = false; // Обнуляем переменную
            for (j = k + 1; j < size_n - 1; j++)
            {
                if (m[k][j] != 0) // Если в строке есть ненулевой элемент значит выставим об этом флаг
                    nenulinline = true;
            }
            if (!nenulinline) // Если в строке все нули, то проверям свободный член
            {
                if (m[k][size_n] != 0) // Если не равно нулю, то решений нет
                    Result = -1;
                else // Иначе данное уравнение имеет бесконечное количество решений
                    Result = 1;
            }
        }
    }

    if (Result == 0) //если можно вычислить результат (один ответ)
    {
        // копирование в матрицу ответов
        for (i = 0; i < size_m; i++)
            b[i] = m[i][size_n - 1];
        
        //переменные для вычисления ошибки
        double* x, sig;
        x = new double[size_m];
        //перемножение матрицы A и X
        for (i = 0; i < size_m; i++)//цикл по матрице ответа
        {
            x[i] = 0;
            for (j = 0; j < size_n - 1; j++) //цикл по столбцу исх матрицы
                x[i] += m1[i][j] * b[j]; //суммирование при умножении матриц
        }

        // определение сигмы
        sig = fabs(x[0]-m1[0][size_m]);//определение погрешности
        for (i = 0; i < size_m; i++)//цикл по элементам вектора свободных членов и вектора ответа
            if (sig < fabs(x[i] - m1[i][size_m])) // если нашил погрешность больше, чем есть, то
                sig = fabs(x[i] - m1[i][size_m]); //  запомнитьт новую погрешность
        printf_s("Максимальная ошибка: %lf\n", sig); // вывод макс погрешности
        // p.s. тут  немножко напутано: 1) матрица ответов должна быть х, а тут b;
        // 2) матрица умножение A на X это b, а тут x.
        // т.е. b и x "в традиционном понимании" поменяны
        delete x;
    }

    return Result;
}

// вычисление определителя
int Det_Gauss(double** m1, int size_m, int size_n, double& det, bool debug)
{
    if (m1 == NULL) return 0;
    int i; //Переменная, номер строки
    int j; //Переменная, номер столбца
    int k;
    double tmp;

    //копируем матрицу, чтобы не испротить исходную
    double** m = new double* [size_m];
    for (i = 0; i < size_m; i++)
        m[i] = new double[size_n];

    CopyMatrix(m1, m, size_m, size_n);

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
    {
        printf("вычисление определителя\n");
        DisplayArray(m, size_m, size_n);
    }

    //Приведение к верхнетреугольному виду, аналогично в методе Гаусса
    for (k = 0; k < size_m; k++)
    {
        tmp = m[k][k]; //Первый элемент в текущей матрице
        if (tmp != 0)
        {
            for (i = k + 1; i < size_m; i++)
            {
                tmp = m[i][k]; // Главный элемент
                // Из следующей строки вычитаем первую, умноженную на главный элемент
                for (j = k; j < size_m + 1; j++)
                    m[i][j] = m[i][j] - tmp * m[k][j] / m[k][k];
            }
        }
    }

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
        DisplayArray(m, size_m, size_n);

    // теперь определитель это произведение диагональных элементов
    det = 1;
    for (i = 0; i < size_m; i++)//цикл по диагонали
        det *= m[i][i];
    return tmp;
}

// вычисление обратной матрицы
int InvMatrix_Gauss(double** m1, int size_m, int size_n, double** im, bool debug)
{
    if (m1 == NULL) return 0;
    int i; //Переменная, номер строки
    int j; //Переменная, номер столбца
    int k, size_n2 = (size_n - 1) * 2;
    double tmp;

    if (im == NULL) //если не выделена память под обратную матрицу, то выделить
    {
        im = new double* [size_m];
        for (int i = 0; i < size_m; i++)
            im[i] = new double[size_n-1];
    }

    //копируем матрицу, чтобы не испротить исходную
    double** m = new double* [size_m];
    for (i = 0; i < size_m; i++)
        m[i] = new double[size_n2];
    CopyMatrix(m1, m, size_m, size_n-1);
    // матрица m имеет в два больше столбцов (см. описание метода)

    
    for (i = 0; i < size_m; i++)//цикл по строкам
        for (j = size_n - 1; j < size_n2; j++) // цикл по столбцам
            if (i - 1 == j - size_n) //диагональные элементы матрицы заполняются "1",
                m[i][j] = 1;
            else                     // остальные элементы заполняются "0"
                m[i][j] = 0;

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
    {
        printf("вычисление обратной матрицы\n");
        DisplayArray(m, size_m, size_n);
    }

    //Приведение к верхнетреугольному виду, аналогично в методе Гаусса
    for (k = 0; k < size_m; k++)
    {
        tmp = m[k][k]; //Первый элемент в текущей матрице

        if (tmp != 0)
        {
            for (j = k; j < size_n2; j++) // Нормализация строки
                m[k][j] = m[k][j] / tmp;
            for (i = k + 1; i < size_m; i++)
            {
                tmp = m[i][k]; // Главный элемент
                // Из следующей строки вычитаем первую, умноженную на главный элемент
                for (j = k; j < size_n2; j++)
                    m[i][j] = m[i][j] - tmp * m[k][j];
            }
        }
    }

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
        DisplayArray(m, size_m, size_n2);

    // Продолжаем формировать матрицу. теперь оставшуюся часть (правый верхний угол матрицы) приводим к 0
    for (k = size_m - 1; k >= 0; k--)//цикл по диагонали
    {
        for (i = 0; i < k; i++)//цикл по строкам
        {
            tmp = m[i][k]; // Множитель
            for (j = k; j < size_n2; j++) // Из следующей строки вычитаем первую, умноженную на главный элемент
                m[i][j] = m[i][j] - tmp * m[k][j];
        }
    }

    if (debug)//если включен режим отладки (debug = true), то выводим матрицу
        DisplayArray(m, size_m, size_n2);

    // обратная матрица это вторая часть получившейся матрицы
    for (i = 0; i < size_m; i++)
        for (j = 0; j < size_n-1; j++)
            im[i][j] = m[i][j + size_n - 1];

    return 0;
}


// освобождение памяти
int DeleteM(double** m, double** im, double *b, int size_m)
{
    for (int i = 0; i < size_m; i++)// цикл по строкам
    {
        if (m != NULL) if (m[i] != NULL) delete m[i]; //если была выделенеа память для строки, то освободить эту память
        if (im != NULL) if (im[i] != NULL) delete im[i]; //если была выделенеа память для строки, то освободить эту память
    }
    if (m != NULL) delete[] m; //освободить память
    if (im != NULL) delete[] im; //освободить память
    if (b != NULL) delete[] b; //освободить память

    return 0;
}

// процедура тестирования функций
int Test(void)
{
    double** m = NULL, * b = NULL, ** im = NULL, det;
    int size_m = 3, size_n = size_m+1;
    int i, j;

    //выделение памяти под все матрицы
    m = new double* [size_m];
    im = new double* [size_m];
    for (int i = 0; i < size_m; i++)
    {
        m[i] = new double[size_n];
        im[i] = new double[size_n];
    }
    b = new double[size_m];


    //Тестирование явно заданной матрицы
    printf("Тестирование явно заданной матрицы\n");
    m[0][0] = 1; m[0][1] =-3; m[0][2] =-6; m[0][3] = -9;
    m[1][0] = 3; m[1][1] = 4; m[1][2] = 5; m[1][3] = 22;
    m[2][0] = 2; m[2][1] = 4; m[2][2] =-4; m[2][3] = 10;

    //определение ведущего элемента по матрице и печать матрицы
    m = Matrix_Pivot(m, size_m, size_n);
    DisplayArray(m, size_m, size_n);

    //решение СЛАУ и вывод результата
    if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
    {
        printf_s("Матрица ответа\n");
        DisplayArray(b, size_m);
    }
    else
        printf_s("Не могу найти ответ\n");

    // вычисление определителя матрицы
    Det_Gauss(m, size_m, size_n, det, true);
    printf_s("\n%lf\n", det);

    // нахождение обратной матрицы
    InvMatrix_Gauss(m, size_m, size_n, im, true);
    DisplayArray(im, size_m, size_n - 1);


    //Тестирование еденичной матрицы
    printf("Тестирование еденичной матрицы\n");
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 4;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = -2;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 1;

    //определение ведущего элемента по матрице и печать матрицы
    m = Matrix_Pivot(m, size_m, size_n);
    DisplayArray(m, size_m, size_n);

    //решение СЛАУ и вывод результата
    if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
    {
        printf_s("Матрица ответа\n");
        DisplayArray(b, size_m);
    }
    else
        printf_s("Не могу найти ответ\n");

    // вычисление определителя матрицы
    Det_Gauss(m, size_m, size_n, det, true);
    printf_s("\n%lf\n", det);

    // нахождение обратной матрицы
    InvMatrix_Gauss(m, size_m, size_n, im, true);
    DisplayArray(im, size_m, size_n - 1);


    //Тестирование вырожденной матрицы
    printf("Тестирование вырожденной матрицы\n");
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 4;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 2; m[1][3] = -2;
    m[2][0] = 0; m[2][1] = 1; m[2][2] = 2; m[2][3] = 1;

    //определение ведущего элемента по матрице и печать матрицы
    m = Matrix_Pivot(m, size_m, size_n);
    DisplayArray(m, size_m, size_n);

    //решение СЛАУ и вывод результата
    if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
    {
        printf_s("Матрица ответа\n");
        DisplayArray(b, size_m);
    }
    else
        printf_s("Не могу найти ответ\n");

    // вычисление определителя матрицы
    Det_Gauss(m, size_m, size_n, det, true);
    printf_s("\n%lf\n", det);

    // нахождение обратной матрицы
    InvMatrix_Gauss(m, size_m, size_n, im, true);
    DisplayArray(im, size_m, size_n - 1);


    //Тестирование матрицы Гильберта
    printf("Тестирование матрицы Гильберта\n");
    for (i = 0; i < size_m; i++)
        for (j = 0; j < size_n; j++)
            m[i][j] = 1.0 / (i + j + 1);

    //определение ведущего элемента по матрице и печать матрицы
    m = Matrix_Pivot(m, size_m, size_n);
    DisplayArray(m, size_m, size_n);

    //решение СЛАУ и вывод результата
    if (SLAU_Gauss(m, size_m, size_n, b, true) == 0)
    {
        printf_s("Матрица ответа\n");
        DisplayArray(b, size_m);
    }
    else
        printf_s("Не могу найти ответ\n");

    // вычисление определителя матрицы
    Det_Gauss(m, size_m, size_n, det, true);
    printf_s("\n%lf\n", det);

    // нахождение обратной матрицы
    InvMatrix_Gauss(m, size_m, size_n, im, true);
    DisplayArray(im, size_m, size_n - 1);


    return 0;
}

