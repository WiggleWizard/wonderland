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
	${OBJECTDIR}/_ext/1973015878/callables.o \
	${OBJECTDIR}/_ext/1973015878/controller.o \
	${OBJECTDIR}/_ext/1973015878/events.o \
	${OBJECTDIR}/_ext/1973015878/hook.o \
	${OBJECTDIR}/_ext/1183286110/ipc_comm.o \
	${OBJECTDIR}/_ext/1183286110/ipc_event.o \
	${OBJECTDIR}/_ext/1183286110/ipc_rx_pkt.o \
	${OBJECTDIR}/_ext/1183286110/ipc_server_man.o \
	${OBJECTDIR}/_ext/1211768401/logger.o \
	${OBJECTDIR}/_ext/1211768401/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalice.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalice.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalice.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -ldl -m32 -fPIC -shared -fPIC

${OBJECTDIR}/_ext/1973015878/callables.o: /home/zinglish/Projects/project-alice/alice/src/cod4/callables.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1973015878
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1973015878/callables.o /home/zinglish/Projects/project-alice/alice/src/cod4/callables.cpp

${OBJECTDIR}/_ext/1973015878/controller.o: /home/zinglish/Projects/project-alice/alice/src/cod4/controller.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1973015878
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1973015878/controller.o /home/zinglish/Projects/project-alice/alice/src/cod4/controller.cpp

${OBJECTDIR}/_ext/1973015878/events.o: /home/zinglish/Projects/project-alice/alice/src/cod4/events.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1973015878
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1973015878/events.o /home/zinglish/Projects/project-alice/alice/src/cod4/events.cpp

${OBJECTDIR}/_ext/1973015878/hook.o: /home/zinglish/Projects/project-alice/alice/src/cod4/hook.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1973015878
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1973015878/hook.o /home/zinglish/Projects/project-alice/alice/src/cod4/hook.cpp

${OBJECTDIR}/_ext/1183286110/ipc_comm.o: /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_comm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1183286110
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1183286110/ipc_comm.o /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_comm.cpp

${OBJECTDIR}/_ext/1183286110/ipc_event.o: /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_event.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1183286110
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1183286110/ipc_event.o /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_event.cpp

${OBJECTDIR}/_ext/1183286110/ipc_rx_pkt.o: /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_rx_pkt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1183286110
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1183286110/ipc_rx_pkt.o /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_rx_pkt.cpp

${OBJECTDIR}/_ext/1183286110/ipc_server_man.o: /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_server_man.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1183286110
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1183286110/ipc_server_man.o /home/zinglish/Projects/project-alice/alice/src/ipc/ipc_server_man.cpp

${OBJECTDIR}/_ext/1211768401/logger.o: /home/zinglish/Projects/project-alice/alice/src/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1211768401
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1211768401/logger.o /home/zinglish/Projects/project-alice/alice/src/logger.cpp

${OBJECTDIR}/_ext/1211768401/main.o: /home/zinglish/Projects/project-alice/alice/src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1211768401
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1211768401/main.o /home/zinglish/Projects/project-alice/alice/src/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalice.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
