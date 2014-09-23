#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/cod4/callables.o \
	${OBJECTDIR}/src/cod4/controller.o \
	${OBJECTDIR}/src/cod4/events.o \
	${OBJECTDIR}/src/cod4/hook.o \
	${OBJECTDIR}/src/ipc/ipc_comm.o \
	${OBJECTDIR}/src/ipc/ipc_event.o \
	${OBJECTDIR}/src/ipc/ipc_rx_func_pkt.o \
	${OBJECTDIR}/src/ipc/ipc_server_man.o \
	${OBJECTDIR}/src/logger.o \
	${OBJECTDIR}/src/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -Wall -fexceptions -fPIC -O0 -J3
CXXFLAGS=-m32 -Wall -fexceptions -fPIC -O0 -J3

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libwonderland.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libwonderland.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libwonderland.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -ldl -m32 -fPIC -shared -fPIC

${OBJECTDIR}/src/cod4/callables.o: src/cod4/callables.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/cod4
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cod4/callables.o src/cod4/callables.cpp

${OBJECTDIR}/src/cod4/controller.o: src/cod4/controller.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/cod4
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cod4/controller.o src/cod4/controller.cpp

${OBJECTDIR}/src/cod4/events.o: src/cod4/events.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/cod4
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cod4/events.o src/cod4/events.cpp

${OBJECTDIR}/src/cod4/hook.o: src/cod4/hook.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/cod4
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cod4/hook.o src/cod4/hook.cpp

${OBJECTDIR}/src/ipc/ipc_comm.o: src/ipc/ipc_comm.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/ipc
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipc/ipc_comm.o src/ipc/ipc_comm.cpp

${OBJECTDIR}/src/ipc/ipc_event.o: src/ipc/ipc_event.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/ipc
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipc/ipc_event.o src/ipc/ipc_event.cpp

${OBJECTDIR}/src/ipc/ipc_rx_func_pkt.o: src/ipc/ipc_rx_func_pkt.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/ipc
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipc/ipc_rx_func_pkt.o src/ipc/ipc_rx_func_pkt.cpp

${OBJECTDIR}/src/ipc/ipc_server_man.o: src/ipc/ipc_server_man.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/ipc
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipc/ipc_server_man.o src/ipc/ipc_server_man.cpp

${OBJECTDIR}/src/logger.o: src/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/logger.o src/logger.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libwonderland.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
