#include <gtest/gtest.h>
#include <cassert>

#include <filesystem>

#include "../../../process.hpp"

namespace WinBatchTests
{
    class WinBatchTests: public testing::Test
    {
	protected:
        std::filesystem::path _sut;
        std::filesystem::path _pattern_dir;

        void SetUp() override
        {
            _pattern_dir = L"../../tests/patterns/";

            assert(std::filesystem::is_directory(_pattern_dir));
            assert(std::filesystem::exists(_pattern_dir));
        }

        _Check_return_ int RunExitCodeBatch (_In_ int exit_code_to_be_returned) const
        {
            auto batch_file = _pattern_dir / "exit_code.bat";
            assert(std::filesystem::is_regular_file(batch_file));
            assert(std::filesystem::exists(batch_file));

            const auto cmd = batch_file.wstring() + L" " + std::to_wstring(exit_code_to_be_returned);

            return RunCmd(cmd);
        }

        static int RunCmd(_In_ const std::wstring& cmd,
                          _In_ std::function<void(const char *bytes, size_t n)> read_stdout=nullptr,
                          _In_ std::function<void(const char *bytes, size_t n)> read_stderr=nullptr)
        {
            TinyProcessLib::Process proc(cmd, L"", read_stdout, read_stderr);
            return proc.get_exit_status();
        }

    };

    TEST_F(WinBatchTests, BatchFileExits0_CanGetExitCode)
    {
        const auto expected_exit_code = 0;

        const auto actual_exit_code = RunExitCodeBatch(expected_exit_code);

        EXPECT_EQ(expected_exit_code, actual_exit_code);
    }

    TEST_F(WinBatchTests, BatchFileExits1_CanGetExitCode)
    {
        const auto expected_exit_code = 1;

        const auto actual_exit_code = RunExitCodeBatch(expected_exit_code);

        EXPECT_EQ(expected_exit_code, actual_exit_code);
    }

    TEST_F(WinBatchTests, BatchFileExitsMinus1_CanGetExitCode)
    {
        const auto expected_exit_code = -1;

        const auto actual_exit_code = RunExitCodeBatch(expected_exit_code);

        EXPECT_EQ(expected_exit_code, actual_exit_code);
    }

    TEST_F(WinBatchTests, EchoToStdOut_CanGetEchoString)
    {
        using namespace std::string_literals;

        const auto string_to_echo  = L"Microchip"s;
        const auto string_expected =  "Microchip\r\n";
        auto output = std::make_shared<std::string>();
        auto error = std::make_shared<std::string>();

        const auto cmd = L"CMD /C echo "s + string_to_echo;
        const auto exit_code = RunCmd(cmd, [output](const char *bytes, size_t n) {
                *output += std::string(bytes, n);
            }, [error](const char *bytes, size_t n) {
                *error += std::string(bytes, n);
            });

        EXPECT_EQ(string_expected, *output);
        EXPECT_EQ(EXIT_SUCCESS, exit_code);
    }
}