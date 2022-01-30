#pragma once
#include <SFML/System/Clock.hpp>

class Chronometer
{
public:
    /*!
     @brief Constructor

     @param initialTime Initial time elapsed
     */
    Chronometer(sf::Time initialTime = sf::Time::Zero)
    {
        reset();
        add(initialTime);
    }

    /*!
     @brief Add some time

     @param time Time to be added to the time elapsed
     @return Time elapsed
     */
    sf::Time add(sf::Time time)
    {
        m_time += time;

        if (m_state == STOPPED) m_state = PAUSED;

        return getElapsedTime();
    }

    /*!
     @brief Reset the chronometer

     @param start if true the chronometer automatically starts
     @return Time elapsed on the chronometer before the reset
     */
    sf::Time reset(bool start = false)
    {
        sf::Time time = getElapsedTime();

        m_time = sf::Time::Zero;
        m_state = STOPPED;

        if (start) resume();

        return time;
    }

    /*!
     @brief Pause the chronometer

     @return Time elapsed
     @see toggle
     */
    sf::Time pause()
    {
        if (isRunning())
        {
            m_state = PAUSED;
            m_time += m_clock.getElapsedTime();
        }
        return getElapsedTime();
    }

    /*!
     @brief Resume the chronometer

     @return Time elapsed

     @see toggle
     */
    sf::Time resume()
    {
        if (!isRunning())
        {
            m_state = RUNNING;
            m_clock.restart();
        }
        return getElapsedTime();
    }

    /*!
     @brief Pause or resume the chronometer

     If the chronometer is running the it is paused;
     otherwise it is resumes.

     @return Time elapsed
     @see pause
     @see resume
     */
    sf::Time toggle()
    {
        if (isRunning())    pause();
        else                resume();

        return getElapsedTime();
    }

    /*!
     @brief Tell the chronometer is running or not
     @brief chronometer's status
     */
    bool isRunning() const
    {
        return m_state == RUNNING;
    }

    /*!
     @brief Give the amount of time elapsed since the chronometer was started

     @return Time elapsed
     */
    sf::Time getElapsedTime() const
    {
        switch (m_state) {
        case STOPPED:
            return sf::Time::Zero;

        case RUNNING:
            return m_time + m_clock.getElapsedTime();

        case PAUSED:
            return m_time;
        }
    }

    /*!
     @brief Implicit conversion to sf::Time
     @return Time elapsed

     @see getElapsedTime
     */
    operator sf::Time() const
    {
        return getElapsedTime();
    }

private:
    enum { STOPPED, RUNNING, PAUSED } m_state;  //!< state
    sf::Time m_time;                            //!< time counter
    sf::Clock m_clock;                          //!< clock
};