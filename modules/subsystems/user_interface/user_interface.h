/*!****************************************************************************
 * @file user_interface.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

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