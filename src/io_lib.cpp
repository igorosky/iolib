// Author: Igor Zaworski
#include <conio.h>
#include "../include/io_lib.h"

#ifdef IO_LIB_GLOBAL_IO
    IOLib io;
#endif

IOLib::IOLib() : _activated(false), _buffor(""), _bufforTmp(""), _asyncMode(false), _isInputPrinted(0), _isPromptPrinted(false), _outputEnabled(true),
    _carretPos(0), _inputHistorySize(-1), _inputFetched(0), _isIterValid(false), _insertMode(false), _commandPrompt("") { }

IOLib::~IOLib() {
    Deactivate();
}

void IOLib::Activate() {
    if(_activated) return;
    _outputThread = new std::thread(OutputThread(), this);
    _activated = true;
}

void IOLib::Deactivate(bool waitForProcessEnd) {
    if(!_activated) return;
    _activated = false;
    DisableAsyncMode(waitForProcessEnd);
    if(waitForProcessEnd)
        _outputThread->join();
    delete _outputThread;
}

void IOLib::DeleteInputField(const bool withPrompt, const std::size_t from) {
    if((!_isInputPrinted && !_isPromptPrinted) || from > _carretPos)
        return;
    if(withPrompt) {
        std::cout<<'\r';
        for(std::size_t i = 0; i <= _isInputPrinted; i++) std::cout<<' ';
        std::cout<<'\r';
        _isPromptPrinted = false;
        _isInputPrinted = 0;
        return;
    }
    for(std::size_t i = _carretPos; i < _buffor.size(); i++) std::cout<<' ';
    for(std::size_t i = _carretPos; i < _buffor.size(); i++) std::cout<<'\b';
    for(std::size_t i = _carretPos; i > from; i--) std::cout<<"\b \b";
    _isInputPrinted = from;
}

void IOLib::UpdateInputField(bool whole, const std::size_t from) {
    whole |= !_isPromptPrinted;
    if(whole) {
        DeleteInputField();
        std::cout<<_commandPrompt<<_buffor;
        _isPromptPrinted = true;
    }
    else if(from < _buffor.size()) {
        std::cout<<_buffor.substr(from, _buffor.size());
    }
    std::cout<<" \b";
    _isInputPrinted = _buffor.size();
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
    if(obj.eventType == SPECIAL_INPUT) {
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
            const auto tmp = _carretPos;
            _carretPos = _buffor.size();
            UpdateInputField(false, tmp);
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
    while(io->_activated) {
        if(io->isOuputEnabled() && io->_output.size()) {
            auto o = io->_output.front();
            io->_output.pop_front();
            if(o.eventType && !io->_asyncMode)
                continue;
            switch (o.eventType)
            {
            case SHOW_CARRET:
                if(io->_isPromptPrinted) break;
                std::cout<<io->_commandPrompt;
                io->_isPromptPrinted = true;
                break;
            case SPECIAL_INPUT:
            case INPUT:
                io->HandleInput(o);
                break;
            case COMMAND_PRINT:
                o.str = io->_commandPrompt + o.str;
            case PRINT:
                io->DeleteInputField();
                std::cout<<o.str;
                std::cout<<"\r\n";
                io->UpdateInputField();
                break;
            default:
                break;
            }
        }
    }
}

void IOLib::InputThread::operator()(IOLib *io) {
    io->_output.push_back({
        eventType: SHOW_CARRET,
    });
    while(io->isInAsyncMode())
        if(kbhit()) {
            auto inp = getch();
            EventType type = INPUT;
            if(inp == 224 || !inp) {
                inp = getch();
                type = SPECIAL_INPUT;
            }
            io->_output.push_back({
                input: inp,
                eventType: type,
            });
        }
}

void IOLib::AsyncMode(std::string commandPrompt) {
    _commandPrompt = commandPrompt;
    _buffor = "";
    _carretPos = 0;
    _asyncMode = true;
    _inputThread = new std::thread(InputThread(), this);
}

void IOLib::DisableAsyncMode(bool waitForProcessEnd) {
    if(!_asyncMode) return;
    _asyncMode = false;
    if(waitForProcessEnd)
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
        _output.push_back({
            str: ans,
            eventType: COMMAND_PRINT,
        });
    return ans;
}

bool IOLib::isInAsyncMode() const noexcept {
    return _asyncMode;
}

bool IOLib::isOuputEnabled() const noexcept {
    return _outputEnabled;
}

void IOLib::setCommandPrompt(const std::string commandPrompt) noexcept {
    _commandPrompt = commandPrompt;
}

std::string IOLib::getCommandPrompt() const noexcept {
    return _commandPrompt;
}

std::string IOLib::CombineStr(const std::string str, const PrintOptionList args) {
    std::string output = "";
    if(args.size()) {
        output = "\033[";
        for(std::size_t i = 0; i < args.size(); i++)
            output += ';' + std::to_string(args[i]);
        output += "m";
    }
    output += str + "\033[0m";
    return output;
}
