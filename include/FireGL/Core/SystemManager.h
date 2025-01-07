#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Core/BaseLog.h>
#include <FireGL/Core/BaseSingletonManager.h>

#include <typeindex>

namespace fgl 
{
    
    /**
     * @class SystemManager
     * @brief Manages instances of singleton managers within the application.
     *
     * This templated class provides functionality to store and retrieve singleton manager
     * instances, ensuring that each type of manager has only one instance that can be
     * accessed globally and safely from anywhere in the application.
     *
     * @tparam T The type of the singleton manager being managed.
     *
     * @note This class utilizes polymorphic behavior to store and retrieve instances
     *       of derived singleton managers.
     */
    template <typename T>
    class SystemManager {
    public:
        /**
         * @brief Registers the singleton instance with the SystemManager.
         *
         * This function stores the provided instance of a singleton manager, allowing
         * it to be accessed globally within the application. The instance is stored as a
         * BaseSingletonManager* to support polymorphic behavior.
         *
         * @param Instance The instance of the singleton manager to be registered.
         *
         * @note This function should be called from the RegisterWithSystemManager method
         *       of the derived singleton manager class.
         */
        static void Set(BaseSingletonManager* Instance) 
        {
            m_Instances[std::type_index(typeid(T))] = Instance;
        }

        /**
         * @brief Retrieves the singleton instance of the specified type.
         *
         * @return A pointer to the singleton instance of the specified type, or nullptr
         *         if the instance has not been registered.
         */
        static T* Get() 
        {
            auto It = m_Instances.find(std::type_index(typeid(T)));
            if (It != m_Instances.end()) {
                return static_cast<T*>(It->second);
            }
            std::string ErrorMessage = "Failed to retrieve singleton instance. Singleton of type [" + std::string(typeid(T).name()) + "] is not registered.";
            LOG_ERROR(ErrorMessage, false);
            return nullptr;
        }

    private:
        /**
         * @brief Stores the singleton instances with their type information.
         *
         * This unordered map holds instances of singleton managers, indexed by their
         * type information. This ensures that each type of singleton manager has only
         * one instance.
         *
         * @note The use of std::type_index and std::unordered_map allows for polymorphic behavior and ensures
         *       efficient O(1) access time to the stored instances, providing fast lookups.
         */
        static inline std::unordered_map<std::type_index, BaseSingletonManager*> m_Instances;
    };

} // namespace fgl