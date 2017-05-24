/////////////////////////////////////////////////////////////////////////////// 
#include <string.h> 
#include <stdlib.h> 
/////////////////////////////////////////////////////////////////////////////// 
typedef struct _DateTimeInfo 
{ 
   int8 Day; 
   int8 Month; 
   int8 Year; 
   int8 Hour; 
   int8 Minute; 
   int8 Second; 
} DateTimeInfo; 
//////////////////////////////////////// 
typedef struct _GPRMCInfo 
{ 
   char Valid; 
   DateTimeInfo DT; 
   float Latitude; 
   char N_S; 
   float Longitude; 
   char E_W; 
   float Speed; 
} GPRMCInfo; 
/////////////////////////////////////////////////////////////////////////////// 
//copy string (pos n to pos m) from s2 to s1 
char* StrnmCpy(char *s1, char *s2, size_t n, size_t m) 
{ 
   int8 i; 
   char *s; 
    
   for (s=s1, i=n, s2+=n; i<=m; i++) 
      *s++ = *s2++; 
   *s = '\0'; 
    
   return s1; 
} 
/////////////////////////////////////////////////////////////////////////////// 
// find c in s starting from pos st 
int8 StrFnd(char *s, char c, size_t st) 
{ 
   int8 l; 
    
   for (l=st, s+=st ; *s != '\0' ; l++, s++) 
      if (*s == c) 
         return l; 
   return -1; 
} 
/////////////////////////////////////////////////////////////////////////////// 
void GPRMC_decode(char *GPRMCStr, GPRMCInfo *RMCInfo) 
{ 
   int8 p1, p2; 
   char TempStr[16]; 
    
   p1 = StrFnd(GPRMCStr, ',', 0);      //find first comma 
   if (p1 == 6) 
   { 
      //check for valid packet: 
      if ( (StrFnd(GPRMCStr, 'A', 0) == 17) && (GPRMCStr[0]=='$')) //valid? 
      {
         RMCInfo->Valid = 'A'; 
          
         //Get time 
         p1 = StrFnd(GPRMCStr, ',', 0);      //find first comma 
         p2 = StrFnd(GPRMCStr, ',', p1+1);   //find next comma 
         RMCInfo->DT.Hour = atoi(StrnmCpy(TempStr, GPRMCStr, p1+1, p1+2));   //hour 
         RMCInfo->DT.Minute = atoi(StrnmCpy(TempStr, GPRMCStr, p1+3, p1+4)); //min 
         RMCInfo->DT.Second = atoi(StrnmCpy(TempStr, GPRMCStr, p1+5, p1+6)); //sec 
          
         //Get latitude & direction 
         p1 = StrFnd(GPRMCStr, ',', p2+1);   //find next comma 
         p2 = StrFnd(GPRMCStr, ',', p1+1);   //find next comma 
         RMCInfo->Latitude = atof(StrnmCpy(TempStr, GPRMCStr, p1+1, p2-1)); 
         RMCInfo->N_S = GPRMCStr[p2+1]; 
          
         //Get longitude & direction 
         p1 = StrFnd(GPRMCStr, ',', p2+1);   //find next comma 
         p2 = StrFnd(GPRMCStr, ',', p1+1);   //find next comma 
         RMCInfo->Longitude = atof(StrnmCpy(TempStr, GPRMCStr, p1+1, p2-1)); 
         RMCInfo->E_W = GPRMCStr[p2+1]; 
          
         //Get speed 
         p1 = StrFnd(GPRMCStr, ',', p2+1);   //find next comma 
         p2 = StrFnd(GPRMCStr, ',', p1+1);   //find next comma 
         RMCInfo->Speed = atof(StrnmCpy(TempStr, GPRMCStr, p1+1, p2-1)); 
          
         //Get date 
         p1 = StrFnd(GPRMCStr, ',', p2+1);   //find next comma 
         p2 = StrFnd(GPRMCStr, ',', p1+1);   //find next comma 
         RMCInfo->DT.Day = atoi(StrnmCpy(TempStr, GPRMCStr, p1+1, p1+2));  //dd 
         RMCInfo->DT.Month = atoi(StrnmCpy(TempStr, GPRMCStr, p1+3, p1+4));//mm 
         RMCInfo->DT.year = atoi(StrnmCpy(TempStr, GPRMCStr, p1+5, p1+6)); //yy 
      } 
      else                                //not valid 
      { 
         RMCInfo->Valid = 'V'; 
      } 
   } 
} 
