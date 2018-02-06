#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include <cstdint>

namespace ir {
    
    struct iovec {
        uint8_t *buffer;
        int size;
    };

    class IrObject {
        public:
            IrObject();
            virtual void serialize(); // Packs the bytes into the iovec
            struct iovec getIovec();

            int getOffset();
            void setOffset(int offset);
        protected:
            int offset;
            struct iovec iovec_data; 

    };
}

#endif
