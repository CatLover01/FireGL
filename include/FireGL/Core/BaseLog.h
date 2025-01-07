#pragma once

#include <FireGL/fglpch.h>

namespace fgl 
{

#define LOG_INFO(Message) Log::Log(Message);
#define LOG_ERROR(ErrorMessage, ShouldThrow) Log::Error(ErrorMessage, __FILE__, __LINE__, ShouldThrow);
#define LOG_ASSERT(Condition, AssertMessage) Log::Assert(Condition, AssertMessage, __FILE__, __LINE__);
#define LOG_ASSERT_CALLBACK(Condition, AssertMessage, Callback) Log::Assert(Condition, AssertMessage, __FILE__, __LINE__, Callback);

	namespace Log {

		/**
		 * @brief Logs a simple informational message.
		 *
		 * This function is called via the `LOG_INFO` macro. It writes an informational message to the log.
		 * It is typically used for general information or debugging output that does not indicate an error or failure.
		 */
		void Log(std::string_view Message);

		/**
		 * @brief Logs an error message.
		 *
		 * This function is called via the `LOG_ERROR` macro. It logs an error message, including the source
		 * file and line number where the error occurred. Optionally, it can throw an exception based on the
		 * `ShouldThrow` parameter.
		 *
		 * @param File The source file where the error occurred.
		 * @param Line The line number where the error occurred.
		 * @param ShouldThrow A flag indicating whether to throw a runtime exception after logging.
		 */
		void Error(std::string_view ErrorMessage, const char* File, int Line, bool ShoudThrow);

		/**
		 * @brief Asserts a condition and logs an assertion message.
		 *
		 * This function is called via the `LOG_ASSERT` or `LOG_ASSERT_CALLBACK` macro. It checks a condition,
		 * and if the condition is false, it logs an assertion failure message along with the file and line
		 * number where the assertion failed. Optionally, a callback function can be provided that will be
		 * executed if the assertion fails.
		 *
		 * @param Condition The condition to check.
		 * @param AssertMessage The message to log if the assertion fails.
		 * @param File The source file where the assertion occurred.
		 * @param Line The line number where the assertion occurred.
		 * @param Callback An optional callback function to execute if the assertion fails.
		 */
		void Assert(bool Condition, std::string_view AssertMessage, const char* File, int Line, std::function<void()> Callback = nullptr);

		/**
		 * @brief Retrieves the current timestamp.
		 *
		 * This function generates and returns a string representing the current timestamp,
		 * which is typically used to annotate log entries with the time they occurred.
		 *
		 * @return A string representing the current timestamp.
		 */
		std::string GetTimestamp();
	}

} // namespace fgl