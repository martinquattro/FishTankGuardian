/*!****************************************************************************
 * @file user_interface.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define FEED_FEED_POSITION_X        11
#define FEED_FEED_POSITION_Y        1

#define TEMP_POSITION_X             6
#define TEMP_POSITION_Y             2

#define TDS_POSITION_X              5
#define TDS_POSITION_Y              3

#define TIME_POSITION_X             15
#define TIME_POSITION_Y             3

namespace Subsystems { 

    class UserInterface 
    {
        public:

            //! Initialize the subsystem
            static void Init();

            //! Returns the subsystem object
            static UserInterface* GetInstance();

            //! Update subsystem
            void Update();

        private:

            UserInterface() {}
            ~UserInterface() = default;
            UserInterface(const UserInterface&) = delete;
            UserInterface& operator=(const UserInterface&) = delete;

            static UserInterface* mInstance;
    };

} // namespace Subsystems

#endif // USER_INTERFACE_H