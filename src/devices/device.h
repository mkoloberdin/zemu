#ifndef _DEVICE_H_INCLUDED_
#define _DEVICE_H_INCLUDED_

class C_Device {
    public:

    virtual ~C_Device() {};
    virtual void Init(void) = 0;
    virtual void Close(void) = 0;
};

#endif
