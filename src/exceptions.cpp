#include "exceptions.h"

C_E::C_E() {
    exc = E_None;
    param = "";
}

C_E::C_E(int exc) {
    this->exc = exc;
    param = "";
}

C_E::C_E(int exc, const char* param) {
    this->exc = exc;
    this->param = string(param);
}

C_E::C_E(int exc, string param) {
    this->exc = exc;
    this->param = param;
}

const char* C_E::Descr(void) const {
    static string descr;
    descr.clear();

    switch (exc) {
        case E_None:
            descr.append("None");
            break;

        case E_FileNotFound:
            descr.append("File not found");
            break;

        case E_ReadError:
            descr.append("Read error");
            break;

        case E_WriteError:
            descr.append("Write error");
            break;

        case E_FileNotOpened:
            descr.append("File not opened");
            break;

        case E_IncorrectAccMode:
            descr.append("Incorrect access mode");
            break;

        case E_UnGetError:
            descr.append("UnGet error");
            break;

        case E_FileIsCompressed:
            descr.append("File is compressed");
            break;

        case E_EmptyFileName:
            descr.append("Empty file name");
            break;

        case E_XMLParserError:
            descr.append("XML parser error");
            break;

        case E_ConfigError:
            descr.append("Config error");
            break;

        case E_DirWork:
            descr.append("Error in DirWork module");
            break;

        case E_CantLock:
            descr.append("Can't lock surface");
            break;

        case E_HandlerAlreadyDefined:
            descr.append("Handler already defined");
            break;

        case E_ObjGfxNotFound:
            descr.append("Object gfx not found");
            break;

        case E_SnapZ80Error:
            descr.append("Snapshot error");
            break;

        default:
            descr.append("Unknown exception");
    }

    if (!param.empty()) {
        descr.append(" : \"");
        descr.append(param);
        descr.append("\"");
    }

    return descr.c_str();
}
