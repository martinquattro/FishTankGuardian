/*!****************************************************************************
 * @file fish_tank_guardian.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef FISH_TANK_GUARDIAN_H
#define FISH_TANK_GUARDIAN_H

#define SYSTEM_TIME_INCREMENT_MS   10

namespace Subsystems { 

    class FishTankGuardian 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the sensor object
            static FishTankGuardian* GetInstance();

            //!
            void Update();

        private:

            FishTankGuardian() {}
            ~FishTankGuardian() = default;
            FishTankGuardian(const FishTankGuardian&) = delete;
            FishTankGuardian& operator=(const FishTankGuardian&) = delete;

            static FishTankGuardian* mInstance;
    };

} // namespace Subsystems

#endif // FISH_TANK_GUARDIAN_H