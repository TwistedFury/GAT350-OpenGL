#include "Transform.h"

namespace neu 
{
    void Transform::Read(const serial_data_t& value) 
    {
        SERIAL_READ(value, position);

        SERIAL_READ(value, rotation);

        SERIAL_READ(value, scale);
    }
}