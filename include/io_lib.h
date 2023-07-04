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
        bool _asyncMode;
        bool _isInputPrinted;
        bool _outputEnabled;
        bool _killOutput;
        std::list<std::string> _inputs;

        struct InputThread {
            void operator()(IOLib*);
        };
        friend InputThread;
        std::thread *_inputThread;

        struct OutputThread {
            void operator()(IOLib*);
        };
        std::thread *_outputThread;

        struct PrintObj {
            std::string str = "";
            PrintOptionList args = {};
            int input = 0;
        };

        void _print(const std::string&, const PrintOptionList&);
        void _print(const std::string&, const unsigned short, va_list);
    protected:
        std::string _buffor;
        std::list<std::pair<std::vector<PrintObj>, int>> _output;
        std::vector<PrintObj> _commandPrompt;
        void _print(const std::vector<PrintObj>&) const;
        void UpdateInputField();
        void DeleteInputField();
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
