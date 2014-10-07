#include "sound.h"


////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& out, const FMOD_VECTOR& v) {
    out << v.x << ", " << v.y << ", " << v.z << std::endl;
    return out;
}


////////////////////////////////////////////////////////////////////////////////
namespace FMOD
{
    void check(FMOD_RESULT result)
    {
        if (result != FMOD_OK) {
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }
    }
}
