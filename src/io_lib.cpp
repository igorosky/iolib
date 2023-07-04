#include <conio.h>
#include "../include/io_lib.h"

IOLib io;

IOLib::IOLib() : _killOutput(false), _asyncMode(false), _outputEnabled(true), _isInputPrinted(false), _buffor(""), _carretPos(0), _inputHistorySize(-1),
    _isIterValid(false), _inputScrollIter(_inputs.end()), _insertMode(false), _bufforTmp("") {
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
    std::cout<<'\r';
    for(int i = 0; i <= _isInputPrinted; i++) std::cout<<' ';
    std::cout<<'\r';
    _isInputPrinted = 0;
}

void IOLib::UpdateInputField(bool whole, const std::size_t from) {
    whole |= !_isInputPrinted;
    if(whole) {
        DeleteInputField();
        _print(_commandPrompt);
        std::cout<<_buffor;
    }
    else if(from < _buffor.size()) {
        std::cout<<_buffor.substr(from, _buffor.size());
    }
    std::cout<<" \b";
    _isInputPrinted = _buffor.size() + _commandPrompt.size();
    for(std::size_t i = 0; i < _buffor.size() - _carretPos; i++) std::cout<<'\b';
}

void IOLib::InputEnterHandler() {
    if(_inputs.size() == _inputHistorySize) {
        _isIterValid = !(!_isIterValid || _inputs.begin() == _inputFetchIter);
        _inputs.pop_front();
    }
    _inputs.push_back(_buffor);
    DeleteInputField();
    _buffor = "";
    _bufforTmp = "";
    _carretPos = 0;
    UpdateInputField();
    _inputScrollIter = _inputs.end();
}

void IOLib::HandleInput(const Event &obj) {
    const auto &c = obj.input;
    if(obj.specialInput) {
        switch(c) {
        case 'H':
            // Println("Up Arrow");
            if(_inputScrollIter == _inputs.begin()) break;
            if(_inputScrollIter == _inputs.end()) _bufforTmp = _buffor;
            _inputScrollIter--;
            _buffor = *_inputScrollIter;
            _carretPos = _buffor.size();
            UpdateInputField();
            break;
        case 'P':
            // Println("Down Arrow");
            if(_inputScrollIter == _inputs.end()) break;
            if(++_inputScrollIter == _inputs.end()) _buffor = _bufforTmp;
            else _buffor = *_inputScrollIter;
            _carretPos = _buffor.size();
            UpdateInputField();
            break;
        case 'M':
            // Println("Right Arrow");
            if(_carretPos < _buffor.size()) {
                std::cout<<_buffor[_carretPos];
                _carretPos++;
            }
            break;
        case 'K':
            // Println("Left Arrow");
            _carretPos--;
            std::cout<<'\b';
            break;
        case 82:
            _insertMode = !_insertMode;
            // Println(std::string("Insert mode ") + (_insertMode ? "ON" : "OFF"));
            break;
        case 71:
            // Println("Home");
            while(_carretPos) {
                std::cout<<'\b';
                _carretPos--;
            }
            break;
        case 83:
            // Println("Delete");
            _buffor.erase(_carretPos, 1);
            UpdateInputField(false, _carretPos);
            break;
        case 79:
            // Println("End");
            _carretPos = _buffor.size();
            UpdateInputField(false);
            break;
        }
        return;
    }
    
    switch(c) {
    case 8:
        if(_carretPos) {
            _buffor.erase(--_carretPos, 1);
            std::cout<<'\b';
            UpdateInputField(false, _carretPos);
        }
        break;
    case 13:
        InputEnterHandler();
        break;
    case 127:
            // Println("Deletev2");
        break;
    default:
        if(c >= 32) {
            std::cout<<(char)c;
            if(_insertMode) {
                _buffor[_carretPos++] = c;
                break;
            }
            _buffor.insert(_carretPos++, std::string(1, c));
            UpdateInputField(false, _carretPos);
        }
    }
}

void IOLib::OutputThread::operator()(IOLib *io) {
    while(!io->toKillOutput()) {
        if(io->isOuputEnabled() && io->_output.size()) {
            const auto o = io->_output.front();
            io->_output.pop_front();
            if(o.second == 1) {
                io->HandleInput(o.first[0]);
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
        if(kbhit()) {
            auto inp = getch();
            bool special = false;
            if(inp == 224) {
                inp = getch();
                special = true;
            }
            io->_output.push_back({{{
                input: inp,
                specialInput: special,
            }}, 1});
        }
}

void IOLib::AsyncMode(std::string commandPrompt) {
    _commandPrompt = {{
        str: commandPrompt,
    }};
    _buffor = "";
    _carretPos = 0;
    _asyncMode = true;
    _inputThread = new std::thread(InputThread(), this);
}

void IOLib::DisableAsyncMode() {
    if(!_asyncMode) return;
    _asyncMode = false;
    _inputThread->join();
    delete _inputThread;
    DeleteInputField();
}

std::size_t IOLib::InputCount() const noexcept {
    return _inputs.size() - _inputFetched;
}

std::size_t IOLib::TotalInputCount() const noexcept {
    return _inputs.size();
}

std::string IOLib::GetLastInput(bool silent) noexcept {
    if(!InputCount())
        return "";
    if(!_isIterValid) {
        _isIterValid = true;
        _inputFetchIter = _inputs.begin();
    }
    else
        _inputFetchIter++;
    auto ans = *_inputFetchIter;
    _inputFetched++;
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

void IOLib::_print(const std::vector<Event> &print) const {
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

void IOLib::setCommandPrompt(const std::string cp) {
    _commandPrompt = {{str: cp}};
    UpdateInputField();
}

std::string IOLib::getCommandPrompt() const noexcept {
    std::string ans = "";
    for(const auto& i : _commandPrompt) {
        ans += i.str;
    }
    return ans;
}
