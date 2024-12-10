# uavcan
两种通信模式:
- 服务(服务器/客户端)
- 消息(发布/订阅模式)

- uavcan消息的传输以`IDL`格式描述,如心跳包的发送
```C
	u_n_Heartbeat.vendor_specific_status_code = 0;
	u_n_Heartbeat.health.value = uavcan_node_Health_1_0_NOMINAL;
	u_n_Heartbeat.mode.value = uavcan_node_Mode_1_0_OPERATIONAL;
	length1 = uavcan_node_Heartbeat_1_0_EXTENT_BYTES_;
	uavcan_node_Heartbeat_1_0_serialize_(&u_n_Heartbeat, buff, &length1);//通过该函数将待发送的数据转为IDL格式
    publish_message(uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_, buff, (uint16_t)length1);

```
## 心跳包解析
- 心跳包对应的通信模式为"消息"。
- 心跳包携带内容:
    - 时间戳:发包时的时间戳
- 心跳包ID
    - 是否为标准的ID，是

- 心跳包的发送过程:
```C
static void CommunicationPubHeartbeat(void)
{
	size_t length1 = 0;
	uint8_t buff[uavcan_node_Heartbeat_1_0_EXTENT_BYTES_] = {0u};
	uavcan_node_Heartbeat_1_0 u_n_Heartbeat = {0};
	u_n_Heartbeat.uptime = stCommunication.u4Uptime;
	u_n_Heartbeat.vendor_specific_status_code = 0;
	u_n_Heartbeat.health.value = uavcan_node_Health_1_0_NOMINAL;
	u_n_Heartbeat.mode.value = uavcan_node_Mode_1_0_OPERATIONAL;
	length1 = uavcan_node_Heartbeat_1_0_EXTENT_BYTES_;
	uavcan_node_Heartbeat_1_0_serialize_(&u_n_Heartbeat, buff, &length1);
	publish_message(uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_, buff, (uint16_t)length1);
}
```
- `uavcan_node_Heartbeat_1_0`来自`"Heartbeat_1_0.h"`
- 该头文件由:nanut对.dsdl文件编译而来
位于`https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/7509.Heartbeat.1.0.dsdl`目录
```DSDL
uint16 MAX_PUBLICATION_PERIOD = 1   # [second]
uint16 OFFLINE_TIMEOUT = 3          # [second]
uint32 uptime                       # [second]
Health.1.0 health
Mode.1.0 mode
uint8 vendor_specific_status_code
@assert _offset_ % 8 == {0}
@assert _offset_ == {56}  # Fits into a single-frame Classic CAN transfer (least capable transport, smallest MTU).
@extent 12 * 8
```

- 编译之后的.h文件
```C

#ifndef UAVCAN_NODE_HEARTBEAT_1_0_INCLUDED_
#define UAVCAN_NODE_HEARTBEAT_1_0_INCLUDED_

#include <nunavut/support/serialization.h>
#include <stdint.h>
#include <stdlib.h>
#include <uavcan/node/Health_1_0.h>
#include <uavcan/node/Mode_1_0.h>

#include "canard_base_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

#define uavcan_node_Heartbeat_1_0_HAS_FIXED_PORT_ID_ true
#define uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_     7509U

#define uavcan_node_Heartbeat_1_0_FULL_NAME_             "uavcan.node.Heartbeat"
#define uavcan_node_Heartbeat_1_0_FULL_NAME_AND_VERSION_ "uavcan.node.Heartbeat.1.0"

#define uavcan_node_Heartbeat_1_0_EXTENT_BYTES_                    12UL
#define uavcan_node_Heartbeat_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_ 7UL

#define uavcan_node_Heartbeat_1_0_MAX_PUBLICATION_PERIOD (1U)
#define uavcan_node_Heartbeat_1_0_OFFLINE_TIMEOUT (3U)

typedef struct
{
    uint32_t uptime;
    uavcan_node_Health_1_0 health;
    uavcan_node_Mode_1_0 mode;
    uint8_t vendor_specific_status_code;
} uavcan_node_Heartbeat_1_0;

int8_t uavcan_node_Heartbeat_1_0_serialize_(void *obj, uint8_t* const buffer,  size_t* const inout_buffer_size_bytes);
int8_t uavcan_node_Heartbeat_1_0_deserialize_(void *out_obj, const uint8_t* buffer, size_t* const inout_buffer_size_bytes);
void uavcan_node_Heartbeat_1_0_initialize_(void *out_obj);

#ifdef uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_
extern CanardBaseHandle uavcan_node_Heartbeat_1_0_handle;

#define UAVCAN_NODE_HEARTBEAT_1_0(name, obj, time, prio, remote_id, transfer_count) \
	size_t uavcan_node_Heartbeat_1_0_size = uavcan_node_Heartbeat_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_; \
	uint8_t uavcan_node_Heartbeat_1_0_buf[uavcan_node_Heartbeat_1_0_size]; \
	memset(uavcan_node_Heartbeat_1_0_buf, 0, uavcan_node_Heartbeat_1_0_size); \
	uavcan_node_Heartbeat_1_0_serialize_(obj, uavcan_node_Heartbeat_1_0_buf, &uavcan_node_Heartbeat_1_0_size); \
	const CanardTransfer name = { \
		.timestamp_usec = time, \
		.priority = prio, \
		.transfer_kind = CanardTransferKindMessage, \
		.port_id = uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_,  \
		.remote_node_id = remote_id, \
		.transfer_id = transfer_count++, \
		.payload_size = uavcan_node_Heartbeat_1_0_size, \
		.payload = uavcan_node_Heartbeat_1_0_buf \
	}

#endif

#ifdef __cplusplus
}
#endif
#endif // UAVCAN_NODE_HEARTBEAT_1_0_INCLUDED_
```

- 编译之后应该产生的.c文件(暂未找到)


