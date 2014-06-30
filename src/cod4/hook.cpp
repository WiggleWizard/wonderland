#include "hook.h"

Hook::Hook(void* targetFunction, unsigned long copyLength, void* replacementFunction)
{
	// Assign our class variables
	mReplacementFunc = replacementFunction;
	mTargetFunc = targetFunction;
	mLength = copyLength;
	
    // Initialize our page size variables
    mPageSize = sysconf(_SC_PAGE_SIZE);
    mPageMask = ( ~(mPageSize - 1) );

    int Relocation;
    mOriginalFunc = malloc( copyLength + (1 + sizeof(int)) );
    memcpy( mOriginalFunc, targetFunction, copyLength);
    memset( (void*) ((unsigned long) (mOriginalFunc) + copyLength),0xE9, 1 ) ;

    Relocation = (int) ((unsigned long) (targetFunction) - ((unsigned long) (mOriginalFunc)+ (1 + sizeof(int)))) ;
    memcpy( (void*)( (unsigned long) (mOriginalFunc) + copyLength + 1), &Relocation, sizeof(int)) ;
    mprotect(GetPage((unsigned long) (targetFunction)),mPageSize,PROT_READ|PROT_WRITE|PROT_EXEC);
    memset(targetFunction, 0xE9, 1);

    Relocation = (int) ( (unsigned long) (replacementFunction) - ((unsigned long) (targetFunction) + (1 + sizeof(int)))) ;
    memcpy( (void*) ((unsigned long) (targetFunction) + 1), &Relocation, sizeof(int)) ;
    mprotect(GetPage((unsigned long) (mOriginalFunc)),mPageSize,PROT_READ|PROT_WRITE|PROT_EXEC);
}

void Hook::UnHook()
{
    mprotect(GetPage((unsigned long) (mTargetFunc)), mPageSize, PROT_READ|PROT_WRITE|PROT_EXEC);
    memcpy( mTargetFunc, mOriginalFunc, mLength);
    free(mOriginalFunc);
}

void Hook::Rehook()
{	
    int Relocation;
    mOriginalFunc = malloc( mLength + (1 + sizeof(int)) );
    memcpy( mOriginalFunc, mTargetFunc, mLength);
    memset( (void*) ((unsigned long) (mOriginalFunc) + mLength),0xE9, 1 ) ;

    Relocation = (int) ((unsigned long) (mTargetFunc) - ((unsigned long) (mOriginalFunc)+ (1 + sizeof(int)))) ;
    memcpy( (void*)( (unsigned long) (mOriginalFunc) + mLength + 1), &Relocation, sizeof(int)) ;
    mprotect(GetPage((unsigned long) (mTargetFunc)),mPageSize,PROT_READ|PROT_WRITE|PROT_EXEC);
    memset(mTargetFunc, 0xE9, 1) ;

    Relocation = (int) ( (unsigned long) (mReplacementFunc) - ((unsigned long) (mTargetFunc) + (1 + sizeof(int)))) ;
    memcpy( (void*) ((unsigned long) (mTargetFunc) + 1), &Relocation, sizeof(int)) ;
    mprotect(GetPage((unsigned long) (mOriginalFunc)),mPageSize,PROT_READ|PROT_WRITE|PROT_EXEC);
}

void* Hook::GetPage(const unsigned long uslngVirtualMemoryAddress)
{
    return (void*) (uslngVirtualMemoryAddress & mPageMask) ;
}

void* Hook::GetOriginalFunc()
{
	return this->mOriginalFunc;
}

Hook::~Hook()
{
    //dtor
}
