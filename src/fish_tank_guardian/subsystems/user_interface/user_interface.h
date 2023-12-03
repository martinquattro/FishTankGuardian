/*!****************************************************************************
 * @file    user_interface.h
 * @brief   UserInterface class header file.
 * @author  Quattrone Martin
 * @date    Oct 2023
 ******************************************************************************/

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define FEED_FEED_POSITION_X        11
#define FEED_FEED_POSITION_Y        1
#define TEMP_POSITION_X             6
#define TEMP_POSITION_Y             2
#define TDS_POSITION_X              5
#define TDS_POSITION_Y              3
#define TIME_POSITION_X             12
#define TIME_POSITION_Y             3

namespace Subsystems {

    class UserInterface 
    {
        public:

            //! Initializes the UserInterface.
            static void Init();

            /*!
            * @brief Gets the singleton instance of the UserInterface.
            * @return UserInterface* Pointer to the UserInterface instance.
            */
            static UserInterface* GetInstance();

            //! Updates the UserInterface.
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