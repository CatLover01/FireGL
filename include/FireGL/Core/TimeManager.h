#pragma once

#include <FireGL/Core/BaseSingletonManager.h>

namespace fgl 
{

    /**
     * @class TimeManager
     *
     * @brief Manages time-related operations for the application, including deltaTime, total elapsed time,
     *        and frame-independent calculations. This class provides utilities for tracking time between
     *        frames and total application runtime, which is essential for smooth, frame-rate independent logic
     *        in games and other time-dependent applications.
     *
     * @note The class is designed to provide basic time tracking functionality and can be extended if additional
     *       time-related features are needed in your application.
     */
    class TimeManager : public BaseSingletonManager 
    {
    public:
        /**
         * Sets deltaTime and last frame to zero
         * Must be called by derived classes to ensure proper initialization.
         */
        TimeManager();

        /**
         * Updates deltaTime each frame.
         * This function must be called within the game loop to calculate the time difference
         * between the current and previous frames. It is used for frame-independent calculations.
         */
        void Update();

        /**
         * Retrieves the deltaTime (time difference between the current and previous frame).
         * This value is useful for performing frame-independent logic (e.g., movement, animations).
         *
         * @return The time difference between the current and previous frame.
         */
        float GetDeltaTime();

        /**
         * Retrieves the total time elapsed since the application started.
         *
         * @return The total time in seconds.
         *         A return value of 0 indicates an error with glfwGetTime().
         */
        double GetTimeSeconds();

        /**
         * This function must be called immediately after the constructor is invoked.
         * It is used to call functions that are mostly virtual, as they cannot be called
         * directly from the constructor.
         */        
        void Initialize();

    protected:
        /**
         * Custom initialization logic to be executed after the TimeManager is constructed.
         * This is a protected method that can be overridden by derived classes to add extra initialization.
         */
        virtual void OnInitialized();

        /**
         * Registers the TimeManager instance with the SystemManager for centralized management.
         * This is called by the Initialize method to ensure that the TimeManager is registered
         * in the system and properly handled during its lifecycle.
         */
        virtual void RegisterWithSystemManager() override final;

    private:
        float m_DeltaTime;  ///< The deltaTime between the current and previous frame.
        double m_LastFrame; ///< Stores the time of the previous frame to calculate the DeltaTime between frames.
    };

} // namespace fgl