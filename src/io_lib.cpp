#include <conio.h>
#include "../include/io_lib.h"

IOLib io;

IOLib::IOLib() : _killOutput(false), _asyncMode(false), _outputEnabled(true), _isInputPrinted(false), _buffor("") {
    _outputThread = new std::thread(OutputThread(), this);
}

IOLib::~IOLib() {
    DisableAsyncMode();
    _killOutput = true;
    _outputThread->join();
    delete _outputThread;
}

void IOLib::DeleteInputField() {
    if(!_isInputPrinted)
        return;
    _isInputPrinted = false;
    std::cout<<'\r';
    for(int i = 0; i <= _buffor.size() + _commandPrompt.size(); i++) std::cout<<' ';
    std::cout<<'\r';
}

void IOLib::UpdateInputField() {
    DeleteInputField();
    _print(_commandPrompt);
    std::cout<<_buffor;
    _isInputPrinted = true;
}

void IOLib::OutputThread::operator()(IOLib *io) {
    while(!io->toKillOutput()) {
        if(io->isOuputEnabled() && io->_output.size()) {
            const auto o = io->_output.front();
            io->_output.pop_front();
            if(o.second == 1) {
                const auto &c = o.first[0].input;
                switch(c) {
                    case 8:
                        if(io->_buffor.size()) {
                            io->_buffor.pop_back();
                            std::cout<<"\b \b";
                        }
                        break;
                    case 13:
                        io->_inputs.push_back(io->_buffor);
                        io->DeleteInputField();
                        io->_buffor = "";
                        io->UpdateInputField();
                        break;
                    case 127:
                        break;
                    default:
                        if(c >= 32) {
                            io->_buffor.push_back(c);
                            std::cout<<(char)c;
                        }
                }
                continue;
            }
            io->DeleteInputField();
            if(o.second == 2)
                io->_print(io->_commandPrompt);
            io->_print(o.first);
            std::cout<<"\r\n";
            io->UpdateInputField();
        }
    }
}

void IOLib::InputThread::operator()(IOLib *io) {
    while(io->isInAsyncMode())
        if(_kbhit())
            io->_output.push_back({{{input: _getch()}}, 1});
}

void IOLib::AsyncMode(std::string commandPrompt) {
    _commandPrompt = {{
        str: commandPrompt,
    }};
    _buffor = "";
    _asyncMode = true;
    _inputThread = new std::thread(InputThread(), this);
}

void IOLib::DisableAsyncMode() {
    _asyncMode = false;
    _inputThread->join();
    delete _inputThread;
    DeleteInputField();
}

std::size_t IOLib::InputCount() const noexcept {
    return _inputs.size();
}

std::string IOLib::GetLastInput(bool silent) noexcept {
    if(_inputs.empty())
        return "";
    auto ans = _inputs.front();
    _inputs.pop_front();
    if(!silent)
        _output.push_back({{{
            str: ans,
        }}, 2});
    return ans;
}

bool IOLib::isInAsyncMode() const noexcept {
    return _asyncMode;
}

bool IOLib::isOuputEnabled() const noexcept {
    return _outputEnabled;
}

bool IOLib::toKillOutput() const noexcept {
    return _killOutput;
}

void IOLib::_print(const std::vector<PrintObj> &print) const {
    for(int h = 0; h < print.size(); h++) {
        const auto& str = print[h].str;
        const auto& args = print[h].args;
        std::string output = "";
        if(args.size()) {
            output = "\033[";
            for(int i = 0; i < args.size(); i++)
                output += ';' + std::to_string(args[i]);
            output += "m";
        }
        output += str + "\033[0m";
        std::cout<<output;
    }
}
