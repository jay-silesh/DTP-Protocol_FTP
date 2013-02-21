#include <iostream>
#include <string.h>
#include <fstream>
#include <cstring>


void blank()
{
 ofstream out("output.txt", ios::out);
  if(!out) {
    cout << "Cannot open file.\n";
  }
  out.close();

}

void writing(char *buffer)
{
  FILE * pFile;
  pFile = fopen ( "output.txt" , "ab+" );
  fwrite (buffer , 1 , strlen(buffer) , pFile);
  fclose (pFile);

}


char * file_handling(int packet_no)
{
  FILE * pFile;
  char *buffer=(char *)malloc(1550);
  size_t result;
  int packet_size=1500;
  pFile = fopen ( "hello.txt" , "rb" );
 
  if (pFile==NULL)
  {
    fputs ("File error",stderr); return NULL;
  }
  
  
    fseek (pFile , packet_no*packet_size , SEEK_SET);
    
    
    result = fread (buffer,1,packet_size,pFile);
    if (buffer != NULL) 
    {
      buffer[strlen(buffer)]='\0';
      char *buff;
      buff=buffer;

      fclose (pFile);
      return buff;
    }
    else
    {
      fclose (pFile);
      return NULL;    
    }
      

}
