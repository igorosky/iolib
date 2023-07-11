// Author: Igor Zaworski
#ifndef IO_LIB
    #define IO_LIB

#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include "print_options.h"

typedef std::vector<PrintOptions> PrintOptionList;

class IOLib {
    private:
        std::string _buffor;
        std::string _bufforTmp;
        std::string _commandPrompt;
        bool _asyncMode;
        bool _isPromptPrinted;
        bool _outputEnabled;
        std::size_t _carretPos;
        std::list<std::string> _inputs;
        std::size_t _inputFetched;
        bool _isIterValid;
        std::list<std::string>::iterator _inputFetchIter;
        std::list<std::string>::iterator _inputScrollIter;
        bool _insertMode;
        // unsigned long long _bufforState;
        // std::string _targetBuffor;
        // std::size_t _targetCarretPos;
        // unsigned long long _targetBufforState;

        struct InputThread {
            void operator()(IOLib*);
        };
        std::thread *_inputThread;

        struct OutputThread {
            void operator()(IOLib*);
        };
        std::thread *_outputThread;

        enum EventType {
            PRINT = 0,
            INPUT = 1,
            SPECIAL_INPUT = 2,
            COMMAND_PRINT = 3,
            SHOW_CARRET = 4,
        };

        struct Event {
            std::string str = "";
            int input = 0;
            EventType eventType = PRINT;
            // unsigned long long state;
        };
        std::list<Event> _events;
        void HandleEvent(const Event&);
        
       /**
        * Prints input field with Command Prompt, with buffor and sets cursor to correct spot
       */
        void PrintInputField();
        /**
         * Deletes input field with Command Prompt
        */
        void DeleteInputField() const;
        void PrintOutput(const std::string&);
        void CommadPrint(const std::string &str);
        void ShowCarret();
        void HandleInput(const char&);
        void HandleSpecialInput(const char&);
        /**
         * Sets carret to _carretPos (its must be at the end of buffor)
        */
        void SetCarretToCorrectSpot() const noexcept;
        void SwapBufforTo(const std::string);
        void InputNewCharHandler(const char&);
        void InputEnterHandler();
        void InputBackSpaceHandler();
        void InputEndHandler();
        void InputHomeHandler();
        void InputInsertHandler();
        void InputDeleteHandler();
        void InputUpArrowHandler();
        void InputDownArrowHandler();
        void InputRightArrowHandler();
        void InputLeftArrowHandler();

        // void UpdateTargetBuffor(const char&, const EventType&);
        // void InputTargetEnterHandler();
    public:
        unsigned short decimalPrecision;
        std::size_t inputHistorySize;
        
        IOLib();
        ~IOLib();
        /**
         * Turns on asynchronous input
         * @param CommandPrompt default: "> "
        */
        void AsyncMode(std::string = "> ");
        /**
         * Turns off asynchronous input
         * @param waitForProcessEnd
        */
        void DisableAsyncMode(bool = true);
        /**
         * Inputs ready to fetch count
        */
        std::size_t InputCount() const noexcept;
        std::size_t TotalInputCount() const noexcept;
        /**
         * @param silent if input should be printed during fetching
        */
        std::string GetLastInput(bool = false) noexcept;
        std::string PeekLastInput() const noexcept;
        bool isInAsyncMode() const noexcept;
        void setCommandPrompt(const std::string) noexcept;
        std::string getCommandPrompt() const noexcept;

        template <typename T>
        inline std::string CombineStr(T str, const PrintOptionList args = {}) {
            std::string output = "";
            if(args.size()) {
                output = "\033[";
                for(std::size_t i = 0; i < args.size(); i++)
                    output += ';' + std::to_string(args[i]);
                output += "m";
            }
            output += toString(str) + "\033[0m";
            return output;
        }

        void EnableOutput() noexcept;
        void DisableOutput() noexcept;
        bool isOuputEnabled() const noexcept;

        template <typename T>
        std::string toString(T);
        // template <typename Y, template <typename> class T>
        // std::string toString(T<Y>);
        // template <typename Y, typename U, template <typename, typename> class T>
        // std::string toString(T<Y, U>);

        /**
         * Prints str (in async mode works same as Println)
         * @param str value to print
         * @param options options of printing
        */
        template <typename T = std::string>
        inline void Print(T str = "", const PrintOptionList args = {}) {
            if(_asyncMode)
                _events.push_back({
                    str: CombineStr(str, args),
                });
            else
                std::cout<<CombineStr(str, args);
        }

        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param options options of printing
        */
        template <typename T = std::string>
        inline void Println(T str = "", const PrintOptionList args = {}) {
            if(_asyncMode)
                _events.push_back({
                    str: CombineStr(str, args),
                });
            else
                std::cout<<CombineStr(str, args)<<"\n\r";
        }
};

#ifndef IO_LIB_NO_GLOBAL_IO
    extern IOLib io;
    #define p io.CombineStr
#endif


#endif
