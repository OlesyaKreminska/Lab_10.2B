#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <Windows.h>
using namespace std;

enum Spetsialnist { KOMPYUTERNI_NAUKY, INFORMATYKA, MATEMATYKA_TA_EKONOMIKA, FIZYKA_TA_INFORMATYKA, TRUDOVE_NAVCHANNYA };
enum Riven { NIZHCH��, SEREDN��, VISHCH�� };

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

    double AverageScore() const {  // ����� ��� ���������� ���������� ����
        return (ozenka_fizyka + ozenka_matematyka + (spetsialnist == KOMPYUTERNI_NAUKY ? ozenka_programuvannya :
            spetsialnist == INFORMATYKA ? ozenka_chyselni_metody : ozenka_pedagogika)) / 3.0;
    }

    int ThirdGrade() const {  // ����� ��� ��������� ������ � �������� ��������
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
    cout << "������ ������� ��������: ";
    cin >> N;

    Student* students = new Student[N];
    int* indexArray = new int[N];  // ��������� �����

    int menuItem;
    do {
        cout << "\n����:\n";
        cout << " [1] - �������� �����\n";
        cout << " [2] - ��������� �����\n";
        cout << " [3] - ���������� ��������\n";
        cout << " [4] - ����� ��������\n";
        cout << " [0] - �����\n";
        cout << "������ ��� ����: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            Create(students, N);
            for (int i = 0; i < N; i++) indexArray[i] = i;  // ����������� ���������� ������
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
            cout << "������ ������� �������� ��� ������: ";
            cin.ignore();
            getline(cin, prizv);
            cout << "������ ������� ��� ��������: ";
            cin >> avgScore;
            cout << "������ ������ � �������� ��������: ";
            cin >> thirdGrade;
            int index = BinarySearch(students, indexArray, N, prizv, avgScore, thirdGrade);
            if (index != -1) {
                cout << "������� ���������:\n";
                Print(&students[index], 1, indexArray);
            }
            else {
                cout << "�������� �� ��������.\n";
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "�������! ��� ������� ��������� �����.\n";
        }
    } while (menuItem != 0);

    delete[] students;
    delete[] indexArray;
    return 0;
}

void Create(Student* s, const int N) {
    for (int i = 0; i < N; i++) {
        cout << "������� �" << i + 1 << ":\n";
        cin.ignore();
        cout << "�������: ";
        getline(cin, s[i].prizv);
        cout << "����: ";
        cin >> s[i].kurs;
        cout << "������������ (0 - ����'����� �����, 1 - �����������, 2 - ���������� �� ��������, 3 - Գ���� �� �����������, 4 - ������� ��������): ";
        int sp;
        cin >> sp;
        s[i].spetsialnist = static_cast<Spetsialnist>(sp);
        cout << "г���� �������� (0 - ������, 1 - �������, 2 - �����): ";
        int r;
        cin >> r;
        s[i].riven = static_cast<Riven>(r);
        cout << "������ � ������: ";
        cin >> s[i].ozenka_fizyka;
        cout << "������ � ����������: ";
        cin >> s[i].ozenka_matematyka;

        if (s[i].spetsialnist == KOMPYUTERNI_NAUKY) {
            cout << "������ � �������������: ";
            cin >> s[i].ozenka_programuvannya;
        }
        else if (s[i].spetsialnist == INFORMATYKA) {
            cout << "������ � ��������� ������: ";
            cin >> s[i].ozenka_chyselni_metody;
        }
        else {
            cout << "������ � ���������: ";
            cin >> s[i].ozenka_pedagogika;
        }
        cout << endl;
    }
}

void Print(const Student* s, const int N, const int* indexArray) {
    cout << "=================================================================================================\n";
    cout << "| � | �������    | ���� | ������������          | г����  | Գ���� | ���������� | ����� ������� |\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < N; i++) {
        int idx = indexArray[i];
        cout << left << "| " << setw(2) << i + 1 << " ";
        cout << "| " << setw(12) << left << s[idx].prizv;
        cout << "| " << setw(4) << s[idx].kurs << " ";
        string spets;
        switch (s[idx].spetsialnist) {
        case KOMPYUTERNI_NAUKY: spets = "����'����� �����"; break;
        case INFORMATYKA: spets = "�����������"; break;
        case MATEMATYKA_TA_EKONOMIKA: spets = "���������� �� ��������"; break;
        case FIZYKA_TA_INFORMATYKA: spets = "Գ���� �� �����������"; break;
        case TRUDOVE_NAVCHANNYA: spets = "������� ��������"; break;
        }
        cout << "| " << setw(23) << left << spets;

        string riven_str;
        switch (s[idx].riven) {
        case NIZHCH��: riven_str = "������"; break;
        case SEREDN��: riven_str = "�������"; break;
        case VISHCH��: riven_str = "�����"; break;
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
    cout << "��������� ����� ����������� �� ������� ��������.\n";
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
