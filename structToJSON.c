/*
*   CONVERTING STRUCTURE TO PAYLOAD
*
*   In this program, converting command and storing to structure array that specified in command.
*   Here there are two funciton used.
*
*   1- commandToStruct(char command[50]);
*
*       - passing the command. converting command and storing to structure array that specified in command.
*
*       @params     : command that come from server
*       @return     : if index value in command is zero or greater than 299, return false. otherwise return true
*
*   2- printStruct(uint16_t index);
*
*       - print the structure array that specified.
*
*       @params     : index number of structure in structure array
*
*   3- structToPayload(void);
*
*       - Converting structure to payload. the payload variable and structure has declared glably, so we does not need any arguments to function
*
*       @return     : if modbus structure array has data, then return true. else return false
*
*/



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
bool structToPayload(void);

int main(){

    char command[50] = "$IMEI,ADD,1,2,400001,4,float,ABCD,voltage;";
    commandToStruct(command);                             //CONVERTING COMMAND TO STRUCTURE AND STORING CORRESPONDING INDEX
    strcpy(modbus[1].result,"230.2");               //READING INPUT AND STORING TO STRUCT

    memset(command,0,sizeof(command));
    strcpy(command,"$IMEI,ADD,2,5,400001,4,float,ABCD,voltage;");
    commandToStruct(command);
    strcpy(modbus[2].result,"234.0");

    memset(command,0,sizeof(command));
    strcpy(command,"$IMEI,ADD,299,9,400001,4,float,ABCD,voltage;");
    commandToStruct(command);
    strcpy(modbus[299].result,"234.0");



    if(structToPayload()){                      //CONVERTING STRUCTURE TO PAYLOAD
        printf("%s\n",payload);
    }else{
        printf("modbus structure has no data");
    }


}











//FOR CONVERTING TO STRUCT
bool commandToStruct(char command[50]){

    uint16_t index;

    strtok(command,",");
    strtok(0,",");
    sscanf(strtok(0,","),"%hu",&index);         //READING INDEX NUMBER

    if(index==0||index >=300)                                   //INDEX DON,T ZERO OR GREATERR THAN 299
        return false;

    memset(&modbus[index],0,sizeof(modbus[index]));             //CLEARING STRUCT

    //STORING TO CORRESPONDING STRUCTURE ARRAY
    modbus[index].index = index;
    sscanf(strtok(0,","),"%hu",&modbus[index].slave_id);
    sscanf(strtok(0,","),"%u",&modbus[index].register_address);
    sscanf(strtok(0,","),"%hhu",&modbus[index].function_code);
    sscanf(strtok(0,","),"%s",&modbus[index].data_type);
    sscanf(strtok(0,","),"%s",&modbus[index].type_order);
    sscanf(strtok(0,";"),"%s",&modbus[index].Alias_Name);

    return true;

}


//FOR PRINT THE STRUCT
void printStruct(uint16_t index){

    printf("\n-------------------------------------------------\n");
    printf("index: %hu\nslave ID: %hu\nregister Address: %u\nfunction code: %hhu\ndata type: %s\ntype order: %s\nalias name: %s\nresult: %s\npoll intervell: %hu\n",modbus[index].index,modbus[index].slave_id,modbus[index].register_address,modbus[index].function_code,modbus[index].data_type,modbus[index].type_order,modbus[index].Alias_Name,modbus[index].result,modbus[index].poll_interval);
    printf("-------------------------------------------------\n");
}

//CREATING PAYLOAD
bool structToPayload(){

    uint16_t payloadIndex=0;
    uint16_t structIndex=0;
    char tempSlaveID[4];
    bool structEmpty = true;

    memset(payload,0,sizeof(payload));

    strcpy(payload,"[");

    for(structIndex=1;structIndex<300;structIndex++){

        if(modbus[structIndex].index!=0){               //IF MODEBUS ARRAY HAS DATA

            structEmpty =false;
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
            //strcpy(payload+strlen(payload),"\"");
            memset(tempSlaveID,0,4);
            sprintf(tempSlaveID,"%hu",modbus[structIndex].slave_id);
            strcpy(payload+strlen(payload),tempSlaveID);
            //strcpy(payload+strlen(payload),"\"");
            strcpy(payload+strlen(payload),"},");

        }

    }

    strcpy(payload+(strlen(payload)-1),"]");

    if(structEmpty){                             //CLEARING IF MODBUS ARRAY HAS NO DATA
        memset(payload,0,sizeof(payload));
        return false;
    }
    return true;

}









