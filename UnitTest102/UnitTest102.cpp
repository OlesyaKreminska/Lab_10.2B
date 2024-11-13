#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab_10.2B/Lab_10.2B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StudentTests
{
    TEST_CLASS(StudentTests)
    {
    public:

        TEST_METHOD(TestCreateAndPrint)
        {
            const int N = 1;
            Student students[N];

            students[0].prizv = "Ivanenko";
            students[0].kurs = 2;
            students[0].spetsialnist = KOMPYUTERNI_NAUKY;
            students[0].riven = SEREDN²É;
         

            Assert::AreEqual(std::string("Ivanenko"), students[0].prizv);
            Assert::AreEqual(2, students[0].kurs);
            Assert::AreEqual(static_cast<int>(KOMPYUTERNI_NAUKY), static_cast<int>(students[0].spetsialnist));
            Assert::AreEqual(static_cast<int>(SEREDN²É), static_cast<int>(students[0].riven));
           
        }

    };
}
