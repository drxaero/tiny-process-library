#include <gtest/gtest.h>

#include <Windows.h>

#include <experimental/filesystem>
using namespace std::experimental::filesystem::v1; 

#include "../../../process.hpp"

namespace WinExeTests
{
    class WinExeTests : public testing::Test
    {
    protected:
        std::experimental::filesystem::path _sut;
        std::experimental::filesystem::path _pattern_dir;

        void SetUp() override
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
    };

    // TODO: <Janus>2018/06/03: Fix the failure of this test. This Google test method is the same as MsTest's WinExeMsTest::CompareTwoIdenticalFiles_ReturnsTheSame().
    //                          But MsTest is ok, but Google Test failed. What's wrong with Google test?
    TEST_F(WinExeTests, CompareTwoIdenticalFiles_ReturnsTheSame)
    {
        const auto file1 = _pattern_dir / "ha1.txt";
        const auto file2 = _pattern_dir / "ha2.txt";

        assert(std::experimental::filesystem::exists(file1));
        assert(std::experimental::filesystem::exists(file2));
        assert(std::experimental::filesystem::is_regular_file(file1));
        assert(std::experimental::filesystem::is_regular_file(file2));

        const auto same = FilesAreTheSame(file1, file2);

        EXPECT_TRUE(same);
    }

    TEST_F(WinExeTests, CompareTwoDifferentFiles_ReturnsDifferent)
    {
        const auto file1 = _pattern_dir / "ha1.txt";
        const auto file2 = _pattern_dir / "haha.txt";

        assert(std::experimental::filesystem::exists(file1));
        assert(std::experimental::filesystem::exists(file2));
        assert(std::experimental::filesystem::is_regular_file(file1));
        assert(std::experimental::filesystem::is_regular_file(file2));

        const auto same = FilesAreTheSame(file1, file2);

        EXPECT_FALSE(same);
    }

    TEST_F(WinExeTests, RunPing_CanGetTheOutput)
    {
        const auto cmd = L"ping.exe";

        auto output = std::make_shared<std::string>();
        auto error = std::make_shared<std::string>();
        const auto exit_code = RunCmd(cmd, [output](const char *bytes, size_t n) {
            *output += std::string(bytes, n);
        }, [error](const char *bytes, size_t n) {
            *error += std::string(bytes, n);
        });

        EXPECT_FALSE(output->empty());

        output->clear();
        error->clear();
    }

    TEST_F(WinExeTests, RunFileCompare_CanGetTheOutput)
    {
        const auto cmd = L"FC.exe";

        auto output = std::make_shared<std::string>();
        auto error = std::make_shared<std::string>();
        const auto exit_code = RunCmd(cmd, [output](const char *bytes, size_t n) {
            *output += std::string(bytes, n);
        }, [error](const char *bytes, size_t n) {
            *error += std::string(bytes, n);
        });

        EXPECT_FALSE(output->empty());

        output->clear();
        error->clear();
    }
}
