#include <FireGL/Core/AssetPathManager.h>
#include <FireGL/Core/SystemManager.h>
#include <FireGL/Core/BaseLog.h>

namespace fgl
{

	AssetPathManager::AssetPathManager(std::string_view ConfigPath)
	{
		LoadConfig(ConfigPath);
		RegisterWithSystemManager();
	}

	void AssetPathManager::LoadConfig(std::string_view ConfigPath)
	{
		std::ifstream File(ConfigPath.data());
		if (!File)
		{
			LOG_ERROR("Failed to open configuration file: " + std::string(ConfigPath) + ". Please verify the path.", true)
			return;
		}
		m_ConfigPathPath = std::filesystem::path(ConfigPath).parent_path();

		std::string Line;
		while (std::getline(File, Line))
		{
			if (!Line.empty() && Line[0] != '[')
			{
				ParseLine(Line);
			}
		}
	}

	void AssetPathManager::ParseLine(const std::string& Line)
	{
		// Find the first '=' to split key-value pairs
		size_t Index = Line.find('=');
		if (Index == std::string::npos)
		{
			LOG_ERROR("Malformed line detected (missing '='): " + Line + ". Ensure the format is key=value.", true)
			return;
		}

		std::string Key = Line.substr(0, Index);
		std::string Value = ResolvePath(Line.substr(Index + 1));

		// Construct the full path by appending relative paths to base path
		std::filesystem::path ResolvedPath = m_ConfigPathPath / Value;
		if (!std::filesystem::exists(ResolvedPath))
		{
			LOG_ERROR("Path resolution failed for key '" + Key + "'. Resolved path does not exist: " + ResolvedPath.string(), true)
			return;
		}

		m_ConfigMap[Key] = ResolvedPath.string();
	}

	std::string AssetPathManager::ResolvePath(std::string_view Value) const
	{
		// Check if the value references another key before the first '/' (slash)
		size_t FirstSlashPos = Value.find('/');
		if (FirstSlashPos != std::string::npos)
		{
			std::string BaseKey = std::string(Value.substr(0, FirstSlashPos));
			auto It = m_ConfigMap.find(BaseKey);
			if (It != m_ConfigMap.end())
			{
				// Append the relative path to the resolved base path
				return It->second + std::string(Value.substr(FirstSlashPos));
			}
		}
		return std::string(Value);
	}

	std::string AssetPathManager::GetPath(std::string_view Key)
	{
		auto It = m_ConfigMap.find(Key.data());
		if (It == m_ConfigMap.end())
		{
			LOG_ERROR("Path retrieval failed. No entry found for key: '" + std::string(Key) + "'.", true)
			return {};
		}
		return It->second;
	}

	void AssetPathManager::RegisterWithSystemManager()
	{
		SystemManager<AssetPathManager>::Set(this);
	}

} // namespace fgl