#include "CppUnitTest.h"

#include <windows.h>

#include "../../../../../process.hpp"

#include <cassert>
#include <experimental/filesystem>
using namespace std::experimental::filesystem::v1; 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WinExeMsTests
{		
	TEST_CLASS(WinExeMsTests)
	{
	public:
		
        std::experimental::filesystem::path _sut;
        std::experimental::filesystem::path _pattern_dir;

        TEST_METHOD_INITIALIZE(BeforeEachTestMethodIsRun)
        {
            _pattern_dir = L"../../tests/patterns/";

            assert(std::experimental::filesystem::is_directory(_pattern_dir));
            assert(std::experimental::filesystem::exists(_pattern_dir));
        }

        static _Check_return_ bool FilesAreTheSame (_In_ const std::experimental::filesystem::path& file1,
                                                    _In_ const std::experimental::filesystem::path& file2)
        {
            using namespace std::string_literals;
            const auto fileComparisonExe  = L"FC.exe"s;
            const auto args = fileComparisonExe + L" " + file1.wstring() + L" " + file2.wstring();
            const DWORD FC_EXIT_CODE_WHEN_FILES_ARE_THE_SAME = 0;
            const auto exit_code = RunCmd(args);
            return (FC_EXIT_CODE_WHEN_FILES_ARE_THE_SAME == exit_code);
        }

        static int RunCmd(_In_ const std::wstring& cmd,
          _In_ std::function<void(const char *bytes, size_t n)> read_stdout=nullptr,
          _In_ std::function<void(const char *bytes, size_t n)> read_stderr=nullptr)
        {
            TinyProcessLib::Process proc(cmd, L"", read_stdout, read_stderr);
            return proc.get_exit_status();
        }

		TEST_METHOD(CompareTwoIdenticalFiles_ReturnsTheSame)
		{
            const auto expected_exit_code = 0;

            const auto file1 = _pattern_dir / "ha1.txt";
            const auto file2 = _pattern_dir / "ha2.txt";

            assert(std::experimental::filesystem::exists(file1));
            assert(std::experimental::filesystem::exists(file2));
            assert(std::experimental::filesystem::is_regular_file(file1));
            assert(std::experimental::filesystem::is_regular_file(file2));

            const auto same = FilesAreTheSame(file1, file2);

            Assert::IsTrue(same);
		}

		TEST_METHOD(CompareTwoDifferentFiles_ReturnsDifferent)
		{
            const auto expected_exit_code = 0;

            const auto file1 = _pattern_dir / "ha1.txt";
            const auto file2 = _pattern_dir / "haha.txt";

            assert(std::experimental::filesystem::exists(file1));
            assert(std::experimental::filesystem::exists(file2));
            assert(std::experimental::filesystem::is_regular_file(file1));
            assert(std::experimental::filesystem::is_regular_file(file2));

            const auto same = FilesAreTheSame(file1, file2);

            Assert::IsFalse(same);
		}

		TEST_METHOD(RunPing_CanGetTheOutput)
        {
            const auto cmd = L"ping.exe";

            auto output = std::make_shared<std::string>();
            auto error = std::make_shared<std::string>();
            const auto exit_code = RunCmd(cmd, [output](const char *bytes, size_t n) {
                *output += std::string(bytes, n);
            }, [error](const char *bytes, size_t n) {
                *error += std::string(bytes, n);
            });

            Assert::IsFalse(output->empty());

            output->clear();
            error->clear();

        }

		TEST_METHOD(RunFileCompare_CanGetTheOutput)
        {
            const auto cmd = L"FC.exe";

            auto output = std::make_shared<std::string>();
            auto error = std::make_shared<std::string>();
            const auto exit_code = RunCmd(cmd, [output](const char *bytes, size_t n) {
                *output += std::string(bytes, n);
            }, [error](const char *bytes, size_t n) {
                *error += std::string(bytes, n);
            });

            Assert::IsFalse(output->empty());

            output->clear();
            error->clear();

        }
	};
}