// Author: Igor Zaworski
#include <conio.h>
#include "../include/io_lib.h"

#ifdef IO_LIB_GLOBAL_IO
    IOLib io;
#endif

IOLib::IOLib() : _activated(false), _buffor(""), _bufforTmp(""), _commandPrompt(""), _asyncMode(false), _isPromptPrinted(false),
    _outputEnabled(true), _carretPos(0), _inputHistorySize(-1), _inputFetched(0), _isIterValid(false), _insertMode(false) { }

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

void IOLib::PrintInputField() {
    std::cout<<_commandPrompt<<_buffor;
    SetCarretToCorrectSpot();
    _isPromptPrinted = true;
}

void IOLib::DeleteInputField() const {
    std::cout<<'\r';
    for(std::size_t i = 0; i < _buffor.size() + _commandPrompt.size(); i++) std::cout<<' ';
    std::cout<<'\r';
}

void IOLib::PrintOutput(const std::string &str) {
    if(!_outputEnabled) return;
    if(_asyncMode)
        DeleteInputField();
    std::cout<<str;
    std::cout<<"\r\n";
    if(_asyncMode)
        PrintInputField();
}

void IOLib::CommadPrint(const std::string &str) { 
    if(!_outputEnabled) return;
    if(_asyncMode)
        DeleteInputField();
    std::cout<<_commandPrompt<<str<<"\n\r";
    if(_asyncMode)
        PrintInputField();
}

void IOLib::ShowCarret() {
    if(_isPromptPrinted) return;
    std::cout<<_commandPrompt;
    _isPromptPrinted = true;
}

void IOLib::HandleInput(const char &c) {
    switch(c) {
    case 8:     //Backspace
        InputBackSpaceHandler();
        break;
    case 13:    //Enter
        InputEnterHandler();
        break;
    case 127:
            // Println("Deletev2");
        break;
    default:
        if(c >= 32) InputNewCharHandler(c);
    }
}

void IOLib::HandleSpecialInput(const char &c) {
    switch(c) {
        case 'H':
            // Println("Up Arrow");
            InputUpArrowHandler();
            break;
        case 'P':
            // Println("Down Arrow");
            InputDownArrowHandler();
            break;
        case 'M':
            // Println("Right Arrow");
            InputRightArrowHandler();
            break;
        case 'K':
            // Println("Left Arrow");
            InputLeftArrowHandler();
            break;
        case 82:
            // Println("Insert");
            InputInsertHandler();
            break;
        case 71:
            // Println("Home");
            InputHomeHandler();
            break;
        case 83:
            // Println("Delete");
            InputDeleteHandler();
            break;
        case 79:
            // Println("End");
            InputEndHandler();
            break;
        }
        return;
}

void IOLib::HandleEvent(const Event &event) {
    if(event.eventType && !_asyncMode) return;
    switch (event.eventType)
    {
    case SHOW_CARRET:
        ShowCarret();
        break;
    case SPECIAL_INPUT:
        HandleSpecialInput(event.input);
        break;
    case INPUT:
        HandleInput(event.input);
        break;
    case COMMAND_PRINT:
        CommadPrint(event.str);
        break;
    case PRINT:
        PrintOutput(event.str);
        break;
    default:
        break;
    }
}

void IOLib::OutputThread::operator()(IOLib *io) {
    while(io->_activated) {
        if(io->_events.empty()) continue;
        auto o = io->_events.front();
        io->_events.pop_front();
        io->HandleEvent(o);
    }
}

void IOLib::InputThread::operator()(IOLib *io) {
    io->_events.push_back({
        eventType: SHOW_CARRET,
    });
    while(io->isInAsyncMode()) {
        if(!kbhit()) continue;
        auto inp = getch();
        EventType type = INPUT;
        if(inp == 224 || !inp) {
            inp = getch();
            type = SPECIAL_INPUT;
        }
        io->_events.push_back({
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
        _events.push_back({
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

void IOLib::SetCarretToCorrectSpot() const noexcept {
    for(std::size_t i = _buffor.size(); i > _carretPos; i--) std::cout<<'\b';
}

void IOLib::SwapBufforTo(const std::string newBuffor) {
    std::cout<<'\r'<<_commandPrompt<<newBuffor;
    _carretPos = newBuffor.size();
    for(std::size_t i = _carretPos; i < _buffor.size(); i++) std::cout<<' ';
    for(std::size_t i = _carretPos; i < _buffor.size(); i++) std::cout<<'\b';
    _buffor = newBuffor;
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
    _inputScrollIter = _inputs.end();
}

void IOLib::InputNewCharHandler(const char &c) {
    std::cout<<(char)c;
    if(_insertMode) {
        _buffor[_carretPos++] = c;
        return;
    }
    _buffor.insert(_carretPos++, std::string(1, c));
    std::cout<<_buffor.substr(_carretPos, _buffor.size() - _carretPos);
    SetCarretToCorrectSpot();
}

void IOLib::InputBackSpaceHandler() {
    if(!_carretPos) return;
    _buffor.erase(--_carretPos, 1);
    std::cout<<'\b'<<_buffor.substr(_carretPos, _buffor.size() - _carretPos)<<" \b";
    SetCarretToCorrectSpot();
}

void IOLib::InputEndHandler() {
    std::cout<<_buffor.substr(_carretPos, _buffor.size() - _carretPos);
    _carretPos = _buffor.size();
}

void IOLib::InputHomeHandler() {
    _carretPos = 0;
    std::cout<<'\r'<<_commandPrompt;
}

void IOLib::InputInsertHandler() {
    // Println(std::string("Insert mode ") + (_insertMode ? "ON" : "OFF"));
    _insertMode = !_insertMode;
}

void IOLib::InputDeleteHandler() {
    _buffor.erase(_carretPos, 1);
    std::cout<<_buffor.substr(_carretPos, _buffor.size() - _carretPos)<<" \b";
    SetCarretToCorrectSpot();
}

void IOLib::InputUpArrowHandler() {
    if(_inputScrollIter == _inputs.begin()) return;
    if(_inputScrollIter-- == _inputs.end()) _bufforTmp = _buffor;
    SwapBufforTo(*_inputScrollIter);
}

void IOLib::InputDownArrowHandler() {
    if(_inputScrollIter == _inputs.end()) return;
    std::string newBuffor = _bufforTmp;
    if(++_inputScrollIter != _inputs.end()) newBuffor = *_inputScrollIter;
    SwapBufforTo(newBuffor);
}

void IOLib::InputRightArrowHandler() {
    if(_carretPos >= _buffor.size()) return;
    std::cout<<_buffor[_carretPos];
    _carretPos++;
}

void IOLib::InputLeftArrowHandler() {
    _carretPos--;
    std::cout<<'\b';
}
