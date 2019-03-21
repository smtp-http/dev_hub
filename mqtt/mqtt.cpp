#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <mqtt.h>
#include <string.h>
#include <unistd.h>

using namespace lux;
using namespace std;

#define HOST "10.10.80.11"
#define PORT  1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE  512

bool session = true;

int Mqtt::mqtt()
{
    char buff[MSG_MAX_SIZE];
    struct mosquitto *mosq = NULL;
    //libmosquitto 库初始化
    mosquitto_lib_init();
    //创建mosquitto客户端
    mosq = mosquitto_new(NULL,session,NULL);
    if(!mosq){
        printf("create client failed..\n");
        mosquitto_lib_cleanup();
        return 1;
    }


    //连接服务器
    if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)){

        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }
    //开启一个线程，在线程里不停的调用 mosquitto_loop() 来处理网络信息
    int loop = mosquitto_loop_start(mosq);
    if(loop != MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto loop error\n");
        return 1;
    }
    int i = 0;
    const void* sss = "ni hao666";
    while(i != 10)
    {
                /*发布消息*/
                printf("---------------mqtt---%d----------\r\n", &i);
                mosquitto_publish(mosq,NULL,"123",10,sss,0,false);
               //usleep(100);
                memset(buff,0,sizeof(buff));
                i++;
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
