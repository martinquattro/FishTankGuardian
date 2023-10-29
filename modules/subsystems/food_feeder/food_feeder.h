/*!****************************************************************************
 * @file food_feeder.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef FOOD_FEEDER_H
#define FOOD_FEEDER_H

namespace Subsystems { 

    class FoodFeeder 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the subsystem  object
            static FoodFeeder* GetInstance();

            //! Update subsystem
            void Update();

        private:

            FoodFeeder() {}
            ~FoodFeeder() = default;
            FoodFeeder(const FoodFeeder&) = delete;
            FoodFeeder& operator=(const FoodFeeder&) = delete;

            static FoodFeeder* mInstance;
    };

} // namespace Subsystems

#endif // FOOD_FEEDER_H