# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NetworkLib/libtest.c 
#../NetworkLib/xmalloc.c \
#../NetworkLib/oauth_http.c \
#../NetworkLib/oauth.c \
#../NetworkLib/hash.c \
#../NetworkLib/sha1.c

OBJS += \
./NetworkLib/libtest.o
#./NetworkLib/xmalloc.o \
#./NetworkLib/oauth_http.o \
#./NetworkLib/oauth.o \
#./NetworkLib/hash.o \
#./NetworkLib/sha1.o

C_DEPS += \
./NetworkLib/libtest.d 
#./NetworkLib/xmalloc.d \
#./NetworkLib/oauth_http.d \
#./NetworkLib/oauth.d \
#./NetworkLib/hash.d \
#./NetworkLib/sha1.d


# Each subdirectory must supply rules for building sources it contributes
NetworkLib/%.o: ../NetworkLib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__GCC_POSIX__=1 -DDEBUG_BUILD=1 -DUSE_STDIO=1 -I../Common_Demo/include -I.. -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O0 -g -Wall -c -fmessage-length=0 -pthread -lrt -Wno-pointer-sign -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

