#include "ir/serializable.hpp"

namespace ir {

    IrObject::IrObject()
    {
        iovec_data.buffer = nullptr;
        iovec_data.size = 0;
    }

    struct iovec IrObject::getIovec()
    {
        return iovec_data;
    }
}
