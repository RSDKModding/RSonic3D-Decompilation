#include "RetroEngine.hpp"

void StrCopy(char *dest, const char *src)
{
#ifdef USE_STDLIB
    strcpy(dest, src);
#else
    int i = 0;
    for (; src[i]; ++i) dest[i] = src[i];
    dest[i] = 0;
#endif
}

#if !RETRO_USE_ORIGINAL_CODE
int FindLastStringToken(const char *string, const char *token)
{
    int tokenCharID  = 0;
    bool tokenMatch  = true;
    int stringCharID = 0;
    int foundTokenID = 0;
    int lastResult = -1;

    while (string[stringCharID]) {
        tokenCharID = 0;
        tokenMatch  = true;
        while (token[tokenCharID]) {
            if (!string[tokenCharID + stringCharID])
                return lastResult;

            if (string[tokenCharID + stringCharID] != token[tokenCharID])
                tokenMatch = false;

            ++tokenCharID;
        }
        if (tokenMatch)
            lastResult = stringCharID;

        ++stringCharID;
    }
    return lastResult;
}
#endif