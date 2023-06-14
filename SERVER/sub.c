
//RUN CODE
 // gcc -o sub sub_server.c   -lmysqlclient -lpaho-mqtt3c -lm

// gcc sub.c $(mariadb_config --libs) -o sub $(mariadb_config --cflags) -lpaho-mqtt3c -lm
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stdint.h>
#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "publisher_demo"
#define SUB_TOPIC   "k20"   
#define PUB_TOPIC   "k21"   


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
// int id,user_id;
// char time,rfid_uid;
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *token = strtok(message->payload, ",");  
    int id = atoi(token); // 1,5,98...,2023-
    token = strtok(NULL, ",");
    char* rfid_uid = token;
    token = strtok(NULL, ",");
    char* time = token;
    printf("Received message: id: %d, rfid_uid: %s ,time:%s\n",id,rfid_uid,time);

    // Connect to database
    MYSQL *conn;
    char *server = "localhost"; 
    char *user = "Sang";
    char *password = "123"; /* set me first */
    char *database = "attendanceSystem";

    conn = mysql_init(NULL);
    // ktra mysql
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }  
    ///tao 3 bien char de gui data xuong mysql

    char sql[1000];
    char sql1[200];
    
    //sprintf(sql, "UPDATE datastudent SET timein='%s' where id = '%s' ",time, rfid_uid);
    sprintf(sql1, "UPDATE uid_table SET id='%s' where  1 ", rfid_uid);
    
    mysql_query(conn,sql1);
    // mysql_query(conn,sql);
    mysql_close(conn); 
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    int rc;  // return code
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    //connect sql
    MYSQL *conn;
    MYSQL_RES *res; // variable used to store DB data
    MYSQL_ROW row;
    char *server = "localhost"; 
    char *user = "Sang";
    char *password = "123"; /* set me 2 */
    char *database = "attendanceSystem";
  
    //listen for operation
    MQTTClient_subscribe(client, SUB_TOPIC, 0);

    while(1) {
        // Connect to database
        conn = mysql_init(NULL);
        mysql_real_connect(conn,server,user,password,database,0,NULL,0);
        // Read data from database
        mysql_query(conn, "select * from datastudent");
        res = mysql_store_result(conn);

    // clear result and close the connection
    mysql_free_result(res);
    mysql_close(conn);
    }
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}