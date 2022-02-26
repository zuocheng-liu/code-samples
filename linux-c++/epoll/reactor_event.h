#pragma once

class ReactorEvent {
    virtual int32_t getFileDescripter() = 0;
    virtual void trigger() = 0;
};
