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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../CDC/autofiles/usbdsc.c ../CDC/system/usb/class/cdc/cdc.c ../CDC/system/usb/usb9/usb9.c ../CDC/system/usb/usbctrltrf/usbctrltrf.c ../CDC/system/usb/usbdrv/usbdrv.c ../CDC/system/usb/usbmmap.c ../c_Micro.c ../c_Version.c ../Communic.c ../Config.c ../Crc.c ../FrameCom.c ../InOut.c ../Interrupt.c ../Main.c ../RunLed.c ../Tempo.c ../Timer.c ../Uart1.c ../UartUsb.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/779142580/usbdsc.o ${OBJECTDIR}/_ext/983962044/cdc.o ${OBJECTDIR}/_ext/687651300/usb9.o ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o ${OBJECTDIR}/_ext/592019309/usbdrv.o ${OBJECTDIR}/_ext/2107228672/usbmmap.o ${OBJECTDIR}/_ext/1472/c_Micro.o ${OBJECTDIR}/_ext/1472/c_Version.o ${OBJECTDIR}/_ext/1472/Communic.o ${OBJECTDIR}/_ext/1472/Config.o ${OBJECTDIR}/_ext/1472/Crc.o ${OBJECTDIR}/_ext/1472/FrameCom.o ${OBJECTDIR}/_ext/1472/InOut.o ${OBJECTDIR}/_ext/1472/Interrupt.o ${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/1472/RunLed.o ${OBJECTDIR}/_ext/1472/Tempo.o ${OBJECTDIR}/_ext/1472/Timer.o ${OBJECTDIR}/_ext/1472/Uart1.o ${OBJECTDIR}/_ext/1472/UartUsb.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/779142580/usbdsc.o.d ${OBJECTDIR}/_ext/983962044/cdc.o.d ${OBJECTDIR}/_ext/687651300/usb9.o.d ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o.d ${OBJECTDIR}/_ext/592019309/usbdrv.o.d ${OBJECTDIR}/_ext/2107228672/usbmmap.o.d ${OBJECTDIR}/_ext/1472/c_Micro.o.d ${OBJECTDIR}/_ext/1472/c_Version.o.d ${OBJECTDIR}/_ext/1472/Communic.o.d ${OBJECTDIR}/_ext/1472/Config.o.d ${OBJECTDIR}/_ext/1472/Crc.o.d ${OBJECTDIR}/_ext/1472/FrameCom.o.d ${OBJECTDIR}/_ext/1472/InOut.o.d ${OBJECTDIR}/_ext/1472/Interrupt.o.d ${OBJECTDIR}/_ext/1472/Main.o.d ${OBJECTDIR}/_ext/1472/RunLed.o.d ${OBJECTDIR}/_ext/1472/Tempo.o.d ${OBJECTDIR}/_ext/1472/Timer.o.d ${OBJECTDIR}/_ext/1472/Uart1.o.d ${OBJECTDIR}/_ext/1472/UartUsb.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/779142580/usbdsc.o ${OBJECTDIR}/_ext/983962044/cdc.o ${OBJECTDIR}/_ext/687651300/usb9.o ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o ${OBJECTDIR}/_ext/592019309/usbdrv.o ${OBJECTDIR}/_ext/2107228672/usbmmap.o ${OBJECTDIR}/_ext/1472/c_Micro.o ${OBJECTDIR}/_ext/1472/c_Version.o ${OBJECTDIR}/_ext/1472/Communic.o ${OBJECTDIR}/_ext/1472/Config.o ${OBJECTDIR}/_ext/1472/Crc.o ${OBJECTDIR}/_ext/1472/FrameCom.o ${OBJECTDIR}/_ext/1472/InOut.o ${OBJECTDIR}/_ext/1472/Interrupt.o ${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/1472/RunLed.o ${OBJECTDIR}/_ext/1472/Tempo.o ${OBJECTDIR}/_ext/1472/Timer.o ${OBJECTDIR}/_ext/1472/Uart1.o ${OBJECTDIR}/_ext/1472/UartUsb.o

# Source Files
SOURCEFILES=../CDC/autofiles/usbdsc.c ../CDC/system/usb/class/cdc/cdc.c ../CDC/system/usb/usb9/usb9.c ../CDC/system/usb/usbctrltrf/usbctrltrf.c ../CDC/system/usb/usbdrv/usbdrv.c ../CDC/system/usb/usbmmap.c ../c_Micro.c ../c_Version.c ../Communic.c ../Config.c ../Crc.c ../FrameCom.c ../InOut.c ../Interrupt.c ../Main.c ../RunLed.c ../Tempo.c ../Timer.c ../Uart1.c ../UartUsb.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4550
MP_PROCESSOR_OPTION_LD=18f4550
MP_LINKER_DEBUG_OPTION=-r=ROM@0x7DC0:0x7FFF -r=RAM@GPR:0x3F4:0x3FF -u_DEBUGSTACK
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/779142580/usbdsc.o: ../CDC/autofiles/usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/779142580" 
	@${RM} ${OBJECTDIR}/_ext/779142580/usbdsc.o.d 
	@${RM} ${OBJECTDIR}/_ext/779142580/usbdsc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/779142580/usbdsc.o   ../CDC/autofiles/usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/779142580/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/779142580/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/983962044/cdc.o: ../CDC/system/usb/class/cdc/cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/983962044" 
	@${RM} ${OBJECTDIR}/_ext/983962044/cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/983962044/cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/983962044/cdc.o   ../CDC/system/usb/class/cdc/cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/983962044/cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/983962044/cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/687651300/usb9.o: ../CDC/system/usb/usb9/usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/687651300" 
	@${RM} ${OBJECTDIR}/_ext/687651300/usb9.o.d 
	@${RM} ${OBJECTDIR}/_ext/687651300/usb9.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/687651300/usb9.o   ../CDC/system/usb/usb9/usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/687651300/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/687651300/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1946939592/usbctrltrf.o: ../CDC/system/usb/usbctrltrf/usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1946939592" 
	@${RM} ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o.d 
	@${RM} ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o   ../CDC/system/usb/usbctrltrf/usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1946939592/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/592019309/usbdrv.o: ../CDC/system/usb/usbdrv/usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/592019309" 
	@${RM} ${OBJECTDIR}/_ext/592019309/usbdrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/592019309/usbdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/592019309/usbdrv.o   ../CDC/system/usb/usbdrv/usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/592019309/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/592019309/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/2107228672/usbmmap.o: ../CDC/system/usb/usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2107228672" 
	@${RM} ${OBJECTDIR}/_ext/2107228672/usbmmap.o.d 
	@${RM} ${OBJECTDIR}/_ext/2107228672/usbmmap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/2107228672/usbmmap.o   ../CDC/system/usb/usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/2107228672/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2107228672/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/c_Micro.o: ../c_Micro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Micro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Micro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/c_Micro.o   ../c_Micro.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/c_Micro.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/c_Micro.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/c_Version.o: ../c_Version.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Version.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Version.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/c_Version.o   ../c_Version.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/c_Version.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/c_Version.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Communic.o: ../Communic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Communic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Communic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Communic.o   ../Communic.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Communic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Communic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Config.o: ../Config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Config.o   ../Config.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Config.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Crc.o: ../Crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Crc.o   ../Crc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Crc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Crc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/FrameCom.o: ../FrameCom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/FrameCom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/FrameCom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/FrameCom.o   ../FrameCom.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/FrameCom.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/FrameCom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/InOut.o: ../InOut.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/InOut.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/InOut.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/InOut.o   ../InOut.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/InOut.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/InOut.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Interrupt.o: ../Interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Interrupt.o   ../Interrupt.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Main.o   ../Main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/RunLed.o: ../RunLed.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/RunLed.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/RunLed.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/RunLed.o   ../RunLed.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/RunLed.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/RunLed.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Tempo.o: ../Tempo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Tempo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Tempo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Tempo.o   ../Tempo.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Tempo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Tempo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Timer.o: ../Timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Timer.o   ../Timer.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Uart1.o: ../Uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Uart1.o   ../Uart1.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Uart1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Uart1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/UartUsb.o: ../UartUsb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/UartUsb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/UartUsb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/UartUsb.o   ../UartUsb.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/UartUsb.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/UartUsb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/779142580/usbdsc.o: ../CDC/autofiles/usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/779142580" 
	@${RM} ${OBJECTDIR}/_ext/779142580/usbdsc.o.d 
	@${RM} ${OBJECTDIR}/_ext/779142580/usbdsc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/779142580/usbdsc.o   ../CDC/autofiles/usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/779142580/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/779142580/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/983962044/cdc.o: ../CDC/system/usb/class/cdc/cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/983962044" 
	@${RM} ${OBJECTDIR}/_ext/983962044/cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/983962044/cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/983962044/cdc.o   ../CDC/system/usb/class/cdc/cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/983962044/cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/983962044/cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/687651300/usb9.o: ../CDC/system/usb/usb9/usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/687651300" 
	@${RM} ${OBJECTDIR}/_ext/687651300/usb9.o.d 
	@${RM} ${OBJECTDIR}/_ext/687651300/usb9.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/687651300/usb9.o   ../CDC/system/usb/usb9/usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/687651300/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/687651300/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1946939592/usbctrltrf.o: ../CDC/system/usb/usbctrltrf/usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1946939592" 
	@${RM} ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o.d 
	@${RM} ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o   ../CDC/system/usb/usbctrltrf/usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1946939592/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1946939592/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/592019309/usbdrv.o: ../CDC/system/usb/usbdrv/usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/592019309" 
	@${RM} ${OBJECTDIR}/_ext/592019309/usbdrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/592019309/usbdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/592019309/usbdrv.o   ../CDC/system/usb/usbdrv/usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/592019309/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/592019309/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/2107228672/usbmmap.o: ../CDC/system/usb/usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2107228672" 
	@${RM} ${OBJECTDIR}/_ext/2107228672/usbmmap.o.d 
	@${RM} ${OBJECTDIR}/_ext/2107228672/usbmmap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/2107228672/usbmmap.o   ../CDC/system/usb/usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/2107228672/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2107228672/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/c_Micro.o: ../c_Micro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Micro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Micro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/c_Micro.o   ../c_Micro.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/c_Micro.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/c_Micro.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/c_Version.o: ../c_Version.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Version.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/c_Version.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/c_Version.o   ../c_Version.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/c_Version.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/c_Version.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Communic.o: ../Communic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Communic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Communic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Communic.o   ../Communic.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Communic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Communic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Config.o: ../Config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Config.o   ../Config.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Config.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Crc.o: ../Crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Crc.o   ../Crc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Crc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Crc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/FrameCom.o: ../FrameCom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/FrameCom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/FrameCom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/FrameCom.o   ../FrameCom.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/FrameCom.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/FrameCom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/InOut.o: ../InOut.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/InOut.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/InOut.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/InOut.o   ../InOut.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/InOut.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/InOut.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Interrupt.o: ../Interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Interrupt.o   ../Interrupt.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Main.o   ../Main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/RunLed.o: ../RunLed.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/RunLed.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/RunLed.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/RunLed.o   ../RunLed.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/RunLed.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/RunLed.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Tempo.o: ../Tempo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Tempo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Tempo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Tempo.o   ../Tempo.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Tempo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Tempo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Timer.o: ../Timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Timer.o   ../Timer.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/Uart1.o: ../Uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/Uart1.o   ../Uart1.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Uart1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Uart1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/UartUsb.o: ../UartUsb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/UartUsb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/UartUsb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../CDC/system/usb/class/cdc" -I"../CDC/system/usb/usb9" -I"../CDC/system/usb/usbctrltrf" -I"../CDC/system/usb/usbdefs" -I"../CDC/system/usb/usbdrv" -I"../CDC/autofiles" -I"../CDC/system/usb" -I"../CDC" -I"../CDC" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/UartUsb.o   ../UartUsb.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/UartUsb.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/UartUsb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../18f4550_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "..\18f4550_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG   -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../18f4550_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "..\18f4550_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w    -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/GEOTROLL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
