#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Core/BaseSingletonManager.h>

#include <filesystem>

namespace fgl
{
	/**
	 * Manages asset paths from a configuration (.ini) file.
	 *
	 * This class loads, parses, and resolves paths defined in an .ini file.
	 * The paths can reference each other, creating a system for structured asset organization.
	 *
	 * Example:
	 * - Assets=Content/Assets
	 * - Models=Assets/Models
	 * In this example, "Models" will resolve to "Content/Assets/Models".
	 *
	 * The file is parsed from top to bottom, so referenced keys must be defined beforehand.
	 *
	 * This class is part of a singleton-based system and is not intended to be derived from.
	 * It serves as a blueprint for implementing asset path management systems.
	 */
	class AssetPathManager : public BaseSingletonManager
	{
	public:
		/**
		 * Constructs the AssetPathManager and loads the configuration file.
		 *
		 * @param ConfigPath The path to the .ini file to load.
		 */
		AssetPathManager(std::string_view ConfigPath);

		/**
		 * @brief Retrieves the resolved path for a given key.
		 *
		 * Looks up and returns the path associated with the specified key. If the key is not found,
		 * the function will return an empty string and cause the application to crash.
		 *
		 * @param Key The key to look up.
		 * @return The resolved path corresponding to the key, or an empty string if the key is not found (results in a crash).
		 */
		std::string GetPath(std::string_view Key);
	
	protected:
		/**
		 * Registers the manager with the system (singleton pattern).
		 * This function ensures the manager is properly initialized within the system.
		 */
		virtual void RegisterWithSystemManager() override final;
	
	private:
		/**
		 * Loads and parses the configuration file to populate the internal path map.
		 *
		 * @param ConfigPath The path to the configuration file.
		 */
		void LoadConfig(std::string_view ConfigPath);

		/**
		 * Parses an individual line from the configuration file.
		 *
		 * @param Line A line from the config file in the format "key=value".
		 */
		void ParseLine(const std::string& Line);

		/**
		 * Resolves paths by replacing predefined keys with their corresponding values.
		 * If the value starts with a key followed by '/', it replaces the key with the associated path.
		 *
		 * @param Value The value to resolve (can reference other paths).
		 * @return The fully resolved path.
		 */
		std::string ResolvePath(std::string_view Value) const;

	private:
		std::unordered_map<std::string, std::string> m_ConfigMap;  ///< Stores key-value pairs from the .ini file.
		std::filesystem::path m_ConfigPathPath;					   ///< Directory path of the loaded config file (used for relative path resolution).
	};

}