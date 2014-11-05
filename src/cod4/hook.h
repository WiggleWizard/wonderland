/**
 * Author:  Terence-Lee 'Zinglish' Davis
 * License: MIT
 * Email:   zinglish[at]gmail.com
 * Twitter: @zingatsu
 */

#ifndef HOOK_H
#define HOOK_H

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

class Hook
{

    public:
        Hook(void* targetFunction, unsigned long copyLength, void* replacementFunction);
		void UnHook();
		void Rehook();
        void* GetPage(const unsigned long uslngVirtualMemoryAddress);
        void* GetOriginalFunc();
        virtual ~Hook();
    protected:
    private:
        unsigned long mPageSize;
        unsigned long mPageMask;
		
		void* mReplacementFunc;
		void* mTargetFunc;
		unsigned long mLength;
        void* mOriginalFunc;
};

#endif // HOOK_H
