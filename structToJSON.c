/**
*   CONVERTING STRUCTURE TO PAYLOAD
*
*   In this program, converting command and storing to structure array that specified in command.
*   Here there are two funciton used.
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

bool commandToStruct(char command[70]);
void printStruct(uint16_t index);
bool checkInteger(char *,uint32_t range);
bool structToPayload(void);

int main(){

    char command[50] = "$IMEI,ADD,1,2,400001,4,float,ABCD,voltage;";
    if(!commandToStruct(command))                             //CONVERTING COMMAND TO STRUCTURE AND STORING CORRESPONDING INDEX
        printf("invalid command\n");
    strcpy(modbus[1].result,"230.2");               //READING INPUT AND STORING TO STRUCT

    memset(command,0,sizeof(command));
    strcpy(command,"$IMEI,ADD,2,5,400001,4,float,ABCD,voltage;");
    if(!commandToStruct(command))                             //CONVERTING COMMAND TO STRUCTURE AND STORING CORRESPONDING INDEX
        printf("invalid command\n");
    strcpy(modbus[2].result,"234.0");

    memset(command,0,sizeof(command));
    strcpy(command,"$IMEI,ADD,299,255,400001,4,float,ABCD,voltage;");
    if(!commandToStruct(command))                             //CONVERTING COMMAND TO STRUCTURE AND STORING CORRESPONDING INDEX
        printf("invalid command\n");
    strcpy(modbus[299].result,"234.0");



    if(structToPayload()){                      //CONVERTING STRUCTURE TO PAYLOAD
        printf("%s\n",payload);
    }else{
        printf("modbus structure has no data");
    }


}











/**
*   commandToStruct(char command[50]);
*
*      FOR CONVERTING TO STRUCT
*       - passing the command. converting command and storing to structure array that specified in command.
*
*       @params     : command that come from server
*       @return     : if index value in command is zero or greater than 299, return false.
*                     if command syntax is error, return false
*                     if slave is greater than 255, return false
*                     if length of values in command is greater than 10 except IMEI, return false
*                     if register address greater than 4294967295, return false
*                     if function code greater than 255, return false
*                     else return true
*/

bool commandToStruct(char *com){
    char command[100];
    memset(command,0,70);
    strcpy(command,com);
    command[sizeof(command)-1]=0;
    uint16_t index;
    char buffer[12];
    uint8_t count=0;
    uint8_t commaError=0;
    bool IMEI=true;


    //CHEKING ERROR IN COMMAND
    if(command[strlen(command)-1]!=';')
        return false;
    for(index=0;index<strlen(command);index++){
        commaError++;
        if(command[index]==','){
            if(commaError==1||commaError>11&&(!IMEI)) return false;
            count++;
            commaError =0;
            IMEI =false;
        }
        if(commaError>11&&(!IMEI)) return false;
    }
    index =0;
    if(count!=8)return false;       //COUNTING COMMAS


    strtok(command,",");
    strtok(0,",");
    memset(buffer,0,12);
    strcpy(buffer,strtok(0,","));
    if(!checkInteger(buffer,299)){memset(&modbus[index],0,sizeof(modbus[index]));return false;}     //ERROR DETETECTION
    sscanf(buffer,"%hu",&index);         //READING INDEX NUMBER

    if(index==0)                                   //INDEX DON,T ZERO OR GREATERR THAN 299
        return false;

    memset(&modbus[index],0,sizeof(modbus[index]));             //CLEARING STRUCT

    //STORING TO CORRESPONDING STRUCTURE ARRAY
    modbus[index].index = index;
    memset(buffer,0,12);
    strcpy(buffer,strtok(0,","));
    buffer[11]=0;
    if(!checkInteger(buffer,255)){memset(&modbus[index],0,sizeof(modbus[index]));return false;}     //ERROR DETETECTION
    sscanf(buffer,"%hu",&modbus[index].slave_id);

    memset(buffer,0,12);
    strcpy(buffer,strtok(0,","));
    buffer[11]=0;
    if(!checkInteger(buffer,4294967295)){memset(&modbus[index],0,sizeof(modbus[index]));return false;}     //ERROR DETETECTION
    sscanf(buffer,"%u",&modbus[index].register_address);

    memset(buffer,0,12);
    strcpy(buffer,strtok(0,","));
    buffer[11]=0;
    if(!checkInteger(buffer,255)){memset(&modbus[index],0,sizeof(modbus[index]));return false;}     //ERROR DETETECTION
    sscanf(buffer,"%hhu",&modbus[index].function_code);

    sscanf(strtok(0,","),"%s",&modbus[index].data_type);
    sscanf(strtok(0,","),"%s",&modbus[index].type_order);
    sscanf(strtok(0,";"),"%s",&modbus[index].Alias_Name);

    return true;

}



