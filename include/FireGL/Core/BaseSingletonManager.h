#pragma once

namespace fgl 
{

    /**
     * @class BaseSingletonManager
     * @brief Provides a base class for singleton managers within the application.
     *
     * This abstract class serves as a foundation for creating singleton manager classes,
     * such as InputManager or BaseWindow. It ensures that each manager is registered with
     * the SystemManager and is accessible globally within the application.
     *
     * @note This class cannot be instantiated directly. Instead, it must be derived by other
     *       singleton manager classes which will implement the required registration method.
     */
    class BaseSingletonManager
    {
    public:
        /** Virtual destructor to allow proper cleanup of derived classes. */
        virtual ~BaseSingletonManager() = default;

    protected:
        /**
         * @brief Registers the singleton instance with the SystemManager.
         *
         * This pure virtual function **must** be implemented in the first derived class
         * of any singleton manager (e.g., InputManager, BaseWindow, any custom ones...).
         * It ensures that there is only **one instance** of that class in the application,
         * and the instance can be accessed globally and safely from anywhere in your code
         * **after successful initialization**.
         *
         * The implementation of this function should call:
         *     SystemManager<YourDerivedClass>::Set(this);
         * where "YourDerivedClass" is the class you derived from the singleton manager,
         * and "this" refers to the current instance of the class.
         *
         * @note Failing to implement this function will prevent the derived class from being
         *       registered properly, leading to issues when trying to access the instance globally.
         *       You should use 'final' to ensure the derived method cannot be overridden in further derived classes.
         *       This is crucial as derived classes of the derived one shouldn't implement this function anymore.
         */
        virtual void RegisterWithSystemManager() = 0;
    };

} // namespace fgl