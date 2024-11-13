#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <Windows.h>
using namespace std;

enum Spetsialnist { KOMPYUTERNI_NAUKY, INFORMATYKA, MATEMATYKA_TA_EKONOMIKA, FIZYKA_TA_INFORMATYKA, TRUDOVE_NAVCHANNYA };
enum Riven { NIZHCHИЙ, SEREDNІЙ, VISHCHИЙ };

struct Student {
    string prizv;
    int kurs;
    Spetsialnist spetsialnist;
    Riven riven;
    int ozenka_fizyka;
    int ozenka_matematyka;
    union {
        int ozenka_programuvannya;  // Only for KOMPYUTERNI_NAUKY
        int ozenka_chyselni_metody; // Only for INFORMATYKA
        int ozenka_pedagogika;      // For other specializations
    };

    double AverageScore() const {  // Метод для обчислення середнього балу
        return (ozenka_fizyka + ozenka_matematyka + (spetsialnist == KOMPYUTERNI_NAUKY ? ozenka_programuvannya :
            spetsialnist == INFORMATYKA ? ozenka_chyselni_metody : ozenka_pedagogika)) / 3.0;
    }

    int ThirdGrade() const {  // Метод для отримання оцінки з третього предмету
        return (spetsialnist == KOMPYUTERNI_NAUKY) ? ozenka_programuvannya :
            (spetsialnist == INFORMATYKA) ? ozenka_chyselni_metody : ozenka_pedagogika;
    }
};

void Create(Student* s, const int N);
void Print(const Student* s, const int N, const int* indexArray);
void SortStudents(Student* s, const int N, int* indexArray);
int BinarySearch(const Student* s, const int* indexArray, int N, const string& prizv, double avgScore, int thirdGrade);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int N;
    cout << "Введіть кількість студентів: ";
    cin >> N;

    Student* students = new Student[N];
    int* indexArray = new int[N];  // Індексний масив

    int menuItem;
    do {
        cout << "\nМеню:\n";
        cout << " [1] - введення даних\n";
        cout << " [2] - виведення даних\n";
        cout << " [3] - сортування студентів\n";
        cout << " [4] - пошук студента\n";
        cout << " [0] - вихід\n";
        cout << "Введіть ваш вибір: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            Create(students, N);
            for (int i = 0; i < N; i++) indexArray[i] = i;  // Ініціалізація індексного масиву
            break;
        case 2:
            Print(students, N, indexArray);
            break;
        case 3:
            SortStudents(students, N, indexArray);
            break;
        case 4: {
            string prizv;
            double avgScore;
            int thirdGrade;
            cout << "Введіть прізвище студента для пошуку: ";
            cin.ignore();
            getline(cin, prizv);
            cout << "Введіть середній бал студента: ";
            cin >> avgScore;
            cout << "Введіть оцінку з третього предмету: ";
            cin >> thirdGrade;
            int index = BinarySearch(students, indexArray, N, prizv, avgScore, thirdGrade);
            if (index != -1) {
                cout << "Студент знайдений:\n";
                Print(&students[index], 1, indexArray);
            }
            else {
                cout << "Студента не знайдено.\n";
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "Помилка! Слід вибрати коректний номер.\n";
        }
    } while (menuItem != 0);

    delete[] students;
    delete[] indexArray;
    return 0;
}

void Create(Student* s, const int N) {
    for (int i = 0; i < N; i++) {
        cout << "Студент №" << i + 1 << ":\n";
        cin.ignore();
        cout << "Прізвище: ";
        getline(cin, s[i].prizv);
        cout << "Курс: ";
        cin >> s[i].kurs;
        cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
        int sp;
        cin >> sp;
        s[i].spetsialnist = static_cast<Spetsialnist>(sp);
        cout << "Рівень студента (0 - Нижчий, 1 - Середній, 2 - Вищий): ";
        int r;
        cin >> r;
        s[i].riven = static_cast<Riven>(r);
        cout << "Оцінка з фізики: ";
        cin >> s[i].ozenka_fizyka;
        cout << "Оцінка з математики: ";
        cin >> s[i].ozenka_matematyka;

        if (s[i].spetsialnist == KOMPYUTERNI_NAUKY) {
            cout << "Оцінка з програмування: ";
            cin >> s[i].ozenka_programuvannya;
        }
        else if (s[i].spetsialnist == INFORMATYKA) {
            cout << "Оцінка з чисельних методів: ";
            cin >> s[i].ozenka_chyselni_metody;
        }
        else {
            cout << "Оцінка з педагогіки: ";
            cin >> s[i].ozenka_pedagogika;
        }
        cout << endl;
    }
}

void Print(const Student* s, const int N, const int* indexArray) {
    cout << "=================================================================================================\n";
    cout << "| № | Прізвище    | Курс | Спеціальність          | Рівень  | Фізика | Математика | Третій Предмет |\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < N; i++) {
        int idx = indexArray[i];
        cout << left << "| " << setw(2) << i + 1 << " ";
        cout << "| " << setw(12) << left << s[idx].prizv;
        cout << "| " << setw(4) << s[idx].kurs << " ";
        string spets;
        switch (s[idx].spetsialnist) {
        case KOMPYUTERNI_NAUKY: spets = "Комп'ютерні науки"; break;
        case INFORMATYKA: spets = "Інформатика"; break;
        case MATEMATYKA_TA_EKONOMIKA: spets = "Математика та економіка"; break;
        case FIZYKA_TA_INFORMATYKA: spets = "Фізика та інформатика"; break;
        case TRUDOVE_NAVCHANNYA: spets = "Трудове навчання"; break;
        }
        cout << "| " << setw(23) << left << spets;

        string riven_str;
        switch (s[idx].riven) {
        case NIZHCHИЙ: riven_str = "Нижчий"; break;
        case SEREDNІЙ: riven_str = "Середній"; break;
        case VISHCHИЙ: riven_str = "Вищий"; break;
        }
        cout << "| " << setw(8) << left << riven_str;

        cout << "| " << setw(7) << s[idx].ozenka_fizyka;
        cout << "| " << setw(11) << s[idx].ozenka_matematyka;
        cout << "| " << setw(14) << s[idx].ThirdGrade();
        cout << " |\n";
    }
    cout << "=================================================================================================\n";
}

void SortStudents(Student* s, const int N, int* indexArray) {
    sort(indexArray, indexArray + N, [&s](int a, int b) {
        if (s[a].AverageScore() != s[b].AverageScore())
            return s[a].AverageScore() > s[b].AverageScore();
        if (s[a].ThirdGrade() != s[b].ThirdGrade())
            return s[a].ThirdGrade() > s[b].ThirdGrade();
        return s[a].prizv > s[b].prizv;
        });
    cout << "Індексний масив відсортовано за умовами завдання.\n";
}

int BinarySearch(const Student* s, const int* indexArray, int N, const string& prizv, double avgScore, int thirdGrade) {
    int left = 0;
    int right = N - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int idx = indexArray[mid];
        if (s[idx].prizv == prizv && s[idx].AverageScore() == avgScore && s[idx].ThirdGrade() == thirdGrade) {
            return idx;
        }
        if (s[idx].AverageScore() > avgScore || (s[idx].AverageScore() == avgScore && s[idx].ThirdGrade() > thirdGrade) || (s[idx].AverageScore() == avgScore && s[idx].ThirdGrade() == thirdGrade && s[idx].prizv > prizv)) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}