/**
*   printStruct(uint16_t index);
*
*   - print the structure array that specified.
*
*   @params     : index number of structure in structure array
*/
void printStruct(uint16_t index){

    printf("\n-------------------------------------------------\n");
    printf("index: %hu\nslave ID: %hu\nregister Address: %u\nfunction code: %hhu\ndata type: %s\ntype order: %s\nalias name: %s\nresult: %s\npoll intervell: %hu\n",modbus[index].index,modbus[index].slave_id,modbus[index].register_address,modbus[index].function_code,modbus[index].data_type,modbus[index].type_order,modbus[index].Alias_Name,modbus[index].result,modbus[index].poll_interval);
    printf("-------------------------------------------------\n");
}


/**
*
*   checkInteger(char *val,uint32_t range);
*
*   - to check the given buffer is integer or not. if it is integer, it convert to int
*     and also check the given integer range
*
*   @params:    val     - pointer of the character array
*   @params:    range   - range of given integer
*
*   @return:    if given character array is not integer, return false
*               if converted integer is not within the given range, return false
*               else return true
*
*/
bool checkInteger(char *val,uint32_t range){
    char value[12];
    bool TEN=false;
    memset(value,0,12);
    strcpy(value,val);
    uint32_t i=0;
    for(i=0;i<11;i++){
        if(value[i]==0)break;
        if((value[i]<48)||(value[i]>57)){

            return false;
        }
    }
    if(i==10)TEN=true;
    i=0;
    sscanf(value,"%u",&i);
    if(i>range||(TEN==true)&&(i<=0x2A05F1FF)) return false;
    return true;

}


/**
*
*
*   3- structToPayload(void);
*
*       - Converting structure to payload. the payload variable and structure has declared glably,
*           so we does not need any arguments to function
*
*       @return:     if modbus structure array has data, then return true. else return false
*
*
*
*
*/
bool structToPayload(){

    uint16_t structIndex=0;
    uint16_t payloadLenght=0;
    char tempSlaveID[4];
    bool structEmpty = true;

    memset(payload,0,sizeof(payload));

    strcpy(payload,"[");
    payloadLenght++;

    for(structIndex=1;structIndex<300;structIndex++){

        if(modbus[structIndex].index!=0){               //IF MODEBUS ARRAY HAS DATA

            structEmpty =false;
            strcpy(payload+payloadLenght,"{");
            payloadLenght++;
            strcpy(payload+payloadLenght,"\"");
            payloadLenght++;
            strcpy(payload+payloadLenght,modbus[structIndex].Alias_Name);
            payloadLenght += strlen(modbus[structIndex].Alias_Name);
            strcpy(payload+payloadLenght,"\"");
            payloadLenght++;
            strcpy(payload+payloadLenght,":");
            payloadLenght++;
            //strcpy(payload+strlen(payload),"\"");
            strcpy(payload+payloadLenght,modbus[structIndex].result);
            payloadLenght += strlen(modbus[structIndex].result);
            //strcpy(payload+strlen(payload),"\"");
            strcpy(payload+payloadLenght,",");
            payloadLenght++;
            strcpy(payload+payloadLenght,"\"slaveID\":");
            payloadLenght += strlen("\"slaveID\":");
            //strcpy(payload+strlen(payload),"\"");
            memset(tempSlaveID,0,4);
            sprintf(tempSlaveID,"%hu",modbus[structIndex].slave_id);
            strcpy(payload+payloadLenght,tempSlaveID);
            payloadLenght+= strlen(tempSlaveID);
            //strcpy(payload+strlen(payload),"\"");
            strcpy(payload+payloadLenght,"},");
            payloadLenght = payloadLenght + 2;
        }

    }

    strcpy(payload+(payloadLenght-1),"]");
    payloadLenght =0;
    if(structEmpty){                             //CLEARING IF MODBUS ARRAY HAS NO DATA
        memset(payload,0,sizeof(payload));
        return false;
    }
    return true;

}









