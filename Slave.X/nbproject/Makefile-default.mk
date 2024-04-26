#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/slave/Slave12F.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1522056099/Slave12F.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1522056099/Slave12F.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1522056099/Slave12F.o

# Source Files
SOURCEFILES=../src/slave/Slave12F.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_CC="C:\Program Files (x86)\PICC\CCSCON.exe"
MP_LD="C:\Program Files (x86)\PICC\CCSCON.exe"
sourceline.device=sourceline="\#device PIC12F1572"
sourceline.xccompat=sourceline="\#device ANSI" sourceline="\#device PASS_STRINGS=IN_RAM" sourceline="\#device CONST=READ_ONLY" sourceline="\#case" sourceline="\#TYPE SIGNED" sourceline="\#type INT=16, LONG=32"
sourceline.gcccompat=sourceline="\#device ANSI" sourceline="\#device PASS_STRINGS=IN_RAM" sourceline="\#device CONST=READ_ONLY" sourceline="\#case" sourceline="\#TYPE SIGNED"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1522056099/Slave12F.o: ../src/slave/Slave12F.c  nbproject/Makefile-${CND_CONF}.mk 
	${MKDIR} ${OBJECTDIR}/_ext/1522056099 
ifeq (2,1) 
	${MKDIR} ${DISTDIR} 
	${MP_CC}  out="${OBJECTDIR}\_ext\1522056099"  ../src/slave/Slave12F.c +FM +DF +CC #__DEBUG=1 +ICD +Y=9 +EA I+="C:\Program Files (x86)\PICC\Drivers" I+="C:\Program Files (x86)\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__12F1572=1 
	@mv ${OBJECTDIR}/_ext/1522056099/Slave12F.cof "${DISTDIR}/Slave.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/_ext/1522056099/Slave12F.hex "${DISTDIR}/Slave.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\_ext\1522056099"" ../src/slave/Slave12F.c +EXPORT +FM +DF +CC #__DEBUG=1 +ICD +Y=9 +EA I+="C:\Program Files (x86)\PICC\Drivers" I+="C:\Program Files (x86)\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__12F1572=1 +EXPORTD="${OBJECTDIR}/_ext/1522056099"  
	
endif 
	
else
${OBJECTDIR}/_ext/1522056099/Slave12F.o: ../src/slave/Slave12F.c  nbproject/Makefile-${CND_CONF}.mk 
	${MKDIR} ${OBJECTDIR}/_ext/1522056099 
ifeq (2,1) 
	${MKDIR} ${DISTDIR} 
	${MP_CC}  out="${OBJECTDIR}\_ext\1522056099"  ../src/slave/Slave12F.c +FM +DF +CC +Y=9 +EA I+="C:\Program Files (x86)\PICC\Drivers" I+="C:\Program Files (x86)\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__12F1572=1 
	@mv ${OBJECTDIR}/_ext/1522056099/Slave12F.cof "${DISTDIR}/Slave.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/_ext/1522056099/Slave12F.hex "${DISTDIR}/Slave.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\_ext\1522056099"" ../src/slave/Slave12F.c +EXPORT +FM +DF +CC +Y=9 +EA I+="C:\Program Files (x86)\PICC\Drivers" I+="C:\Program Files (x86)\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__12F1572=1 +EXPORTD="${OBJECTDIR}/_ext/1522056099"  
	
endif 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	${MKDIR} ${DISTDIR} 
ifeq (2,1) 
	
else 
	${MP_LD}   out="${DISTDIR}"  +FM +DF +CC LINK=Slave.X.${IMAGE_TYPE}.hex=${OBJECTDIR}/_ext/1522056099/Slave12F.o +Y=9 +EA I+="C:\Program Files (x86)\PICC\Drivers" I+="C:\Program Files (x86)\PICC\Devices" +DF +LN +T +A +M +J +Z -P
	
endif 
	
else
${DISTDIR}/Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	${MKDIR} ${DISTDIR} 
ifeq (2,1) 
	
else 
	${MP_LD}   out="${DISTDIR}"  +FM +DF +CC LINK=Slave.X.${IMAGE_TYPE}.hex=${OBJECTDIR}/_ext/1522056099/Slave12F.o +Y=9 +EA I+="C:\Program Files (x86)\PICC\Drivers" I+="C:\Program Files (x86)\PICC\Devices" +DF +LN +T +A +M +J +Z -P
	
endif 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
