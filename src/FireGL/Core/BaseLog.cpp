#include <FireGL/Core/BaseLog.h>
#include <stdexcept>
#include <chrono>
#include <iomanip>

namespace fgl {

	namespace Log {

		std::string GetTimestamp() {
			auto Now = std::chrono::system_clock::now();
			auto TimeTNow = std::chrono::system_clock::to_time_t(Now);

			std::tm LocalTime;
#if defined(_WIN32) || defined(_WIN64)
			localtime_s(&LocalTime, &TimeTNow);
#else
			localtime_r(&TimeTNow, &LocalTime);
#endif

			std::ostringstream Oss;
			Oss << std::put_time(&LocalTime, "[%Y-%m-%d %H:%M:%S]");
			return Oss.str();
		}

		void Log(std::string_view Message)
		{
			std::cout << GetTimestamp() << " [INFO]: " << Message << std::endl;
		}

		void Error(std::string_view ErrorMessage, const char* File, int Line, bool ShoudThrow)
		{
			std::cerr << GetTimestamp() << " [ERROR] (" << File << ":" << Line << "): \n" << ErrorMessage << "\n----------------" << std::endl;
			if (ShoudThrow)
			{
				throw std::runtime_error(ErrorMessage.data());
			}
		}

		void Assert(bool Condition, std::string_view AssertMessage, const char* File, int Line, std::function<void()> Callback)
		{
			if (!Condition)
			{
				std::cerr << GetTimestamp() << " [ASSERTION FAILED] (" << File << ":" << Line << "): \n" << AssertMessage << "\n----------------" << std::endl;
				if (Callback)
				{
					Callback();
				}
				throw std::runtime_error(AssertMessage.data());
			}
		}

	}

} // namespace fgl