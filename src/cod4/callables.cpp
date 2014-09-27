#include "callables.h"

Callables::Callables() {
}

Callables::Callables(const Callables& orig) {
}

Callables::~Callables() {
}

uint32_t Callables::GetPlayerIdByOffset(uint32_t offset)
{
	return (offset - 0x090B4F8C) / 677436;
}