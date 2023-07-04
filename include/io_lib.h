// Author: Igor Zaworski
#ifndef IO_LIB
    #define IO_LIB

#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <cstdarg>
#include "print_options.h"

typedef std::vector<PrintOptions> PrintOptionList;

class IOLib {
    private:
        std::string _buffor;
        std::string _bufforTmp;
        bool _asyncMode;
        std::size_t _isInputPrinted;
        bool _isPromptPrinted;
        bool _outputEnabled;
        bool _killOutput;
        std::size_t _carretPos;
        std::list<std::string> _inputs;
        std::size_t _inputHistorySize;
        std::size_t _inputFetched;
        bool _isIterValid;
        std::list<std::string>::iterator _inputFetchIter;
        std::list<std::string>::iterator _inputScrollIter;
        bool _insertMode;

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
        };
        std::list<Event> _output;
        std::string _commandPrompt;

        void _print(const std::string&, const PrintOptionList&);
        void _print(const std::string&, const unsigned short, va_list);
        void InputEnterHandler();
        /**
         * @param whole
         * @param from if not whole
        */
        void UpdateInputField(bool = true, const std::size_t = 0);
        void DeleteInputField(const bool = true, const std::size_t from = 0);
        void HandleInput(const Event&);
        bool isOuputEnabled() const noexcept;
        bool toKillOutput() const noexcept;
    public:
        IOLib();
        ~IOLib();
        /**
         * Turns on asynchronous input
         * @param CommandPrompt default: "> "
        */
        void AsyncMode(std::string = "> ");
        /**
         * Turns off asynchronous input
        */
        void DisableAsyncMode();
        /**
         * Inputs ready to fetch count
        */
        std::size_t InputCount() const noexcept;
        std::size_t TotalInputCount() const noexcept;
        /**
         * @param silent if input should be printed during fetching
        */
        std::string GetLastInput(bool = false) noexcept;
        bool isInAsyncMode() const noexcept;
        void setCommandPrompt(const std::string) noexcept;
        std::string getCommandPrompt() const noexcept;

        static std::string CombineStr(const std::string, const PrintOptionList = {});
        
        #pragma region Prints

        /**
         * Go to new line
        */
        void Println();

        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param n options count
         * @param options n options of printing
        */
        void Println(const std::string, const unsigned short = 0, ...);

        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param options options of printing
        */
        void Println(const std::string, const PrintOptionList);



        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param n options count
         * @param options n options of printing
        */
        void Println(const char, const unsigned short = 0, ...);

        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param options options of printing
        */
        void Println(const char, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const unsigned char, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const unsigned char, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const unsigned short, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const unsigned short, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const short, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const short, const PrintOptionList);



        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param n options count
         * @param options n options of printing
        */
        void Println(const int, const unsigned short = 0, ...);

        /**
         * Prints str and goes to new line
         * @param str value to print
         * @param options options of printing
        */
        void Println(const int, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const unsigned int, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const unsigned int, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const long, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const long, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const unsigned long, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const unsigned long, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const long long, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const long long, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const unsigned long long, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const unsigned long long, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const float, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const float, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const double, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const double, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const long double, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const long double, const PrintOptionList);



        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param n options count
        //  * @param options n options of printing
        // */
        // void Println(const bool, const unsigned short = 0, ...);

        // /**
        //  * Prints str and goes to new line
        //  * @param str value to print
        //  * @param options options of printing
        // */
        // void Println(const bool, const PrintOptionList);

        #pragma endregion
};

#ifndef IO_LIB_NO_GLOBAL_IO
    #define IO_LIB_GLOBAL_IO
    extern IOLib io;
#endif

#endif
