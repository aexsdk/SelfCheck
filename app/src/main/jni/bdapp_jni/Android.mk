LOCAL_PATH		:= $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE		:= DataSwitch
LOCAL_SRC_FILES	:= JniDataSwitchCls.cpp
LOCAL_LDLIBS		:= -llog
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE		:= DataPrase
LOCAL_SRC_FILES	:= BDDataParse.cpp
LOCAL_STATIC_LIBRARIES 	:= DataSwitch
LOCAL_LDLIBS		:= -llog
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE		:= BDAll
LOCAL_SRC_FILES	:= BDAll.cpp
LOCAL_STATIC_LIBRARIES 	:= DataSwitch
LOCAL_LDLIBS		:= -llog
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE		:= DownCpu
LOCAL_SRC_FILES	:= CDownLoadCpu.cpp
LOCAL_STATIC_LIBRARIES 	:= DataSwitch
LOCAL_LDLIBS		:= -llog
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE		:= DownCpuRD
LOCAL_SRC_FILES	:= DownLoadCpuRD.cpp
LOCAL_STATIC_LIBRARIES 	:= DataSwitch
LOCAL_LDLIBS		:= -llog
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE		:= DataInterface
LOCAL_SRC_FILES	:= Interface.cpp
LOCAL_STATIC_LIBRARIES := DataPrase BDAll DownCpu DownCpuRD
LOCAL_LDLIBS		:= -llog

include $(BUILD_SHARED_LIBRARY)
