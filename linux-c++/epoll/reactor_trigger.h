#pragma once

class ReactorTrigger {
    virtual int32_t getFileDescripter() = 0;
    virtual void run() = 0;
};
