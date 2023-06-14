// RUN CODE:
 // gcc -o ras_mqtt ras_mqtt.c -lwiringPi -lpthread -lmysqlclient -lpaho-mqtt3c -lm

#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stdint.h>

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "subscribe_demo"
#define PUB_TOPIC   "k20" 
#define SUB_TOPIC   "k21"   


void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

// int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
//     char *token = strtok(message->payload, ",");  
//     int id= atoi(token); // 1,5,98...,2023-
//     token = strtok(NULL, ",");
//     char* rfid_uid = token;
//     token = strtok(NULL, ",");
//     char* name = token;

//     // sscanf(token, "%d,%d,%s,%s", &id,&user_id,&rfid_uid, &time);

//     printf("----Received message:  rfid_uid: '%s' ,name :'%s'\n",rfid_uid,name);


//     // Connect to database
//     MYSQL *conn;
//     char *server = "localhost"; 
//     char *user = "sangiot";
//     char *password = "123"; /* set me first */
//     char *database = "finalExam";

//     conn = mysql_init(NULL);
//     // ktra mysql
//     if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) 
//     {
//         fprintf(stderr, "%s\n", mysql_error(conn));
//         mysql_close(conn);
//         exit(1);
//     }  
//     ///tao 3 bien char de gui data xuong mysql

//     char sql[1000];
//     //sprintf(sql, "INSERT INTO attendance(user_id,rfid_uid,time) VALUES(%d,'%s','%s')",user_id,rfid_uid,time);
//     // sprintf(sql, "UPDATE datastudent SET timein='%s' where id = '%s' ",time, rfid_uid);
//     sprintf(sql, "INSERT INTO users (id,rfid_uid,name) values (%d,'%s','%s')",id, rfid_uid,name);

//     mysql_query(conn,sql);
//     mysql_close(conn); 
//     MQTTClient_freeMessage(&message);
//     MQTTClient_free(topicName);
//     return 1;
// }

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    //MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    int rc;  
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    //connect sql
    MYSQL *conn;
    MYSQL_RES *res; // variable used to store DB data
    MYSQL_ROW row;
    char *server = "localhost"; 
    char *user = "sangiot";
    char *password = "123"; /* set me 2 */
    char *database = "finalExam";
  
    //listen for operation
    MQTTClient_subscribe(client, SUB_TOPIC, 0);

    while(1) {
        // Connect to database
        conn = mysql_init(NULL);
        mysql_real_connect(conn,server,user,password,database,0,NULL,0);
        // Read data from database
        mysql_query(conn, "select * from checkedForm");
        res = mysql_store_result(conn);
        int num_row = mysql_num_rows(res);
        int num_column = mysql_num_fields(res);
        //printf("%d",num_row);
        while (row = mysql_fetch_row(res)){
            //for(int i = 1; i <= num_column; i++){
                char msg [200];
                sprintf(msg,"%s,%s,%s",row[0],row[1],row[2]); // gui data format: id,time_red,time_green,time_yellow
                publish(client, PUB_TOPIC, msg);
            //}
            // sau 2s thi gui data di 1 lana
            sleep(2);
        }
    // clear result and close the connection
    mysql_free_result(res);
    mysql_close(conn);
    }
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}