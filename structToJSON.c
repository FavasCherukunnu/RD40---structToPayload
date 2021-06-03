#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdint.h>


char payload[7800];

struct mod{
    uint16_t index;
    uint16_t slave_id;
    uint32_t register_address;
    uint8_t function_code;
    char data_type[10];
    char type_order[10];
    char Alias_Name[10];
    char result[10];
    uint16_t poll_interval;
}modbus[300];

bool commandToStruct(char command[50]);
void printStruct(uint16_t index);
void structToPayload(void);

int main(){

    char command[50] = "$IMEI,ADD,1,2,400001,4,float,ABCD,voltage;";
    commandToStruct(command);
    strcpy(modbus[1].result,"230.2");

    memset(command,0,sizeof(command));
    strcpy(command,"$IMEI,ADD,2,5,400001,4,float,ABCD,voltage;");
    commandToStruct(command);
    strcpy(modbus[2].result,"234.0");

    memset(command,0,sizeof(command));
    strcpy(command,"$IMEI,ADD,299,9,400001,4,float,ABCD,voltage;");
    commandToStruct(command);
    strcpy(modbus[299].result,"234.0");

//    printStruct(1);        //printing the specified index structure
    //printStruct(2);

    structToPayload();          //CONVERTING STRUCTURE TO PAYLOAD


    printf("\n%s",payload);

}











//FOR CONVERTING TO STRUCT
bool commandToStruct(char command[50]){

    uint16_t index;
    sscanf(command,"%*[^,]%*c%*[^,]%*c%hu%*[]",&index);           //READING INDEX NUMBER

    if(index==0)
        return false;                                           //INDEX DON,T ZERO

    memset(&modbus[index],0,sizeof(modbus[index]));             //CLEARING STRUCT

    //STORING TO CORRESPONDING STRUCTURE ARRAY
    sscanf(command,"%*[^,]%*c%*[^,]%*c%hu%*c%hu%*c%u%*c%hhu%*c%[^,]%*c%[^,]%*c%[^;]%*[]",&modbus[index].index,&modbus[index].slave_id,&modbus[index].register_address,&modbus[index].function_code,&modbus[index].data_type,&modbus[index].type_order,&modbus[index].Alias_Name);
    return true;

}


//FOR PRINT THE STRUCT
void printStruct(uint16_t index){

    printf("\n-------------------------------------------------\n");
    printf("index: %hu\nslave ID: %hu\nregister Address: %u\nfunction code: %hhu\ndata type: %s\ntype order: %s\nalias name: %s\nresult: %s\npoll intervell: %hu\n",modbus[index].index,modbus[index].slave_id,modbus[index].register_address,modbus[index].function_code,modbus[index].data_type,modbus[index].type_order,modbus[index].Alias_Name,modbus[index].result,modbus[index].poll_interval);
    printf("-------------------------------------------------\n");
}

//CREATING PAYLOAD
void structToPayload(){

    uint16_t payloadIndex=0;
    uint16_t structIndex=0;
    char tempSlaveID[4];

    memset(payload,0,sizeof(payload));

    strcpy(payload,"[");

    for(structIndex=1;structIndex<300;structIndex++){

        if(modbus[structIndex].index!=0){               //IF MODEBUS ARRAY HAS DATA

            strcpy(payload+strlen(payload),"{");
            strcpy(payload+strlen(payload),"\"");
            strcpy(payload+strlen(payload),modbus[structIndex].Alias_Name);
            strcpy(payload+strlen(payload),"\"");
            strcpy(payload+strlen(payload),":");
            //strcpy(payload+strlen(payload),"\"");
            strcpy(payload+strlen(payload),modbus[structIndex].result);
            //strcpy(payload+strlen(payload),"\"");
            strcpy(payload+strlen(payload),",");
            strcpy(payload+strlen(payload),"\"slaveID\":");
            memset(tempSlaveID,0,4);
            sprintf(tempSlaveID,"%d",modbus[structIndex].slave_id);
            strcpy(payload+strlen(payload),tempSlaveID);
            strcpy(payload+strlen(payload),"},");

        }

    }

    strcpy(payload+(strlen(payload)-1),"]");





}









