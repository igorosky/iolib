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
        // second = true guarantees vector.size() = 1
        bool _asyncMode;
        std::size_t _isInputPrinted;
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
        // friend InputThread;
        std::thread *_inputThread;

        struct OutputThread {
            void operator()(IOLib*);
        };
        std::thread *_outputThread;

        struct Event {
            std::string str = "";
            PrintOptionList args = {};
            int input = 0;
            bool specialInput = false;
        };
        std::list<std::pair<std::vector<Event>, int>> _output;
        std::vector<Event> _commandPrompt;

        void _print(const std::string&, const PrintOptionList&);
        void _print(const std::string&, const unsigned short, va_list);
        void InputEnterHandler();
        void _print(const std::vector<Event>&) const;
        /**
         * @param whole
         * @param from if not whole
        */
        void UpdateInputField(bool = true, const std::size_t = 0);
        void DeleteInputField();
        void HandleInput(const Event&);
        bool isOuputEnabled() const noexcept;
        bool toKillOutput() const noexcept;
    public:
        IOLib();
        ~IOLib();
        void AsyncMode(std::string = "> ");
        void DisableAsyncMode();
        std::size_t InputCount() const noexcept;
        /**
         * @param silent
        */
        std::string GetLastInput(bool = false) noexcept;
        bool isInAsyncMode() const noexcept;
        void setCommandPrompt(const std::string);
        std::string getCommandPrompt() const noexcept;
        
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

extern IOLib io;

#endif
