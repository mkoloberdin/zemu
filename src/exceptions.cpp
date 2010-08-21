#include "exceptions.h"

C_E::C_E()
{
	exc = E_None;
	param = "";
}

C_E::C_E(int exc)
{
	this->exc = exc;
	param = "";
}

C_E::C_E(int exc, const char *param)
{
	this->exc = exc;
	this->param = string(param);
}

C_E::C_E(int exc, string param)
{
	this->exc = exc;
	this->param = param;
}

const char * C_E::Descr(void) const
{
	switch (exc)
	{
		case E_None: return "None";
	}

	return "Unknown exception";
}
