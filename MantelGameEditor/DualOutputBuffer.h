#pragma once

#include "Globals.h"

class DualOutputBuffer : public std::streambuf {
public:
    DualOutputBuffer(std::streambuf* original, std::ostream& secondary) : originalStream(original), secondaryStream(secondary) {}

    streambuf* GetOriginalStream() {
        return this->originalStream;
    }

protected:
    virtual int_type overflow(int_type c) {
        if (c != EOF) {
            originalStream->sputc(c); // Write to the original stream (stdout)
            secondaryStream.put(c);  // Write to the secondary stream (e.g., ImGui window)
        }
        return c;
    }

private:
    std::streambuf* originalStream;
    std::ostream& secondaryStream;
};