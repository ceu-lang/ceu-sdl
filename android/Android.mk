DEBUG   := -D CEU_DEBUG -D DEBUG
ANDROID := -D __ANDROID__ -D ANDROID
#NDK_DEBUG := 1

LOCAL_C_FLAGS := $(DEBUG) $(ANDROID)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH       := android-project/jni/SDL
SDL_image_PATH := android-project/jni/SDL_image
SDL_ttf_PATH   := android-project/jni/SDL_ttf
SDL_mixer_PATH := android-project/jni/SDL_mixer
SDL_mixer_PATH := android-project/jni/SDL_net
LUA_PATH       := android-project/jni/lua

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
                    $(LOCAL_PATH)/$(SDL_image_PATH)   \
                    $(LOCAL_PATH)/$(SDL_ttf_PATH)     \
                    $(LOCAL_PATH)/$(SDL_net_PATH)     \
                    $(LOCAL_PATH)/$(LUA_PATH)
                    #$(LOCAL_PATH)/$(SDL_mixer_PATH)

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
	main.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_gfx SDL2_image SDL2_ttf SDL2_net lua #SDL2_mixer

ifdef DEBUG
LOCAL_LDLIBS := -llog
endif

include $(BUILD_SHARED_LIBRARY)

$(shell (cd $(LOCAL_PATH) ; ceu ../samples/clt-srv.ceu))
