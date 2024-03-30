#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
  

#include <string.h>

#include "graph.h"
#include "cjson/cJSON.h" 

#define WEIGHTS 5
#define LR 0.3
#define RUN 1
#define STOP 0

// #define DEBUG
// #define RELEASE


double res = 0;
double error=0.0;
static double w[WEIGHTS]={0,0,0,0,1};

void  distrib_weights(bool *inputs,bool value,bool result) {
    
    double delta =0.0;

      error=+value-result;  
    #ifdef DEBUG
      printf("%f error in ditrib \t",error);
    #endif 
    for(int i=0;i<4;i++){
      delta = LR * error * *(inputs+i);
      w[i+1] = w[i] + delta;
      
#ifdef DEBUG
      printf("w[%d]=%f",i,w[i]);
#endif
    }
  
}

double first_activate(const double net) {
     return net>=0 ? 1 : 0;
}

void _null_weights(bool flag)
{

if(flag==true) 
{
  for(int i=0; i<WEIGHTS;++i) 
  {
      printf("\n I=%d \n ",i);
      w[i]=0;
  }
}else{
  printf("\n weights don't change \n ");
}

}

bool*  _table()
{
  
  
    int rows = 1 << WEIGHTS; // количество строк в таблице (2^n)

// вывод заголовков столбцов
    printf("====================Таблица истиности: ==================== \n");
    printf("||");
    for (int i = 0; i < 4; ++i) {
        printf("x%d ||\t", i+1);
    }
    printf("!x1 * x2 * (!x3 + x4)\t || \n");
 
  bool X[16][4];

  bool* result = (bool *)malloc(16 * sizeof(bool[4]));
 
    // Заполнение массива данными таблицы истиности в цикле
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            X[i][j] = (i >> (3 - j)) & 1;
        }
    }
    printf("-----------------------------------------------------------\n");
    // Вывод массива
    for (int i = 0; i < 16; i++) {

        result[i]=!X[i][0]*X[i][1]*(!X[i][2]+X[i][3]);

        printf("|| %d ||\t %d ||\t %d ||\t %d ||\t\t  %d\t\t ||",X[i][0],X[i][1],X[i][2],X[i][3],result[i]);
        printf("\n");
    }
    printf("===========================================================\n");
    
    bool* ptr = &result[0]; 
    return ptr;
}
bool* output(){

  bool (*X)[4] = (bool (*)[4])malloc(16 * sizeof(bool[4]));

     for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            X[i][j] = (i >> (3 - j)) & 1;
        };
    };

  void* ptr = &X[0][0];
  
  return ptr;
}
double net(const bool *inputs,int size) {


   for(int i=1;i<size;i++){
    res =+ w[i] * inputs[i]+w[4];
  }
#ifdef DEBUG

 printf("\n Res: %f ",res );

#endif
  return res;
}

double second_activate(const double net){
   
  return res=0.5*(net/(1+fabs(net)+1));

}


bool boolean_func(bool result, bool *inputs)
{     
#ifdef DEBUG
      printf("\ninputs[1]: %d\n",inputs[0]);
      printf("inputs[2]: %d\n",inputs[1]);
      printf("inputs[3]: %d\n",inputs[2]);
      printf("inputs[4]: %d\n",inputs[3]);
#endif     
      
      bool value = !inputs[0]*inputs[1]*(!inputs[2]+inputs[3]);

      if (value==result)
      {
#ifdef DEBUG
        printf("True - Expected  %d : Actual %d \n ",value,result);
#endif 
        return true;
      }else{
#ifdef DEBUG
        printf("False - Expected %d : Actual %d \n",value,result);
#endif
        distrib_weights(inputs,value,result);            
        return false;
      }
}


int run(bool *pF,bool *pT){

bool inputs[4];
int error=0;
bool *iPointer_F=pF;
bool *iPointer_T=pT;
for(int j=0;j<16;j++){

for(int i=0; i<4; i++){
    // printf("Итерация %d", i);
    inputs[i] = *(iPointer_T+i);
  
}
  
  iPointer_T+=(sizeof(bool)*4);
  if(boolean_func(first_activate(net(inputs,4)),inputs)==false)
  {
    error++;

  }else{
    continue;
  }


}
iPointer_T=pT;
iPointer_F=pF;
return error;

}


int run_second(bool *pF,bool *pT)
{

bool inputs[4];
int error=0;
bool *iPointer_F=pF;
bool *iPointer_T=pT;
for(int j=0;j<16;j++){

for(int i=0; i<4; i++){
    // printf("Итерация %d", i);
    inputs[i] = *(iPointer_T+i);
  
}
  
  iPointer_T+=(sizeof(bool)*4);
  if(boolean_func(second_activate(net(inputs,4)),inputs)==false)
  {
    error++;

  }else{
    continue;
  }
  

  


}
    iPointer_T=pT;
    iPointer_F=pF;
    return error;

}



int main(int argc, char** argv) {
  int i,j=0;
  srand(time(NULL));  
  


  bool *pTable = output();
  bool *pFunc_ = _table();
  cJSON *root = cJSON_CreateObject(); // Создание корневого объекта JSON
  

  int era=0;

  saveIntegerValueToJsonFile("data.json","error",error,root);
  int error = run(pFunc_,pTable+era);
 


  while(error>0){
  saveIntegerValueToJsonFile("data.json","error",error,root);
  error = run(pFunc_,pTable+era);
  printf("\n sum_error %d", error);
  era+=4;
  
    
  if( error == 0 ){
  
    break;

  }else{

    continue;
  }
  };
  renderGraph("data.json");

_null_weights(true);

era=0;

cJSON_Delete(root); // очистка памяти после использования


cJSON *root_second = cJSON_CreateObject(); // Создание корневого объекта JSON
saveIntegerValueToJsonFile("data2.json","error",error,root_second);
error = run_second(pFunc_,pTable+era);

while(error>0){
  
    saveIntegerValueToJsonFile("data2.json","error",error,root_second);

    error = run_second(pFunc_,pTable+era);
  
    
    era+=4;
    
    
    if(error==0){ 
  
      break;
  
    }else{

      continue;
    
    }
  };
 

  renderGraph("data2.json");


#ifdef RELEASE 
remove("data.json");
remove("data2.json");
#endif 


};
