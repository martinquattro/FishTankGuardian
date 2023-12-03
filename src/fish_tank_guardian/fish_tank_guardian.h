/*!****************************************************************************
 * @file    fish_tank_guardian.h
 * @brief   FishTankGuardian class header file.
 * @author  Quattrone Martin
 * @date    Oct 2023
 ******************************************************************************/

#ifndef FISH_TANK_GUARDIAN_H
#define FISH_TANK_GUARDIAN_H

#include "delay.h"

#define SYSTEM_TIME_INCREMENT_MS   1000

namespace Subsystems {

    class FishTankGuardian
    {
        public:

            /*!
            * @brief Initializes the FishTankGuardian subsystem.
            */
            static void Init();

            /*!
            * @brief Gets the singleton instance of the FishTankGuardian.
            * @return FishTankGuardian* Pointer to the FishTankGuardian instance.
            */
            static FishTankGuardian* GetInstance();

            /*!
            * @brief Updates the FishTankGuardian subsystem.
            */
            void Update();

        private:

            FishTankGuardian();
            ~FishTankGuardian() = default;
            FishTankGuardian(const FishTankGuardian&) = delete;
            FishTankGuardian& operator=(const FishTankGuardian&) = delete;

            static FishTankGuardian* mInstance;
            Util::Delay mDelay;
    };

} // namespace Subsystems

#endif // FISH_TANK_GUARDIAN_H
