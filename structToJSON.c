#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdint.h>




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
    strcpy(command,"$IMEI,ADD,2,2,400001,4,float,ABCD,voltage;");
    commandToStruct(command);
    strcpy(modbus[2].result,"234.0");

//    printStruct(1);        //printing the specified index structure
//    printStruct(2);

    structToPayload();

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

    char payload[7800];
    memset(payload,0,sizeof(payload));



}









